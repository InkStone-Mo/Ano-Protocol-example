#include "ano-parameter.h"

static T_ParameterInfo *g_parameterInfo;
static uint16_t g_parameterCount;

void Ano_SetParameterInfo(T_ParameterInfo *parameterInfo, uint16_t parameterCount)
{
  g_parameterCount = parameterCount;
  g_parameterInfo = parameterInfo;

  for(int i=0;i<g_parameterCount;i++){
    switch (g_parameterInfo[i].PAR_TYPE) {
      case PAR_VAL_TYPE_UINT8:
      case PAR_VAL_TYPE_INT8:
        // Allocate memory for uint8_t or int8_t
        g_parameterInfo[i].PAR_VAL = malloc(sizeof(uint8_t));
        break;
      case PAR_VAL_TYPE_UINT16:
      case PAR_VAL_TYPE_INT16:
        // Allocate memory for uint16_t or int16_t
        g_parameterInfo[i].PAR_VAL = malloc(sizeof(uint16_t));
        break;
      case PAR_VAL_TYPE_UINT32:
      case PAR_VAL_TYPE_INT32:
        // Allocate memory for uint32_t or int32_t
        g_parameterInfo[i].PAR_VAL = malloc(sizeof(uint32_t));
        break;
      case PAR_VAL_TYPE_UINT64:
      case PAR_VAL_TYPE_INT64:
        // Allocate memory for uint64_t or int64_t
        g_parameterInfo[i].PAR_VAL = malloc(sizeof(uint64_t));
        break;
      case PAR_VAL_TYPE_FLOAT:
        // Allocate memory for float
        g_parameterInfo[i].PAR_VAL = malloc(sizeof(float));
        break;
      case PAR_VAL_TYPE_DOUBLE:
        // Allocate memory for double
        g_parameterInfo[i].PAR_VAL = malloc(sizeof(double));
        break;
      case PAR_VAL_TYPE_STRING:
        // Allocate memory for string (assuming a default size or dynamically allocate based on requirements)
        g_parameterInfo[i].PAR_VAL = malloc(sizeof(char) * 100); // Change 100 to the desired size
        break;
      default:
        // Handle unknown type
        break;
    }
  }
}

void Ano_SendParameterInfo(uint16_t parameterNum)
{  
  if(parameterNum >= g_parameterCount){
    Ano_LogErr((const char *)"len is large");
    return;
  }
  uint8_t sendInfo[MAX_INFO_LENGTH] = {'\0'};
  sendInfo[0] = Little(g_parameterInfo[parameterNum].PAR_ID);
  sendInfo[1] = Big(g_parameterInfo[parameterNum].PAR_ID);
  sendInfo[2] = g_parameterInfo[parameterNum].PAR_TYPE;
  memset(&g_parameterInfo[parameterNum].PAR_NAME[strlen(g_parameterInfo[parameterNum].PAR_NAME)], 0x00, 20-strlen(g_parameterInfo[parameterNum].PAR_NAME));
  memcpy(&sendInfo[3], g_parameterInfo[parameterNum].PAR_NAME, 20);
  memcpy(&sendInfo[23], g_parameterInfo[parameterNum].PAR_INFO, strlen(g_parameterInfo[parameterNum].PAR_INFO));
  Ano_SendData(ID_PARAMETER_INFO,(const uint8_t *) sendInfo, 23+strlen(g_parameterInfo[parameterNum].PAR_INFO));
}

void Ano_SendParameCount(void)
{
  uint8_t sendInfo[3] = {'\0'};
  sendInfo[0] = 0x01;
  sendInfo[1] = Little(g_parameterCount);
  sendInfo[2] = Big(g_parameterCount);
  printf("num %d\n", g_parameterCount);
  Ano_SendData(ID_PARAMETER_COMMAND,(const uint8_t *)sendInfo, sizeof(sendInfo));
}

