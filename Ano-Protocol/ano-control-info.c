#include "ano-control-info.h"

void Ano_Send_InertialSensor(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz)
{
  uint8_t *sendData = malloc(13);

  Ano_GetData_fromInt16(ax, &sendData[0]);
  Ano_GetData_fromInt16(ay, &sendData[2]);
  Ano_GetData_fromInt16(az, &sendData[4]);
  Ano_GetData_fromInt16(gx, &sendData[6]);
  Ano_GetData_fromInt16(gy, &sendData[8]);
  Ano_GetData_fromInt16(gz, &sendData[10]);
  sendData[12] = 0x00;

  Ano_SendData(ID_FC_INFO_INERTIAL_SENSOR,(const uint8_t *)sendData, 13);
  free(sendData);
}

void Ano_Send_CompassAndTemperature(int16_t mx, int16_t my, int16_t mz, int16_t tmp)
{
  uint8_t *sendData = malloc(9);

  Ano_GetData_fromInt16(mx, &sendData[0]);
  Ano_GetData_fromInt16(my, &sendData[2]);
  Ano_GetData_fromInt16(mz, &sendData[4]);
  Ano_GetData_fromInt16(tmp, &sendData[6]);
  sendData[8] = 0x00;

  Ano_SendData(ID_FC_INFO_COMPASS_TEMP_SENSOR,(const uint8_t *)sendData, 9);
  free(sendData);
}

void Ano_Send_EulerAngle(int16_t rol, int16_t pit, int16_t yaw)
{
  uint8_t *sendData = malloc(7);

  Ano_GetData_fromInt16(rol, &sendData[0]);
  Ano_GetData_fromInt16(pit, &sendData[2]);
  Ano_GetData_fromInt16(yaw, &sendData[4]);
  sendData[6] = 0x00;

  Ano_SendData(ID_FC_INFO_ATTITUDE_EULER_ANGLE,(const uint8_t *)sendData, 7);
  free(sendData);
}

void Ano_Send_Quaternion(int16_t qv0, int16_t qv1, int16_t qv2, int16_t qv3)
{
   uint8_t *sendData = malloc(9);

  Ano_GetData_fromInt16(qv0, &sendData[0]);
  Ano_GetData_fromInt16(qv1, &sendData[2]);
  Ano_GetData_fromInt16(qv2, &sendData[4]);
  Ano_GetData_fromInt16(qv3, &sendData[6]);

  sendData[8] = 0x00;

  Ano_SendData(ID_FC_INFO_ATTITUDE_QUATERNION,(const uint8_t *)sendData, 9);
  free(sendData);
}

void Ano_Send_Altitude(int32_t alt_bar, int32_t alt_add, int32_t alt_fu)
{
  uint8_t *sendData = malloc(13);

  Ano_GetData_fromInt32(alt_bar, &sendData[0]);
  Ano_GetData_fromInt32(alt_add, &sendData[4]);
  Ano_GetData_fromInt32(alt_fu, &sendData[8]);
  sendData[12] = 0x00;

  Ano_SendData(ID_FC_INFO_ALTITUDE,(const uint8_t *)sendData, 13);
  free(sendData);
}

void Ano_Send_Dist(uint16_t dist_ra, uint16_t dist_rd)
{
  uint8_t *sendData = malloc(29);
  sendData[0] = 0x00;
  Ano_GetData_fromInt32((int32_t)0x98, &sendData[1]);
  Ano_GetData_fromInt32((int32_t)0, &sendData[5]);
  Ano_GetData_fromInt32((int32_t)0, &sendData[9]);
  Ano_GetData_fromInt32((int32_t)0, &sendData[13]);
  Ano_GetData_fromInt32((int32_t)0, &sendData[17]);
  Ano_GetData_fromInt32((int32_t)0, &sendData[21]);

  Ano_GetData_fromInt16((uint16_t)100, &sendData[25]);
  Ano_GetData_fromInt16((uint16_t)120, &sendData[27]);

  Ano_SendData(ID_FC_CONTROL_GENERIC_DISTANCE_SENSOR_DATA,(const uint8_t *)sendData, 29);
  free(sendData);
}