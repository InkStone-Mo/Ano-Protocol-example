#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Ano-Protocol/ano-base.h"
#include "Ano-Protocol/ano-command.h"
#include "Ano-Protocol/ano-parameter.h"
#include "Ano-Protocol/ano-update.h"
#include "Ano-Protocol/ano-control-info.h"

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int serverSocket;
struct sockaddr_in serverAddr, clientAddr;
socklen_t clientAddrLen = sizeof(clientAddr);
uint8_t buffer[BUFFER_SIZE] = {};

// 传输通道
void transmissionChannel_Cb(const uint8_t *frameData, uint16_t frameDataLen)
{
    if (sendto(serverSocket, frameData, frameDataLen, 0, (struct sockaddr *)&clientAddr, clientAddrLen) == -1) {
        printf("Error sending response\n");
    }
}


void updateBin(uint8_t *upgradeData,uint16_t frameLen, uint16_t frameNumber)
{
    printf("update frame num : %d \n", frameNumber);
}
void updateStart(void)
{
    printf("update start!\n");
}
void updateEnd(void)
{
    printf("update end!\n");
}

int main()
{
/* 自定义数据传输，可以是任意方式，把发送回调注册给Ano_Init即可 */
    memset(&serverAddr, 0, sizeof(serverAddr));// 设置服务器地址信息
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // 创建UDP套接字
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Error creating socket\n");
    }
    // 绑定套接字到指定地址和端口
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        printf("Error binding socket\n");
    }
    printf("UDP Server is running on port %d...\n", SERVER_PORT);

/* 定义设备信息 */
    T_DevInfo devInfo ={
        .DEV_ID = 0x08, 
        .HW_VER = 0x08,
        .SW_VER = 0x08,
        .BL_VER = 0x08,
        .PT_VER = 0x08,
        .DEVNAME = "update test!"
    };
    Ano_SetDevInfo(&devInfo); // 初始化设备信息
    Ano_Init(transmissionChannel_Cb); // 注册传输通道
    
/* 注册升级回调 */
    T_AnoUpgrade anoUpdate={
        .Ano_UpgradeEnd = updateEnd,
        .Ano_UpgradeSave = updateBin,
        .Ano_UpgradeStart = updateStart
    };
    Ano_UpgradeInit(&anoUpdate);

/* 接收上位机命令并用Ano_ProcessFrame解析 */
    while (1) { // 接收客户端的消息
        int len = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (len == -1) {
            printf("Error receiving message\n");
            continue;
        }
	    Ano_ProcessFrame(buffer, len);
        memset(buffer, '\0', BUFFER_SIZE);
    }

    // 关闭套接字
    close(serverSocket);
    return 0;
}
