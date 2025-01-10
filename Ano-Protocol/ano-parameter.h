#ifndef _ANO_PARAMETER_H_
#define _ANO_PARAMETER_H_

#include "ano-base.h"

/** 
 * @brief 写入参数值数据 
 * 
 * 将给定的参数值数据写入到某个位置。 
 * 
 * @param parameterData 参数值数据的指针 
 * @param parameterDataLen 参数值数据的长度 
 */ 
void Ano_WriteParameValue(uint8_t *parameterData, uint16_t parameterDataLen); 
 
/** 
 * @brief 设置参数信息 
 * 
 * 根据给定的参数信息结构体和参数数量，设置ANO协议所需的参数信息。 
 * 
 * @param parameterInfo 参数信息结构体指针 
 * @param parameterCount 参数数量 
 */ 
void Ano_SetParameterInfo(T_ParameterInfo *parameterInfo, uint16_t parameterCount); 

/** 
 * @brief 解析参数命令 
 * 
 * 解析传入的数据，以提取和处理ANO协议中的参数命令。 
 * 
 * @param toParseData 要解析的数据指针 
 */ 
void Ano_ParseParCMD(uint8_t *toParseData); 
 
/** 
 * @brief 发送参数值 
 * 
 * 发送指定编号的参数值到ANO协议的目标端。 
 * 
 * @param parameterNum 参数编号 
 */ 
void Ano_SendParameValue(uint16_t parameterNum); 
 
/** 
 * @brief 发送参数数量 
 * 
 * 发送ANO协议中参数的总数量到目标端。 
 */ 
void Ano_SendParameCount(void); 
 
/** 
 * @brief 发送参数信息 
 * 
 * 发送指定编号的参数信息到ANO协议的目标端。 
 * 
 * @param parameterNum 参数编号 
 */ 
void Ano_SendParameterInfo(uint16_t parameterNum);

#endif //_ANO_PARAMETER_H_