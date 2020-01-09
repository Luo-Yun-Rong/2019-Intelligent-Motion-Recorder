/**
 *******************************************************************************
 * @file    DemoDatalog.h
 * @author  MEMS Software Solutions Team
 * @brief   Header for DemoDatalog.c.
 *******************************************************************************
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEMO_DATALOG_H
#define DEMO_DATALOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Public types --------------------------------------------------------------*/
typedef struct
{
  uint8_t date[3];
  uint8_t time[3];
} DateTime_t;

typedef struct
{
  DateTime_t date_time;
  uint8_t data_valid; /* 0 if new session */
  uint8_t activity_type;
} DataByte_t;

/* Private defines -----------------------------------------------------------*/
#define DATABYTE_LEN                  ((uint8_t)20)

#if (defined (USE_STM32F4XX_NUCLEO))
#define FLASH_SECTOR                  FLASH_SECTOR_6
#define FLASH_SECTOR_SIZE             (SIZE_FLASH_SECTOR_6)
#define SIZE_FLASH_SECTOR_6           ((uint32_t)0x00020000)

#elif (defined (USE_STM32L1XX_NUCLEO))
#define FLASH_SECTOR_SIZE             ((uint32_t)0x00020000) /* Size of sector 64 .. 95 */

#elif (defined (USE_STM32L4XX_NUCLEO))
#define FLASH_SECTOR_SIZE             ((uint32_t)0x00020000)

#else
#error Not supported platform
#endif

#define FLASH_ITEM_SIZE               8U

/* Exported defines ----------------------------------------------------------*/
#if (defined (USE_STM32F4XX_NUCLEO))
#define FLASH_ADDRESS                 ((uint32_t)0x08040000)

#elif (defined (USE_STM32L1XX_NUCLEO))
#define FLASH_ADDRESS                 ((uint32_t)0x08040000) /* page 1024 */

#elif (defined (USE_STM32L4XX_NUCLEO))
#define FLASH_ADDRESS                 ((uint32_t)0x080DF800) /* page 447 */

#else
#error Not supported platform
#endif

/* Public variables ----------------------------------------------------------*/
extern uint32_t Address2F;
extern DataByte_t DataByte[];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
flash_state_t Datalog_SetAddress(void);
unsigned char Datalog_FlashErase(void);
unsigned char Datalog_SaveData2Mem(uint8_t IndexMax);
uint32_t Datalog_SearchNextFreeMemoryIndex(uint32_t *FlashSectorBaseAddress);
void Datalog_FillBuffer2BSent(uint32_t AddF, uint8_t LenBuf);

#ifdef __cplusplus
}
#endif

#endif /* DEMO_DATALOG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

