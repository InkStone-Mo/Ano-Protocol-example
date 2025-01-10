#ifndef _ANO_COMMAND_H_
#define _ANO_COMMAND_H_

#include "Ano-Protocol/ano-base.h"

/** 
 * @brief 写入命令信息数据 
 * 
 * 将给定的命令信息数据写入到某个位置（可能是缓冲区或文件等）。 
 * 
 * @param cmdInfoData 命令信息数据的指针 
 * @param cmdInfoDataLen 命令信息数据的长度 
 */ 
void Ano_WriteCmdInfo(const uint8_t *cmdInfoData, uint16_t cmdInfoDataLen); 
 
/** 
 * @brief 设置命令信息 
 * 
 * 根据给定的命令信息结构体和命令数量，设置ANO协议所需的命令信息，并指定写入命令信息的回调函数。 
 * 
 * @param cmdInfo 命令信息结构体指针 
 * @param cmdCount 命令数量 
 * @param writeCmdInfo 写入命令信息的回调函数 
 */ 
void Ano_SetCmdInfo(T_CMDInfo *cmdInfo, uint16_t cmdCount, void (*writeCmdInfo)(const uint8_t *, uint16_t)); 
 
/** 
 * @brief 解析功能命令 
 * 
 * 解析传入的数据，以提取和处理ANO协议中的功能命令。 
 * 
 * @param toParseData 要解析的数据指针 
 */ 
void Ano_ParseFunctionCMD(uint8_t *toParseData);

static void (* Ano_WriteCmdInfo_Cb)(const uint8_t *frameData, uint16_t frameDataLen);

#endif //_ANO_COMMAND_H_