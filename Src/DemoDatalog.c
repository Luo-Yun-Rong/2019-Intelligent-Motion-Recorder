/**
 ******************************************************************************
 * @file    DemoDatalog.c
 * @author  MEMS Software Solutions Team
 * @brief   Handle the data logging to flash memory
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
#include "main.h"
#include "DemoDatalog.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup ACTIVITY_RECOGNITION_WRIST ACTIVITY RECOGNITION WRIST
 * @{
 */

/* Private defines -----------------------------------------------------------*/
#if (defined (USE_STM32F4XX_NUCLEO))
#define FLASH_LESSTHAN2HH            ((uint32_t)0x0805FA5F)

#elif (defined (USE_STM32L1XX_NUCLEO))
#define FLASH_LESSTHAN2HH            ((uint32_t)0x0805FA5F)

#elif (defined (USE_STM32L4XX_NUCLEO))
#define FLASH_LESSTHAN2HH            ((uint32_t)0x080FF25F)

#else
#error Not supported platform
#endif

/* Private types -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
uint32_t Address2F = FLASH_ADDRESS;
DataByte_t DataByte[DATABYTE_LEN];

/* Private function prototypes -----------------------------------------------*/
#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)))
static void ReadIntFlash(uint32_t NStartAddress, uint32_t *ReadBuffer, uint32_t NBytesToRead);

#elif (defined (USE_STM32L4XX_NUCLEO))
static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);
static void ReadIntFlash(uint32_t NStartAddress, uint64_t *ReadBuffer, uint32_t NBytesToRead);

#else
#error Not supported platform
#endif

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Set current data storage address to the first free address in flash memory
 * @param  None
 * @retval Flash memory state (FLASH_FULL/FLASH_READY)
 */
flash_state_t Datalog_SetAddress(void)
{
  uint32_t flash_sector = FLASH_ADDRESS;

  Address2F += Datalog_SearchNextFreeMemoryIndex(&flash_sector);

  if (Address2F > FLASH_LESSTHAN2HH)
  {
    return FLASH_FULL;
  }
  else
  {
    return FLASH_READY;
  }
}

/**
 * @brief  Save the data to memory
 * @param  IndexMax  Index to last data record in buffer
 * @retval 1 in case of success, 0 otherwise
 */
unsigned char Datalog_SaveData2Mem(uint8_t IndexMax)
{
#if (defined (USE_STM32F4XX_NUCLEO))
  uint32_t address = 0x8060000;

#elif (defined (USE_STM32L1XX_NUCLEO))
  uint32_t address = 0x8060000;

#elif (defined (USE_STM32L4XX_NUCLEO))
  uint32_t address = 0x80FF800;

#else
#error Not supported platform
#endif

  unsigned char success = 1;
  uint8_t idx;
  uint8_t nword;

#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)))
  uint32_t *lpdata; /* MISRA C-2012 rule 11.9 only seemingly violated */

#elif (defined (USE_STM32L4XX_NUCLEO))
  uint64_t *lpdata; /* MISRA C-2012 rule 11.9 only seemingly violated */

#else
#error Not supported platform
#endif

  (void)HAL_FLASH_Unlock();

  /* Clear pending flags (if any) */
#if (defined (USE_STM32F4XX_NUCLEO))
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR |
                         FLASH_FLAG_PGSERR);

#elif (defined (USE_STM32L1XX_NUCLEO))
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR |
                         FLASH_FLAG_OPTVERRUSR | FLASH_FLAG_WRPERR);

#elif (defined (USE_STM32L4XX_NUCLEO))
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

