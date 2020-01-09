/**
 ******************************************************************************
 * @file    DemoSerial.c
 * @author  MEMS Software Solutions Team
 * @brief   Handle the Serial Protocol
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed under Software License Agreement
 * SLA0077, (the "License"). You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *     www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1
 *
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "com.h"
#include "DemoDatalog.h"
#include "DemoSerial.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup ACTIVITY_RECOGNITION_WRIST ACTIVITY RECOGNITION WRIST
 * @{
 */

/* Private defines -----------------------------------------------------------*/
#define LPS25HB_UNICLEO_ID_ONBOARD 1
#define LPS25HB_UNICLEO_ID_DIL 2
#define LPS22HB_UNICLEO_ID 3
#define HTS221_UNICLEO_ID 1
#define LSM6DS0_UNICLEO_ID 1
#define LSM6DS3_UNICLEO_ID 2
#define LSM6DSL_UNICLEO_ID 3
#define LIS3MDL_UNICLEO_ID 1
#define LSM303AGR_UNICLEO_ID_MAG 2

#define FW_ID "8"
#define FW_VERSION "2.0.0"
#define LIB_VERSION "2.0.2"

#if (defined (USE_IKS01A2))
#define EXPANSION_BOARD "IKS01A2"
#else
#error Not supported shield
#endif

#define DATA_TX_LEN  MIN(4, DATABYTE_LEN)

/* Private variables ---------------------------------------------------------*/
static uint8_t PresentationString[] = {"MEMS shield demo,"FW_ID","FW_VERSION","LIB_VERSION","EXPANSION_BOARD};
static volatile uint8_t DataStreamingDest = 2;

/**
 * @brief  Build the reply header
 * @param  Msg the pointer to the message to be built
 * @retval None
 */
void BUILD_REPLY_HEADER(TMsg *Msg)
{
  Msg->Data[0] = Msg->Data[1];
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] += CMD_Reply_Add;
}

/**
 * @brief  Initialize the streaming header
 * @param  Msg the pointer to the header to be initialized
 * @retval None
 */
void INIT_STREAMING_HEADER(TMsg *Msg)
{
  Msg->Data[0] = DataStreamingDest;
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_Start_Data_Streaming;
  Msg->Len = 3;
}

/**
 * @brief  Initialize the streaming message
 * @param  Msg the pointer to the message to be initialized
 * @retval None
 */
void INIT_STREAMING_MSG(TMsg *Msg)
{
  uint32_t i;

  Msg->Data[0] = DataStreamingDest;
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_Start_Data_Streaming;
  for (i = 3; i < STREAMING_MSG_LENGTH + 3; i++)
  {
    Msg->Data[i] = 0;
  }
  Msg->Len = 3;
}

