/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#include "Ano-Protocol/ano-base.h"
#include "Ano-Protocol/ano-command.h"
#include "Ano-Protocol/ano-parameter.h"
#include "Ano-Protocol/ano-control-info.h"
#include "Ano-Protocol/ano-update.h"

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_0)
#define RXD_PIN (GPIO_NUM_1)

static T_ParameterInfo parameterInfo[] = {
        {0,PAR_VAL_TYPE_UINT8,"test parameter1", NULL,"test!"},
        {1,PAR_VAL_TYPE_UINT16,"test parameter2", NULL,"test!"},
        {2,PAR_VAL_TYPE_UINT32,"test parameter99", NULL,"test!?"},
    };

static void writeCmdInfo_Cb(const uint8_t *cmdInfoData, uint16_t cmdInfoDataLen)
{ // 命令回调函数，不建议在此函数做耗时操作。
    char getCmdInfo[200];
    getCmdInfo[0] = cmdInfoData[0];
    getCmdInfo[1] = cmdInfoData[1];
    getCmdInfo[2] = cmdInfoData[2];

    Ano_LogDebug("cmd : %02x %02x %02x\n", getCmdInfo[0],getCmdInfo[1],getCmdInfo[2]);
    Ano_LogDebug("the parameter now : %d %d %ld", *(uint8_t *)parameterInfo[0].PAR_VAL, *(uint16_t *)parameterInfo[1].PAR_VAL, *(uint32_t *)parameterInfo[2].PAR_VAL);
}

static void rx_task(void *arg)
{
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            Ano_ProcessFrame(data, (uint16_t)rxBytes); // 处理接收的数据
        }
    }
    free(data);
}

void init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 1000000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

static void transmissionChannel_CB(const uint8_t *sendData, uint16_t dataLen)
{
    const int txBytes = uart_write_bytes(UART_NUM_1, sendData, dataLen);
}

void app_main(void)
{
    init();

    /******** 匿名协议示例 ***********/

    Ano_Init(transmissionChannel_CB); //注册发送通道
    static T_DevInfo devInfo ={
            .DEV_ID = 0x07, 
            .HW_VER = 0x06,
            .SW_VER = 0x05,
            .BL_VER = 0x04,
            .PT_VER = 0x03,
            .DEVNAME = "esp32 uart test!"
        };
    Ano_SetDevInfo(&devInfo);
    Ano_SetParameterInfo(parameterInfo, sizeof(parameterInfo)/sizeof(T_ParameterInfo)); // 注册参数信息
    static T_CMDInfo cmdInfo[] = {
        {{0}, {0x00,0x00,0x01},{CMD_VAL_TYPE_UINT8,0x00,0x00,0x00,0x00,0x00,0x00,0x00},{"test1"},{"is test1"},{NULL}},
        {{1}, {0x00,0x00,0x02},{CMD_VAL_TYPE_UINT8,CMD_VAL_TYPE_UINT16,CMD_VAL_TYPE_UINT32,CMD_VAL_TYPE_FLOAT,0x00,0x00,0x00,0x00},{"test2"},{"is test2"},{NULL}},
    };
    Ano_SetCmdInfo(cmdInfo, sizeof(cmdInfo)/sizeof(T_CMDInfo), writeCmdInfo_Cb);

    /********************************/

    xTaskCreate(rx_task, "uart_rx_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
}
