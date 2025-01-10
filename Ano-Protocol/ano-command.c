#include "Ano-Protocol/ano-command.h"

static T_CMDInfo *g_cmdInfo;
static uint16_t g_cmdCount;

void Ano_WriteCmdInfo(const uint8_t *cmdInfoData, uint16_t cmdInfoDataLen)
{
  uint8_t getCmdInfo[3];
  uint8_t *getCmdData = malloc(cmdInfoDataLen);
  memcpy(getCmdData, &cmdInfoData[3], cmdInfoDataLen-3);
  getCmdInfo[0] = cmdInfoData[0];
  getCmdInfo[1] = cmdInfoData[1];
  getCmdInfo[2] = cmdInfoData[2];
  // printf("cmd : %02x %02x %02x \n", getCmdInfo[0],getCmdInfo[1],getCmdInfo[2]);
  if(cmdInfoDataLen>3) { //如果有带参数
    for(int i=0;i<g_cmdCount;i++){
      if(getCmdInfo[0] == g_cmdInfo[i].CMD[0] &&getCmdInfo[1] == g_cmdInfo[i].CMD[1] &&getCmdInfo[2] == g_cmdInfo[i].CMD[2]){ // 找到对应的命令
        int len = 0;
        for(int j=0;j<8;j++){
          // printf("the type %d-%d:%d \n",i,j,g_cmdInfo[i].VAL_TYPE[j]);
          switch (g_cmdInfo[i].VAL_TYPE[j]) {
            case CMD_VAL_TYPE_UINT8:
            case CMD_VAL_TYPE_INT8:
              *(uint8_t *)g_cmdInfo[i].VAL[j] = getCmdData[len];
              printf("INT8 : %d \n", *(uint8_t *)g_cmdInfo[i].VAL[j]);
              len += 1;
              break;

            case CMD_VAL_TYPE_UINT16:
            case CMD_VAL_TYPE_INT16:
              *(uint16_t *)g_cmdInfo[i].VAL[j] = getCmdData[len] | (getCmdData[len+1] << 8);
              printf("INT16 %u\n", *(uint16_t *)g_cmdInfo[i].VAL[j]);
              len += 2;
              break;

            case CMD_VAL_TYPE_UINT32:
            case CMD_VAL_TYPE_INT32:
              *(uint32_t *)g_cmdInfo[i].VAL[j] = getCmdData[len] | (getCmdData[len+1] << 8) | (getCmdData[len+2] << 16) | (getCmdData[len+3] << 24);
              // printf("uint32_t : %d \n", *(uint32_t *)g_cmdInfo[i].VAL[j]);
              len += 4;
              break;

            case CMD_VAL_TYPE_FLOAT:
              {
                uint32_t temp = getCmdData[len] | (getCmdData[len+1] << 8) | (getCmdData[len+2] << 16) | (getCmdData[len+3] << 24);
                *(float *)(g_cmdInfo[i].VAL[j]) = *((float *)&temp);
                printf("float : %f \n", *(float *)(g_cmdInfo[i].VAL[j]));
                len += 4;
              }
              break;
            default:
              // Handle unknown type
              break;
          }
        }
      }
    }
  }
  if(Ano_WriteCmdInfo_Cb != NULL) Ano_WriteCmdInfo_Cb((const uint8_t *)getCmdInfo, 3); // 回调
  else{
    printf("the Ano_WriteCmdInfo_Cb is NULL!\n");
  }
}

void Ano_SetCmdInfo(T_CMDInfo *cmdInfo, uint16_t cmdCount,void (* writeCmdInfo)(const uint8_t *, uint16_t))
{
  printf("count %d\n", cmdCount);
  g_cmdCount = cmdCount;
  if(cmdInfo != NULL) g_cmdInfo = cmdInfo;
  if(writeCmdInfo != NULL) Ano_WriteCmdInfo_Cb = writeCmdInfo;
  for(int i=0;i<g_cmdCount;i++){
    for(int j=0;j<8;j++){
      switch (g_cmdInfo[i].VAL_TYPE[j]) {
        case CMD_VAL_TYPE_NOT:
          g_cmdInfo[i].VAL[j] = NULL;
          break;
        case CMD_VAL_TYPE_UINT8:
        case CMD_VAL_TYPE_INT8:
          // Allocate memory for uint8_t or int8_t
          g_cmdInfo[i].VAL[j] = malloc(sizeof(uint8_t));
          break;
        case CMD_VAL_TYPE_UINT16:
        case CMD_VAL_TYPE_INT16:
          // Allocate memory for uint16_t or int16_t
          g_cmdInfo[i].VAL[j] = malloc(sizeof(uint16_t));
          break;
        case CMD_VAL_TYPE_UINT32:
        case CMD_VAL_TYPE_INT32:
          // Allocate memory for uint32_t or int32_t
          g_cmdInfo[i].VAL[j] = malloc(sizeof(uint32_t));
          break;
        case CMD_VAL_TYPE_FLOAT:
          // Allocate memory for float
          g_cmdInfo[i].VAL[j] = malloc(sizeof(float));
          break;
        default:
          // Handle unknown type
          break;
      }
    }
  }
}

void Ano_SendCmdCount(void)
{
  uint8_t sendInfo[3] = {'\0'};
  sendInfo[0] = 0x00;
  sendInfo[1] = Little(g_cmdCount);
  sendInfo[2] = Big(g_cmdCount);
  Ano_SendData(ID_CMD_FUNCTION_FRAME, (const uint8_t *)sendInfo,3);
}

void Ano_SendCmdInfo(uint16_t cmdNum)
{
  if(cmdNum >= g_cmdCount){
    return;
  }
  uint8_t sendInfo[MAX_INFO_LENGTH] = {'\0'};
  sendInfo[0] = Little(g_cmdInfo[cmdNum].CMD_ID);
  sendInfo[1] = Big(g_cmdInfo[cmdNum].CMD_ID);
  memcpy(&sendInfo[2], g_cmdInfo[cmdNum].CMD, 3);
  memcpy(&sendInfo[5], g_cmdInfo[cmdNum].VAL_TYPE, 8);
  memset(&g_cmdInfo[cmdNum].CMD_NAME[strlen(g_cmdInfo[cmdNum].CMD_NAME)], 0x00, 20-strlen(g_cmdInfo[cmdNum].CMD_NAME));
  memcpy(&sendInfo[13], g_cmdInfo[cmdNum].CMD_NAME, 20);
  memcpy(&sendInfo[33], g_cmdInfo[cmdNum].CMD_INFO, strlen(g_cmdInfo[cmdNum].CMD_INFO));
  Ano_SendData(ID_CMD_COMMAND_INFO_FRAME,(const uint8_t *)sendInfo, 33+strlen(g_cmdInfo[cmdNum].CMD_INFO));
}

void Ano_ParseFunctionCMD(uint8_t *toParseData)
{
  uint8_t parCmd = toParseData[0];
  uint16_t parVal = ((uint16_t)toParseData[2] << 8)|toParseData[1];
  switch (parCmd){
    case 0x00: // 读取命令个数
      Ano_SendCmdCount();
      Ano_LogDebug((const char *)"send cmd count");
      break;
    case 0x01:  // 读取命令列表内容
      Ano_SendCmdInfo(parVal);
      Ano_LogDebug((const char *)"send cmd info");
      break;
    default:
      Ano_LogErr((const char *)"unkondw cmd");
      break;
  }
}