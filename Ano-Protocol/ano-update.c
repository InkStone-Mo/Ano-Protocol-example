#include "ano-update.h"

static T_AnoUpgrade anoUpdate;

static void (* Ano_Upgrade_Cb)(uint8_t *frameData, uint16_t frameNumber);
static int Ano_ValidateBin(void);
static int Ano_ValidateFirmware(const T_FirmwareInfo *info);
static int Ano_EraseFlash(void);
static void Ano_SaveUpgradeBin(uint8_t *upgradeData);
static T_FirmwareInfo info;
uint32_t firmwareCount = 0;

void Ano_UpgradeInit(T_AnoUpgrade *Ano_Upgrade)
{
  if(Ano_Upgrade->Ano_UpgradeEnd != NULL)
    anoUpdate.Ano_UpgradeEnd = Ano_Upgrade->Ano_UpgradeEnd;
  if(Ano_Upgrade->Ano_UpgradeSave != NULL)
    anoUpdate.Ano_UpgradeSave = Ano_Upgrade->Ano_UpgradeSave;
  if(Ano_Upgrade->Ano_UpgradeStart != NULL)
    anoUpdate.Ano_UpgradeStart = Ano_Upgrade->Ano_UpgradeStart;
}

static int Ano_ValidateBin(void)
{
  uint8_t sendInfo[3] = {'\0'};
  sendInfo[0] = 0x00;
  sendInfo[1] = 0x10;
  sendInfo[2] = 0x12;
  Ano_SendData(ID_UPGRADE, sendInfo, 3);
  return 0;
}

void printFirmwareInfo(const T_FirmwareInfo *info) {
  Ano_LogDebug((const char *)"BinLen: %u\n", info->BinLen);
  Ano_LogDebug((const char *)"BinFLen: %u\n", info->BinFLen);
  Ano_LogDebug((const char *)"BinCrc: %02x\n", info->BinCrc);
  Ano_LogDebug((const char *)"BinVer: %u\n", info->BinVer);
  Ano_LogDebug((const char *)"HwTyp: %u\n", info->HwTyp);
  Ano_LogDebug((const char *)"HwVerMin: %u\n", info->HwVerMin);
  Ano_LogDebug((const char *)"HwVerMax: %u\n", info->HwVerMax);
  Ano_LogDebug((const char *)"Cmd: %u\n", info->Cmd);
}

void Ano_ProcessUpgrade(uint8_t *upgradeFrameData, uint16_t frameDataLen)
{

  // printf("upgrade cmd:%02x \n",upgradeFrameData[0]);
  switch (upgradeFrameData[0]){
    
    case 0x00: //固件命令
      switch (upgradeFrameData[1]){
        case 0x01: // 第一部分命令
          switch (upgradeFrameData[2]){
            case 0x10: //擦除flash
              Ano_LogDebug((const char *)" e flash");
              Ano_EraseFlash();
              anoUpdate.Ano_UpgradeStart();
              break;
            default:
              break;
          }
          break;
        case 0x10: // 第二
          switch (upgradeFrameData[2]){
            case 0x10: // 发送完成
              Ano_ValidateBin(); //升级成功
              anoUpdate.Ano_UpgradeEnd();
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
      break;

    case 0x01: // 升级包信息
      info.BinLen = (uint32_t)upgradeFrameData[4] << 24 | (uint32_t)upgradeFrameData[3] << 16 | (uint32_t)upgradeFrameData[2] << 8 | (uint32_t)upgradeFrameData[1];
      info.BinFLen = (uint16_t)upgradeFrameData[6] << 8 | upgradeFrameData[5];
      info.BinCrc = (uint16_t)upgradeFrameData[8] << 8 | upgradeFrameData[7];
      info.BinVer = (uint16_t)upgradeFrameData[10] << 8 | upgradeFrameData[9];
      info.HwTyp = upgradeFrameData[11];
      info.HwVerMin = (uint16_t)upgradeFrameData[13] << 8 | upgradeFrameData[12];
      info.HwVerMax = (uint16_t)upgradeFrameData[15] << 8 | upgradeFrameData[14];
      info.Cmd = (uint32_t)upgradeFrameData[19] << 24 | (uint32_t)upgradeFrameData[18] << 16 | (uint32_t)upgradeFrameData[17] << 8 | (uint32_t)upgradeFrameData[16];
      printFirmwareInfo(&info);
      Ano_ValidateFirmware(&info);
      firmwareCount = info.BinLen / info.BinFLen;
      Ano_LogDebug("count %d, %d", firmwareCount, info.BinLen % info.BinFLen);
      break;
    case 0x10:
      Ano_SaveUpgradeBin(&upgradeFrameData[1]);
      break;
    default:
      break;
  }
}

static int Ano_ValidateFirmware(const T_FirmwareInfo *info)
{
  uint8_t sendInfo[3] = {'\0'};
  sendInfo[0] = 0x00;
  sendInfo[1] = 0x01;
  sendInfo[2] = 0x02;
  Ano_SendData(ID_UPGRADE, (const uint8_t *)sendInfo, 3);
  return 0;
}

static int Ano_EraseFlash(void)
{
  uint8_t sendInfo[3] = {'\0'};
  sendInfo[0] = 0x00;
  sendInfo[1] = 0x01;
  sendInfo[2] = 0x12;
  Ano_SendData(ID_UPGRADE,(const uint8_t *)sendInfo, 3);
  return 0;
}



void Ano_SaveUpgradeBin(uint8_t *upgradeData)
{
  uint16_t number = ((uint16_t)upgradeData[1]<<8|upgradeData[0]);
  uint8_t sendInfo[5] = {'\0'} ;
  sendInfo[0] = 0x00;
  sendInfo[1] = 0x10;
  sendInfo[2] = 0x02;
  sendInfo[3] = upgradeData[0];
  sendInfo[4] = upgradeData[1];
  Ano_SendData(ID_UPGRADE,(const uint8_t *)sendInfo, 5);
  if(number == firmwareCount){
    if(anoUpdate.Ano_UpgradeSave != NULL) anoUpdate.Ano_UpgradeSave(&upgradeData[2], (uint16_t)info.BinLen %info.BinFLen, number);
  }else{
    if(anoUpdate.Ano_UpgradeSave != NULL) anoUpdate.Ano_UpgradeSave(&upgradeData[2], (uint16_t)info.BinFLen, number);
  }
}