#else
#error Not supported platform
#endif

  for (idx = 0; idx < IndexMax; idx++)
  {
#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)))
    lpdata = (uint32_t *)&DataByte[idx]; /* MISRA C-2012 rule 11.3 only seemingly violated */

#elif (defined (USE_STM32L4XX_NUCLEO))
    lpdata = (uint64_t *)&DataByte[idx]; /* MISRA C-2012 rule 11.3 only seemingly violated */

#else
#error Not supported platform
#endif

    if (Address2F < (uint32_t)(address - IndexMax * sizeof(DataByte_t)))
    {
#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)))
      for (nword = 0; nword < (sizeof(DataByte_t) / 4); nword++)
      {
        /* MISRA C-2012 rule 18.4 violation for purpose */
        if (HAL_FLASH_Program(TYPEPROGRAM_WORD, Address2F, *(lpdata + nword)) == HAL_OK)
        {
          Address2F += 4;
        }
        else
        {
          /* Error occurred while writing data in Flash memory */
          Error_Handler();
        }
      }

#elif (defined (USE_STM32L4XX_NUCLEO))
      for (nword = 0; nword < (sizeof(DataByte_t) / 8U); nword++)
      {
        /* MISRA C-2012 rule 18.4 violation for purpose */
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address2F, *(lpdata + nword)) == HAL_OK)
        {
          Address2F += 8U;
        }
        else
        {
          /* Error occurred while writing data in Flash memory */
          Error_Handler();
        }
      }

#else
#error Not supported platform
#endif

    }
    else
    {
      success = 0;
    }
  }

  (void)HAL_FLASH_Lock();
  return success;
}

/**
 * @brief  Reset flash (block 6)
 * @param  None
 * @retval 1 in case of success, 0 otherwise
 */
unsigned char Datalog_FlashErase(void)
{
  /* Reset data in FLASH */
  unsigned char success = 1;

  /* Erase First Flash sector */
  FLASH_EraseInitTypeDef erase_init_struct;
  uint32_t sector_error = 0;

#if (defined (USE_STM32F4XX_NUCLEO))
  erase_init_struct.TypeErase = TYPEERASE_SECTORS;
  erase_init_struct.VoltageRange = VOLTAGE_RANGE_3;
  erase_init_struct.Sector = FLASH_SECTOR;
  erase_init_struct.NbSectors = 1;

#elif (defined (USE_STM32L1XX_NUCLEO))
  erase_init_struct.TypeErase   = FLASH_TYPEERASE_PAGES;
  erase_init_struct.PageAddress = FLASH_ADDRESS;
  erase_init_struct.NbPages     = 512; /* page 1024 .. 1535 */

#elif (defined (USE_STM32L4XX_NUCLEO))
  erase_init_struct.TypeErase   = FLASH_TYPEERASE_PAGES;
  erase_init_struct.Banks       = GetBank(FLASH_ADDRESS);
  erase_init_struct.Page        = GetPage(FLASH_ADDRESS);
  erase_init_struct.NbPages     = 64;

#else
#error Not supported platform
#endif

  /* Unlock the Flash to enable the flash control register access */
  (void)HAL_FLASH_Unlock();

  /* Clear pending flags (if any) */
#if (defined (USE_STM32F4XX_NUCLEO))
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR |
                         FLASH_FLAG_PGSERR);

#elif (defined (USE_STM32L1XX_NUCLEO))
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR |
                         FLASH_FLAG_OPTVERRUSR | FLASH_FLAG_WRPERR);

#elif (defined (USE_STM32L4XX_NUCLEO))
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

#else
#error Not supported platform
#endif

  if (HAL_FLASHEx_Erase(&erase_init_struct, &sector_error) != HAL_OK)
  {
    success = 0;
    Error_Handler();
  }

  /* Lock the Flash to disable the flash control register access */
  (void)HAL_FLASH_Lock();

  Address2F = FLASH_ADDRESS;

  return success;
}

/**
 * @brief  Search the next free Flash memory
 * @param  FlashSectorBaseAddress start address of related Flash sector
 * @retval sector_offset value to add to next Flash address to write
 */
