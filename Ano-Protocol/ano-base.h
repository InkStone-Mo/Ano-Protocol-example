#ifndef _ANO_BASE_H_
#define _ANO_BASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>

/* define */

#define Little(rawData) (rawData & 0xFF) //int数据小端前部
#define Big(rawData) (rawData >> 8) //int数据小端后部

#define MAX_INFO_LENGTH 200

// ACK NULL
#define ID_ACK 0x00
// 飞控信息相关
#define ID_FC_INFO_INERTIAL_SENSOR 0x01
#define ID_FC_INFO_COMPASS_TEMP_SENSOR 0x02
#define ID_FC_INFO_ATTITUDE_EULER_ANGLE 0x03
#define ID_FC_INFO_ATTITUDE_QUATERNION 0x04
#define ID_FC_INFO_ALTITUDE 0x05
#define ID_FC_INFO_FLIGHT_MODE 0x06
#define ID_FC_INFO_FLIGHT_SPEED 0x07
#define ID_FC_INFO_POSITION_OFFSET 0x08
#define ID_FC_INFO_WIND_SPEED_ESTIMATION 0x09
#define ID_FC_INFO_TARGET_ATTITUDE 0x0A
#define ID_FC_INFO_TARGET_SPEED 0x0B
#define ID_FC_INFO_RETURN_HOME_INFO 0x0C
#define ID_FC_INFO_VOLTAGE_CURRENT 0x0D
#define ID_FC_INFO_EXTERNAL_MODULE_STATUS 0x0E
#define ID_FC_INFO_RGB_BRIGHTNESS_OUTPUT 0x0F

// 飞控控制量
#define ID_FC_CONTROL_PWM_CONTROL 0x20
#define ID_FC_CONTROL_ATTITUDE_CONTROL 0x21
#define ID_FC_CONTROL_GPS_SENSOR_INFO_1 0x30
#define ID_FC_CONTROL_RAW_OPTICAL_FLOW_INFO 0x31
#define ID_FC_CONTROL_GENERIC_POSITION_SENSOR 0x32
#define ID_FC_CONTROL_GENERIC_VELOCITY_SENSOR 0x33
#define ID_FC_CONTROL_GENERIC_DISTANCE_SENSOR_DATA 0x34
#define ID_FC_CONTROL_GENERIC_IMAGE_FEATURE_POINTS_FRAME 0x35

// 飞控接收控制
#define ID_FC_RECEIVE_REMOTE_CONTROLLER_DATA 0x40
#define ID_FC_RECEIVE_REAL_TIME_CONTROL_FRAME 0x41

// 光流信息
#define ID_OPTICAL_FLOW_ANONYMOUS_DATA 0x51

// GPS 航点读写
#define ID_GPS_WAYPOINT_READ 0x60
#define ID_GPS_WAYPOINT_READ_WRITE 0x61

// 命令功能
#define ID_CMD_COMMAND_FRAME 0xC0
#define ID_CMD_FUNCTION_FRAME 0xC1
#define ID_CMD_COMMAND_INFO_FRAME 0xC2

// 参数读写
#define ID_PARAMETER_COMMAND 0xE0
#define ID_PARAMETER_VALUE_RW 0xE1
#define ID_PARAMETER_INFO 0xE2
#define ID_DEVICE_INFO 0xE3
// 参数命令
#define PAR_CMD_READ_DEV 0x00  // 读取设备信息
#define PAR_CMD_PAR_NUMBER 0x01 //读取参数个数
#define PAR_CMD_PAR_VAL 0x02
#define PAR_CMD_PAR_INFO 0x03
#define PAR_CMD_PAR_CTRL 0x10
#define PAR_VAL_ALL_RESET 0xAA
#define PAR_VAL_SAVE_PAR 0xAB
#define PAR_VAL_CLEAN_ACC 0x20
#define PAR_VAL_AFTER_ACC 0x21
#define PAR_VAL_CLEAN_COMPASS 0x30
#define PAR_VAL_AFTER_COMPASS 0x31

// 固件升级
#define ID_UPGRADE 0xF0

// 其它
#define ID_LOG_STRING_OUTPUT 0xA0
#define ID_LOG_STRING_NUMBER_OUTPUT 0xA1
#define ID_IMAGE_DATA 0xB0
#define ID_IP_NETWORK_DATA_FORMAT_1 0xB1
#define ID_IP_NETWORK_DATA_FORMAT_2 0xB2
#define ID_SPECIAL_DATA 0xFB

// 灵活帧（VERSATILE frames）
#define ID_VERSATILE_FRAME_1 0xF1
#define ID_VERSATILE_FRAME_2 0xF2
#define ID_VERSATILE_FRAME_3 0xF3
#define ID_VERSATILE_FRAME_4 0xF4
#define ID_VERSATILE_FRAME_5 0xF5
#define ID_VERSATILE_FRAME_6 0xF6
#define ID_VERSATILE_FRAME_7 0xF7
#define ID_VERSATILE_FRAME_8 0xF8
#define ID_VERSATILE_FRAME_9 0xF9
#define ID_VERSATILE_FRAME_10 0xFA

/* end define */

enum{
  CMD_VAL_TYPE_NOT = 0,
  CMD_VAL_TYPE_UINT8,
  CMD_VAL_TYPE_INT8,
  CMD_VAL_TYPE_UINT16,
  CMD_VAL_TYPE_INT16,
  CMD_VAL_TYPE_UINT32,
  CMD_VAL_TYPE_INT32,
  CMD_VAL_TYPE_FLOAT
};

