#include "ano-base.h"
#include "ano-command.h"
#include "ano-parameter.h"
#include "ano-update.h"

#define FRAME_HEADER 0xAB
#define FIXED_FRAME_LENGTH 8

const uint8_t FRAME_HEAD = 0xAB;
const uint8_t FRAME_D_ADDR = 0xFE;
const uint8_t FRAME_S_ADDR = 0x08;

static T_DevInfo *g_devInfo;

// 传输通道
static void (* Ano_transmissionChannel)(const uint8_t *frameData, uint16_t frameDataLen);

static void Ano_Ack(const uint8_t *frameData, uint16_t frameDataLen);
static int Ano_CheckFrame(uint8_t *data_buf);
static void Ano_DebugTransmissionChannel(const uint8_t *frameDate, uint16_t frameDateLen);
static void Ano_SendLog(uint8_t color,const uint8_t *str);

void Ano_Init(void (* transmissionChannel)(const uint8_t *, uint16_t))
{
  if(transmissionChannel != NULL) Ano_transmissionChannel = transmissionChannel;
  else Ano_transmissionChannel = Ano_DebugTransmissionChannel;
}

void Ano_ProcessFrame(uint8_t *frameData, uint16_t frameDataLen)
{
  frameData[frameDataLen] = '\0';
  uint8_t getID = frameData[3];
  uint8_t getData[frameDataLen];
  memcpy(getData, &frameData[6], frameDataLen);
  // printf("frame ID :%02x \n",getID);
  // for(int i =0;i<frameDataLen;i++){
  //   printf("%02x ", frameData[i]);
  // }
  // if(Ano_CheckFrame(frameData) != 1){
  //   printf("this frame is error!\n");
  //   return;
  // }
  // printf("\n");
  switch (getID){
    case ID_CMD_COMMAND_FRAME: // 操作命令
      Ano_WriteCmdInfo(getData, ((uint16_t)frameData[5] << 8)|frameData[4]);
      Ano_Ack(frameData, frameDataLen); //返回校验帧
      break;
    case ID_CMD_FUNCTION_FRAME: // 命令读取
      Ano_ParseFunctionCMD(getData);
      Ano_Ack(frameData, frameDataLen);//返回校验帧
      break;
    case ID_PARAMETER_COMMAND: // 参数读写
      Ano_ParseParCMD(getData); // 进一步解析命令
      Ano_Ack(frameData, frameDataLen);//返回校验帧
      break;
    case ID_PARAMETER_VALUE_RW:
      Ano_WriteParameValue(getData, ((uint16_t)frameData[5] << 8)|frameData[4]);
      Ano_Ack(frameData, frameDataLen);
      break;
    case ID_UPGRADE:
      Ano_ProcessUpgrade(getData, ((uint16_t)frameData[5] << 8)|frameData[4]);
      break;
    default:
      break;
  }
}


/*
帧头  | 源地址 | 目标地址 | 功能码 | 数据长度  | 数据内容 | 和校验   | 附加校验
 ------|--------|----------|--------|-----------|---------|------------|----------
HEAD  | S_ADDR | D_ADDR  | ID   | LEN    | DATA  | SUM CHECK | ADD CHECK
1Byte | 1Byte | 1Byte  | 1Byte |1Byte 1Byte| nByte  | 1Byte   | 1Byte
*/
void Ano_SendData(char cmd, const uint8_t *data, int dataLenght)
{
  if(dataLenght <= 0) return;
  uint8_t *frameData = malloc(sizeof(char) * (dataLenght+8));
  if (frameData == NULL) {
    return;
  }

  frameData[0] = FRAME_HEAD;
  frameData[1] = FRAME_S_ADDR;
  frameData[2] = FRAME_D_ADDR;
  frameData[3] = cmd;
  frameData[4] = Little(dataLenght);
  frameData[5] = Big(dataLenght);
  memcpy(&frameData[6], data, dataLenght);

  char sum_check = 0;
  char add_check = 0;
  for(int i=0;i<(dataLenght+6);i++){
    sum_check += frameData[i]; //从帧头开始，对每一字节进行求和，直到 DATA 区结束
    add_check += sum_check; //每一字节的求和操作，进行一次 sumcheck 的累加
  }
  frameData[6+dataLenght] = sum_check;
  frameData[7+dataLenght] = add_check;
  Ano_transmissionChannel((const uint8_t *)frameData, dataLenght+8);
  free(frameData); // 释放空间
}


void Ano_SendIntData(char cmd, int data)
{
  uint8_t dataTmp[2];
  dataTmp[0] = Little(data);
  dataTmp[1] = Big(data);
  Ano_SendData(cmd ,(const uint8_t *)dataTmp, 2);
}