uint32_t Datalog_SearchNextFreeMemoryIndex(uint32_t *FlashSectorBaseAddress)
{
  uint32_t i;

#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)))
  uint32_t tmp_read_buffer = 0x00;

#elif (defined (USE_STM32L4XX_NUCLEO))
  uint64_t tmp_read_buffer = 0x00;

#else
#error Not supported platform
#endif

  uint32_t sector_offset = 0;
  uint32_t delta = 0;

  ReadIntFlash(*FlashSectorBaseAddress + (FLASH_SECTOR_SIZE / 2U), &tmp_read_buffer, 4);

#if (defined (USE_STM32F4XX_NUCLEO))
  if (tmp_read_buffer != 0xFFFFFFFFU)

#elif (defined (USE_STM32L1XX_NUCLEO))
  if (tmp_read_buffer != 0x00000000U)

#elif (defined (USE_STM32L4XX_NUCLEO))
  if (tmp_read_buffer != 0xFFFFFFFFFFFFFFFFU)

#else
#error Not supported platform
#endif

  {
    *FlashSectorBaseAddress = *FlashSectorBaseAddress + (FLASH_SECTOR_SIZE / 2U);
    delta = (FLASH_SECTOR_SIZE >> 1);
  }

  for (i = 0; i < (FLASH_SECTOR_SIZE >> 1); i = i + FLASH_ITEM_SIZE)
  {
    ReadIntFlash(*FlashSectorBaseAddress + i, &tmp_read_buffer, 4);

#if (defined (USE_STM32F4XX_NUCLEO))
    if (tmp_read_buffer == 0xFFFFFFFFU)

#elif (defined (USE_STM32L1XX_NUCLEO))
    if (tmp_read_buffer == 0x00000000U)

#elif (defined (USE_STM32L4XX_NUCLEO))
    if (tmp_read_buffer == 0xFFFFFFFFFFFFFFFFU)

#else
#error Not supported platform
#endif

    {
      sector_offset = i;
      break;
    }
    else
    {
      sector_offset = FLASH_SECTOR_SIZE / 2U;
    }
  }
  return (sector_offset + delta);
}

/**
 * @brief  Read of FLASH
 * @param  NStartAddress FLASH address where start to read
 * @param  NBytesToRead number of bytes to read in  FLASH
 * @retval None
 */
#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)))
void ReadIntFlash(uint32_t NStartAddress, uint32_t *ReadBuffer, uint32_t NBytesToRead)
{
  uint32_t addr = NStartAddress;
  while (addr < NStartAddress + NBytesToRead)
  {
    *(ReadBuffer) = *(__IO uint32_t *)addr;

    addr += 4;
    ReadBuffer += 1; /* MISRA C-2012 rule 18.4 violation for purpose */
  }
}

#elif (defined (USE_STM32L4XX_NUCLEO))
void ReadIntFlash(uint32_t NStartAddress, uint64_t *ReadBuffer, uint32_t NBytesToRead)
{
  uint32_t addr = NStartAddress;
  while (addr < NStartAddress + NBytesToRead)
  {
    *(ReadBuffer) = *(__IO uint64_t *)addr;
    addr += 8U;

    ReadBuffer += 1; /* MISRA C-2012 rule 18.4 violation for purpose */
  }
}

#else
#error Not supported platform
#endif

/**
 * @brief  Read from FLASH and fill the buffer to be sent via USART
 * @param  AddF FLASH address where start to read
 * @param  LenBuf length buffer
 * @retval None
 */
void Datalog_FillBuffer2BSent(uint32_t AddF, uint8_t LenBuf)
{
  uint32_t i;

#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)))
  uint32_t *pdata;

#elif (defined (USE_STM32L4XX_NUCLEO))
  uint64_t *pdata;

#else
#error Not supported platform
#endif

  for (i = 0; i < LenBuf; i++)
  {

#if ((defined (USE_STM32F4XX_NUCLEO)) || (defined (USE_STM32L1XX_NUCLEO)))
    pdata = (uint32_t *)&DataByte[i]; /* MISRA C-2012 rule 11.3 violation for purpose */

#elif (defined (USE_STM32L4XX_NUCLEO))
    pdata = (uint64_t *)&DataByte[i]; /* MISRA C-2012 rule 11.3 violation for purpose */

#else
#error Not supported platform
#endif

    ReadIntFlash(AddF, pdata, sizeof(DataByte_t));
    AddF += sizeof(DataByte_t);
  }
}

#if (defined (USE_STM32L4XX_NUCLEO))
/**
 * @brief  Gets the page of a given address
 * @param  Addr: Address of the FLASH Memory
 * @retval The page of a given address
 */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
 * @brief  Gets the bank of a given address
 * @param  Addr: Address of the FLASH Memory
 * @retval The bank of a given address
 */
static uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank;

  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
  {
    /* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
    /* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }

  return bank;
}
#endif /* USE_STM32L4XX_NUCLEO */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