void Ano_SendParameValue(uint16_t parameterNum)
{
  if(parameterNum >= g_parameterCount){
    Ano_LogErr((const char *)"no found this parameter\n");
    return;
  }
  uint8_t sendInfo[MAX_INFO_LENGTH] = {'\0'};
  sendInfo[0] = Little(g_parameterInfo[parameterNum].PAR_ID);
  sendInfo[1] = Big(g_parameterInfo[parameterNum].PAR_ID);
  int len = 2;
  switch (g_parameterInfo[parameterNum].PAR_TYPE) {
    case PAR_VAL_TYPE_UINT8:
    case PAR_VAL_TYPE_INT8:
      sendInfo[2] = *(uint8_t *)g_parameterInfo[parameterNum].PAR_VAL;
      len += 1;
      break;

    case PAR_VAL_TYPE_UINT16:
    case PAR_VAL_TYPE_INT16:
      sendInfo[2] = *(uint16_t *)g_parameterInfo[parameterNum].PAR_VAL & 0xFF;
      sendInfo[3] = (*(uint16_t *)g_parameterInfo[parameterNum].PAR_VAL >> 8) & 0xFF;
      len += 2;
      break;

    case PAR_VAL_TYPE_UINT32:
    case PAR_VAL_TYPE_INT32:
      sendInfo[2] = *(uint32_t *)g_parameterInfo[parameterNum].PAR_VAL & 0xFF;
      sendInfo[3] = (*(uint32_t *)g_parameterInfo[parameterNum].PAR_VAL >> 8) & 0xFF;
      sendInfo[4] = (*(uint32_t *)g_parameterInfo[parameterNum].PAR_VAL >> 16) & 0xFF;
      sendInfo[5] = (*(uint32_t *)g_parameterInfo[parameterNum].PAR_VAL >> 24) & 0xFF;
      len += 4;
      break;

    case PAR_VAL_TYPE_UINT64:
    case PAR_VAL_TYPE_INT64:
      sendInfo[2] = *(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL & 0xFF;
      sendInfo[3] = (*(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL >> 8) & 0xFF;
      sendInfo[4] = (*(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL >> 16) & 0xFF;
      sendInfo[5] = (*(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL >> 24) & 0xFF;
      sendInfo[6] = (*(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL >> 32) & 0xFF;
      sendInfo[7] = (*(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL >> 40) & 0xFF;
      sendInfo[8] = (*(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL >> 48) & 0xFF;
      sendInfo[9] = (*(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL >> 56) & 0xFF;
      len += 8;
      break;

    case PAR_VAL_TYPE_FLOAT:
      {
        // Assuming sizeof(float) is 4
        uint32_t temp = *(uint32_t *)g_parameterInfo[parameterNum].PAR_VAL;
        sendInfo[2] = temp & 0xFF;
        sendInfo[3] = (temp >> 8) & 0xFF;
        sendInfo[4] = (temp >> 16) & 0xFF;
        sendInfo[5] = (temp >> 24) & 0xFF;
        len += 4;
      }
      break;

    case PAR_VAL_TYPE_DOUBLE:
      {
        // Assuming sizeof(double) is 8
        uint64_t temp = *(uint64_t *)g_parameterInfo[parameterNum].PAR_VAL;
        sendInfo[2] = temp & 0xFF;
        sendInfo[3] = (temp >> 8) & 0xFF;
        sendInfo[4] = (temp >> 16) & 0xFF;
        sendInfo[5] = (temp >> 24) & 0xFF;
        sendInfo[6] = (temp >> 32) & 0xFF;
        sendInfo[7] = (temp >> 40) & 0xFF;
        sendInfo[8] = (temp >> 48) & 0xFF;
        sendInfo[9] = (temp >> 56) & 0xFF;
        len += 8;
      }
      break;

    case PAR_VAL_TYPE_STRING:
      // Assuming val points to a null-terminated string
      strcpy((char *)sendInfo, (const char *)g_parameterInfo[parameterNum].PAR_VAL);
      len += strlen(g_parameterInfo[parameterNum].PAR_VAL);
      break;

    default:
      // Handle unknown type
      break;
  }
  Ano_SendData(ID_PARAMETER_VALUE_RW, (const uint8_t *)sendInfo, len);
}

void Ano_ParseParCMD(uint8_t *toParseData)
{
  uint8_t parCmd = toParseData[0];
  uint16_t parVal = ((uint16_t)toParseData[2] << 8)|toParseData[1];
  Ano_Log((const char *)"cmd %d : %d", parCmd, parVal);
  switch (parCmd){
    case PAR_CMD_READ_DEV: // 设备信息
      Ano_SendDevInfo();
      Ano_LogDebug((const char *)"send dev info");
      break;
    case PAR_CMD_PAR_NUMBER:  
      Ano_SendParameCount(); // 发送参数数量
      Ano_LogDebug((const char *)"read par number");
      break;
    case PAR_CMD_PAR_VAL:
      Ano_SendParameValue(parVal); // 发送参数对应的数值
      Ano_LogDebug((const char *)"par value %d", parCmd);
      break;
    case PAR_CMD_PAR_INFO:
      printf("val-id : %d", parVal);
      Ano_SendParameterInfo(parVal); // 发送对应参数ID参数信息
      Ano_LogDebug((const char *)"par info");
      break;
    case PAR_CMD_PAR_CTRL:
      Ano_LogDebug((const char *)"par ctrl");
      break;
    default:
      Ano_LogErr((const char *)"unkondw cmd");
      break;
  }
}

void Ano_WriteParameValue(uint8_t *parameterData, uint16_t parameterDataLen)
{
  printf("parameterDataLen : %d\n", parameterDataLen);
  uint16_t parNum = ((uint16_t)parameterData[1] << 8)|parameterData[0];
  switch (g_parameterInfo[parNum].PAR_TYPE) {
    case PAR_VAL_TYPE_UINT8:
    case PAR_VAL_TYPE_INT8:
      *(uint8_t *)g_parameterInfo[parNum].PAR_VAL = parameterData[2];
      break;

    case PAR_VAL_TYPE_UINT16:
    case PAR_VAL_TYPE_INT16:
      *(uint16_t *)g_parameterInfo[parNum].PAR_VAL = parameterData[2] | (parameterData[3] << 8);
      break;

    case PAR_VAL_TYPE_UINT32:
    case PAR_VAL_TYPE_INT32:
      *(uint32_t *)g_parameterInfo[parNum].PAR_VAL = parameterData[2] | (parameterData[3] << 8) | (parameterData[4] << 16) | (parameterData[5] << 24);
      break;

    case PAR_VAL_TYPE_UINT64:
    case PAR_VAL_TYPE_INT64:
      *(uint64_t *)g_parameterInfo[parNum].PAR_VAL = ((uint64_t)parameterData[2]) | ((uint64_t)parameterData[3] << 8) | ((uint64_t)parameterData[4] << 16) | ((uint64_t)parameterData[5] << 24) |
                ((uint64_t)parameterData[6] << 32) | ((uint64_t)parameterData[7] << 40) | ((uint64_t)parameterData[8] << 48) | ((uint64_t)parameterData[9] << 56);
      break;

    case PAR_VAL_TYPE_FLOAT:
      {
        // Assuming sizeof(float) is 4
        uint32_t temp = parameterData[2] | (parameterData[3] << 8) | (parameterData[4] << 16) | (parameterData[5] << 24);
        *(float *)g_parameterInfo[parNum].PAR_VAL = *((float *)&temp);
      }
      break;

    case PAR_VAL_TYPE_DOUBLE:
      {
        // Assuming sizeof(double) is 8
        uint64_t temp = ((uint64_t)parameterData[2]) | ((uint64_t)parameterData[3] << 8) | ((uint64_t)parameterData[4] << 16) | ((uint64_t)parameterData[5] << 24) |
                ((uint64_t)parameterData[6] << 32) | ((uint64_t)parameterData[7] << 40) | ((uint64_t)parameterData[8] << 48) | ((uint64_t)parameterData[9] << 56);
        *(double *)g_parameterInfo[parNum].PAR_VAL = *((double *)&temp);
      }
      break;

    case PAR_VAL_TYPE_STRING:
      // Assuming g_parameterInfo[parNum].PAR_VAL points to a buffer with enough space
      strcpy((char *)g_parameterInfo[parNum].PAR_VAL, (const char *)parameterData);
      break;

    default:
      // Handle unknown type
      break;
  }
}