void Ano_SetDevInfo(T_DevInfo *devInfo)
{
  if(devInfo != NULL){
    g_devInfo = devInfo;
  }
  else Ano_SendLog(1,(const uint8_t *)"devinfo setting error!");
}

void Ano_GetData_fromInt16( uint16_t input_data, uint8_t *data)
{
  if(data == NULL){
    Ano_SendLog(1, (const uint8_t *)"get int16 data error");
  }
  data[0] = input_data & 0xFF;
  data[1] = input_data >> 8;
}


void Ano_GetData_fromInt32( uint32_t input_data, uint8_t *data)
{
  if(data == NULL){
    Ano_SendLog(1, (const uint8_t *)"get int32 data error");
  }
  data[0] = input_data & 0xFF;
  data[1] = (input_data >> 8) & 0xFF;
  data[2] = (input_data >> 16) & 0xFF;
  data[3] = (input_data >> 24) & 0xFF;
}


static void Ano_SendLog(uint8_t color,const uint8_t *str)
{
  size_t len = strlen((const char *)str);
  uint8_t logStr[256] = {'\0'};
  logStr[0] = color;
  memcpy(&logStr[1], str, len);
  Ano_SendData(ID_LOG_STRING_OUTPUT, logStr, len+1);
}

void Ano_LogErr(const char *fmt, ...)
{
  char __ano_log_buf[256];
  va_list args;

  va_start(args, fmt);
  vsnprintf(__ano_log_buf, sizeof(__ano_log_buf), fmt, args);
  va_end(args);

  Ano_SendLog((uint8_t)1, (const uint8_t *)__ano_log_buf);
}

void Ano_LogDebug(const char *fmt, ...)
{
  char __ano_log_buf[256];
  va_list args;

  va_start(args, fmt);
  vsnprintf(__ano_log_buf, sizeof(__ano_log_buf), fmt, args);
  va_end(args);

  Ano_SendLog((uint8_t)2, (const uint8_t *)__ano_log_buf);
}

void Ano_Log(const char *fmt, ...)
{
  char __ano_log_buf[256];
  va_list args;

  va_start(args, fmt);
  vsnprintf(__ano_log_buf, sizeof(__ano_log_buf),(const char *__restrict__)fmt, args);
  va_end(args);

  Ano_SendLog((uint8_t)0, (const uint8_t *)__ano_log_buf);
}

void Ano_SendDevInfo(void)
{
  if(g_devInfo == NULL){
    Ano_SendLog(1,(const uint8_t *)"no dev info");
    printf("no dev info\n");
    return;
  }
  uint8_t devInfo[29] = {'\0'};
  devInfo[0] = g_devInfo->DEV_ID;

  devInfo[1] = Little(g_devInfo->HW_VER);
  devInfo[2] = Big(g_devInfo->HW_VER);

  devInfo[3] = Little(g_devInfo->SW_VER);
  devInfo[4] = Big(g_devInfo->SW_VER);

  devInfo[5] = Little(g_devInfo->BL_VER);
  devInfo[6] = Big(g_devInfo->BL_VER);

  devInfo[7] = Little(g_devInfo->PT_VER);
  devInfo[8] = Big(g_devInfo->PT_VER);
  memcpy(&devInfo[9], g_devInfo->DEVNAME,strlen(g_devInfo->DEVNAME));
  Ano_SendData(ID_DEVICE_INFO,(const uint8_t *)devInfo, 9+strlen(g_devInfo->DEVNAME));
}

/* static function */

static int Ano_CheckFrame(uint8_t *data_buf)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;
  uint16_t flen = data_buf[4] + data_buf[5] * 256;
  for(uint16_t i=0; i < (flen+6); i++){
    sumcheck += data_buf[i]; //从帧头开始，对每一字节进行求和，直到 DATA 区结束
    addcheck += sumcheck; //每一字节的求和操作，进行一次 sumcheck 的累加
  }
  //如果计算出的 sumcheck 和 addcheck 和接收到的 check 数据相等，代表校验通过，反之数据有误
  if(sumcheck == data_buf[flen+6] && addcheck == data_buf[flen+7]){
    return 1; //校验通过
  }else{
    return -1; //校验失败
  }
}

static void Ano_Ack(const uint8_t *frameData, uint16_t frameDataLen)
{
  uint8_t ackData[3] = {'\0'};
  ackData[0] = frameData[3];
  ackData[1] = frameData[frameDataLen-2];
  ackData[2] = frameData[frameDataLen-1];
  
  Ano_SendData(ID_ACK, ackData, 3);
}

static void Ano_DebugTransmissionChannel(const uint8_t *frameDate, uint16_t frameDateLen)
{
  printf(" is no transmission channel !\n");
}
