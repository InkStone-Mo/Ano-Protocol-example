#ifndef _Ano_Upgrade_H_
#define _Ano_Upgrade_H_

#include "ano-base.h"

typedef struct 
{
  void (* Ano_UpgradeStart)(void); //开始升级
  void (* Ano_UpgradeSave)(uint8_t *upgradeData,uint16_t frameLen, uint16_t frameNumber); //升级帧回调
  void (* Ano_UpgradeEnd)(void); //结束升级
}T_AnoUpgrade;

/** 
 * @brief 升级初始化 
 * 
 * 初始化ANO协议的升级过程，设置升级所需的结构体和其他相关信息。 
 * 
 * @param Ano_Upgrade 指向ANO升级结构体的指针，用于存储升级过程的状态和配置信息 
 */ 
void Ano_UpgradeInit(T_AnoUpgrade *Ano_Upgrade); 
 
/** 
 * @brief 处理升级数据帧 
 * 
 * 处理接收到的升级数据帧，包括解析数据、验证数据完整性以及更新目标设备或系统。 
 * 
 * @param upgradeFrameData 升级数据帧的指针 
 * @param frameDataLen 升级数据帧的长度 
 */ 
void Ano_ProcessUpgrade(uint8_t *upgradeFrameData, uint16_t frameDataLen);

#endif //_Ano_Upgrade_H_