/**
 * @brief  Handle a message
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
int HandleMSG(TMsg *Msg)
/*  DestAddr | SouceAddr | CMD | PAYLOAD
 *      1          1        1       N
 */
{
  int ret = 1;
  uint32_t i;
  uint32_t addf;
  uint32_t countb = 0;

  if (Msg->Len < 2U)
  {
    return 0;
  }
  if (Msg->Data[0] != DEV_ADDR)
  {
    return 0;
  }
  switch (Msg->Data[2])   /* CMD */
  {
    case CMD_Ping:
      if (Msg->Len != 3U)
      {
        return 0;
      }
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      UART_SendMsg(Msg);
      break;

    case CMD_Enter_DFU_Mode:
      if (Msg->Len != 3U)
      {
        return 0;
      }
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      break;

    case CMD_Read_PresString:
      if (Msg->Len != 3U)
      {
        return 0;
      }
      BUILD_REPLY_HEADER(Msg);
      i = 0;
      while (i < (sizeof(PresentationString) - 1U))
      {
        Msg->Data[3U + i] = PresentationString[i];
        i++;
      }
      Msg->Len = 3U + i;
      UART_SendMsg(Msg);
      break;

    case CMD_PRESSURE_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }
      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], LPS22HB_UNICLEO_ID, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_HUMIDITY_TEMPERATURE_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }
      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], HTS221_UNICLEO_ID, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_ACCELERO_GYRO_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }
      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], LSM6DSL_UNICLEO_ID, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_MAGNETO_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }
      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], LSM303AGR_UNICLEO_ID_MAG, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_Start_Data_Streaming:
      if (Msg->Len < 3U)
      {
        return 0;
      }
      SensorsEnabled = Deserialize(&Msg->Data[3], 4);

      /* Start enabled sensors */
      if ((SensorsEnabled & PRESSURE_SENSOR) == PRESSURE_SENSOR)
      {
        (void)IKS01A2_ENV_SENSOR_Enable(IKS01A2_LPS22HB_0, ENV_PRESSURE);
      }
      if ((SensorsEnabled & TEMPERATURE_SENSOR) == TEMPERATURE_SENSOR)
      {
        (void)IKS01A2_ENV_SENSOR_Enable(IKS01A2_HTS221_0, ENV_TEMPERATURE);
      }
      if ((SensorsEnabled & HUMIDITY_SENSOR) == HUMIDITY_SENSOR)
      {
        (void)IKS01A2_ENV_SENSOR_Enable(IKS01A2_HTS221_0, ENV_HUMIDITY);
      }
      if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
      {
        (void)IKS01A2_MOTION_SENSOR_Enable(IKS01A2_LSM6DSL_0, MOTION_ACCELERO);
      }
      if ((SensorsEnabled & GYROSCOPE_SENSOR) == GYROSCOPE_SENSOR)
      {
        (void)IKS01A2_MOTION_SENSOR_Enable(IKS01A2_LSM6DSL_0, MOTION_GYRO);
      }
      if ((SensorsEnabled & MAGNETIC_SENSOR) == MAGNETIC_SENSOR)
      {
        (void)IKS01A2_MOTION_SENSOR_Enable(IKS01A2_LSM303AGR_MAG_0, MOTION_MAGNETO);
      }

      (void)HAL_TIM_Base_Start_IT(&AlgoTimHandle);
      DataLoggerActive = 1;

      DataStreamingDest = Msg->Data[1];
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      UART_SendMsg(Msg);
      break;

    case CMD_Stop_Data_Streaming:
      if (Msg->Len < 3U)
      {
        return 0;
      }
      DataLoggerActive = 0;
      (void)HAL_TIM_Base_Stop_IT(&AlgoTimHandle);

      /* Disable all sensors */
      (void)IKS01A2_ENV_SENSOR_Disable(IKS01A2_LPS22HB_0, ENV_PRESSURE);
      (void)IKS01A2_ENV_SENSOR_Disable(IKS01A2_HTS221_0, ENV_TEMPERATURE);
      (void)IKS01A2_ENV_SENSOR_Disable(IKS01A2_HTS221_0, ENV_HUMIDITY);
      (void)IKS01A2_MOTION_SENSOR_Disable(IKS01A2_LSM6DSL_0, MOTION_ACCELERO);
      (void)IKS01A2_MOTION_SENSOR_Disable(IKS01A2_LSM6DSL_0, MOTION_GYRO);
      (void)IKS01A2_MOTION_SENSOR_Disable(IKS01A2_LSM303AGR_MAG_0, MOTION_MAGNETO);

      SensorsEnabled = 0;

      BUILD_REPLY_HEADER(Msg);
      UART_SendMsg(Msg);
      break;

    case CMD_Set_DateTime:
      if (Msg->Len < 3U)
      {
        return 0;
      }
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      RTC_TimeRegulate(Msg->Data[3], Msg->Data[4], Msg->Data[5]);
      RTC_DateRegulate(Msg->Data[6], Msg->Data[7], Msg->Data[8], Msg->Data[9]);
      UART_SendMsg(Msg);
      break;

    case CMD_UploadXX:
      if (Msg->Len < 3U)
      {
        return 0;
      }
      addf = (Address2F - FLASH_ADDRESS);
      Msg->Len = 4;
      Msg->Data[0] = (uint8_t)((addf >> 24) & 0xFFU);
      Msg->Data[1] = (uint8_t)((addf >> 16) & 0xFFU);
      Msg->Data[2] = (uint8_t)((addf >>  8) & 0xFFU);
      Msg->Data[3] = (uint8_t)((addf) & 0xFFU);
      (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)Msg->Data, 4, 5000);

      /* Read buffer from flash */
      /* MISRA C-2012 rule 10.1r3 only seemingly violated */
      for (addf = FLASH_ADDRESS; addf < Address2F; addf += ((uint32_t)DATA_TX_LEN * sizeof(DataByte_t)))
      {
        Datalog_FillBuffer2BSent(addf, DATA_TX_LEN);
        /* MISRA C-2012 rule 10.1r3 only seemingly violated */
        (void)memcpy(&Msg->Data[0], DataByte, ((uint32_t)DATA_TX_LEN * sizeof(DataByte_t)));
        /* MISRA C-2012 rule 10.1r3 only seemingly violated */
        i = MIN((Address2F - FLASH_ADDRESS - countb), ((uint32_t)DATA_TX_LEN * sizeof(DataByte_t)));
        /* MISRA C-2012 rule 10.1r3 only seemingly violated */
        countb += (uint32_t)DATA_TX_LEN * sizeof(DataByte_t);
        (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataByte, (uint16_t)i, 5000);
        HAL_Delay(10);
      }
      (void)Datalog_FlashErase();
      BSP_LED_Off(LED2);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