enum{
  PAR_VAL_TYPE_UINT8 = 0,
  PAR_VAL_TYPE_INT8 ,
  PAR_VAL_TYPE_UINT16 ,
  PAR_VAL_TYPE_INT16,
  PAR_VAL_TYPE_UINT32 ,
  PAR_VAL_TYPE_INT32 ,
  PAR_VAL_TYPE_UINT64 ,
  PAR_VAL_TYPE_INT64 ,
  PAR_VAL_TYPE_FLOAT ,
  PAR_VAL_TYPE_DOUBLE,
  PAR_VAL_TYPE_STRING
};

typedef struct {
  uint16_t PAR_ID;
  uint8_t PAR_TYPE;
  char PAR_NAME[20];
  void *PAR_VAL;
  const char *PAR_INFO; // 注意：这里使用指针表示可变长度的参数介绍
} T_ParameterInfo;

typedef struct {
  uint16_t CMD_ID;
  uint8_t CMD[3];
  uint8_t VAL_TYPE[8];
  char CMD_NAME[20];
  const char *CMD_INFO; // 注意：这里使用指针表示可变长度的参数介绍
  void *VAL[8];
} T_CMDInfo;

typedef struct {
  uint32_t BinLen;   // 固件总长度
  uint16_t BinFLen;  // 每帧数据内固件长度
  uint16_t BinCrc;   // 固件 CRC 校验数据
  uint16_t BinVer;   // 固件软件版本（非必须，默认=0）
  uint8_t HwTyp;    // 固件适用硬件种类（非必须，默认=0）
  uint16_t HwVerMin;  // 固件适用最小硬件版本（非必须，默认=0）
  uint16_t HwVerMax;  // 固件适用最大硬件版本（非必须，默认=0）
  uint32_t Cmd;    // 附加命令（非必须，默认=0）
}T_FirmwareInfo;


typedef struct {
  uint8_t DEV_ID;
  int16_t HW_VER;
  int16_t SW_VER;
  int16_t BL_VER;
  int16_t PT_VER;
  char DEVNAME[20];
}T_DevInfo;

/** 
 * @brief 初始化ANO协议处理 
 * 
 * 设置ANO协议的传输通道回调函数，用于发送数据。 
 * 
 * @param transmissionChannel 传输通道回调函数，用于发送数据 
 */ 
void Ano_Init(void (* transmissionChannel)(const uint8_t *, uint16_t)); 

/**
 * @brief 从int16数据中解出大小端
 * @param input_data 输入的uint16类型数据
 * @param data 存放地址
 */
void Ano_GetData_fromInt16( uint16_t input_data, uint8_t *data);

/**
 * @brief 从int32数据中解出大小端
 * @param input_data 输入的int32类型数据
 * @param data 存放地址
 */
void Ano_GetData_fromInt32( uint32_t input_data, uint8_t *data);

/** 
 * @brief 发送ANO协议数据 
 * 
 * 发送带有指定命令和数据的数据包。 
 * 
 * @param cmd 数据包的命令标识 
 * @param data 数据包的二进制数据 
 * @param dataLenght 数据包数据的长度 
 */ 
void Ano_SendData(char cmd, const uint8_t *data, int dataLenght); 
 
/** 
 * @brief 发送ANO协议整数数据 
 * 
 * 发送带有指定命令和整数值的数据包。 
 * 
 * @param cmd 数据包的命令标识 
 * @param data 要发送的整数值 
 */ 
void Ano_SendIntData(char cmd, int data); 
 
/** 
 * @brief 记录ANO协议的错误日志 
 * 
 * 使用格式化字符串记录ANO协议的错误信息。 
 * 
 * @param fmt 格式化字符串 
 * @param ... 格式化字符串对应的参数列表 
 */ 
void Ano_LogErr(const char *fmt, ...); 
 
/** 
 * @brief 记录ANO协议的调试日志 
 * 
 * 使用格式化字符串记录ANO协议的调试信息。 
 * 
 * @param fmt 格式化字符串 
 * @param ... 格式化字符串对应的参数列表 
 */ 
void Ano_LogDebug(const char *fmt, ...); 
 
/** 
 * @brief 记录ANO协议的通用日志 
 * 
 * 使用格式化字符串记录ANO协议的通用信息。 
 * 
 * @param fmt 格式化字符串 
 * @param ... 格式化字符串对应的参数列表 
 */ 
void Ano_Log(const char *fmt, ...); 
 
/** 
 * @brief 处理ANO协议帧数据 
 * 
 * 解析并处理接收到的ANO协议帧数据。 
 * 
 * @param frameData 帧数据的缓冲区 
 * @param frameDataLen 帧数据的长度 
 */ 
void Ano_ProcessFrame(uint8_t *frameData, uint16_t frameDataLen); 
 
/** 
 * @brief 设置ANO协议的设备信息 
 * 
 * 设置ANO协议所需的设备信息结构体。 
 * 
 * @param devInfo 设备信息结构体指针 
 */ 
void Ano_SetDevInfo(T_DevInfo *devInfo); 
 
/** 
 * @brief 发送ANO协议的设备信息 
 * 
 * 将之前设置的设备信息发送出去。 
 */ 
void Ano_SendDevInfo(void);

#endif //_ANO_BASE_H_
