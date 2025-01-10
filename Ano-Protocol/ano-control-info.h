#ifndef _ANO_CONTROL_INFO_H_
#define _ANO_CONTROL_INFO_H_

#include "ano-base.h"

/** 
 * @brief 发送惯性传感器数据 
 * 
 * 将惯性传感器（加速度计和陀螺仪）的数据发送到某个协议。 
 * 
 * @param ax 加速度计X轴数据 
 * @param ay 加速度计Y轴数据 
 * @param az 加速度计Z轴数据 
 * @param gx 陀螺仪X轴数据 
 * @param gy 陀螺仪Y轴数据 
 * @param gz 陀螺仪Z轴数据 
 */ 
void Ano_Send_InertialSensor(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz); 
 
/** 
 * @brief 发送罗盘和温度数据 
 * 
 * 将罗盘（磁力计）和温度数据发送到某个协议。 
 * 
 * @param mx 罗盘X轴数据 
 * @param my 罗盘Y轴数据 
 * @param mz 罗盘Z轴数据 
 * @param tmp 温度数据 
 */ 
void Ano_Send_CompassAndTemperature(int16_t mx, int16_t my, int16_t mz, int16_t tmp); 
 
/** 
 * @brief 发送欧拉角数据 
 * 
 * 将欧拉角（翻滚角、俯仰角和偏航角）数据发送到某个协议。 
 * 
 * @param rol 翻滚角数据 
 * @param pit 俯仰角数据 
 * @param yaw 偏航角数据 
 */ 
void Ano_Send_EulerAngle(int16_t rol, int16_t pit, int16_t yaw); 
 
/** 
 * @brief 发送四元数数据 
 * 
 * 将四元数数据发送到某个协议。 
 * 
 * @param qv0 四元数第一个分量 
 * @param qv1 四元数第二个分量 
 * @param qv2 四元数第三个分量 
 * @param qv3 四元数第四个分量 
 */ 
void Ano_Send_Quaternion(int16_t qv0, int16_t qv1, int16_t qv2, int16_t qv3); 
 
/** 
 * @brief 发送高度数据 
 * 
 * 将高度数据（气压高度、附加高度和融合高度）发送到某个协议。 
 * 
 * @param alt_bar 气压高度数据 
 * @param alt_add 附加高度数据 
 * @param alt_fu 融合高度数据 
 */ 
void Ano_Send_Altitude(int32_t alt_bar, int32_t alt_add, int32_t alt_fu);


void Ano_Send_Dist(uint16_t dist_ra, uint16_t dist_rd);

#endif //_ANO_CONTROL_INFO_H_