/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN Private defines */

#define USE_HAL_API
#define SPINORFLASH_CS_GPIO                   FLASH_CS_GPIO_Port
#define SPINORFLASH_CS_PIN                    FLASH_CS_Pin
#ifdef USE_HAL_API
#define SPINORFLASH_CS_H                      HAL_GPIO_WritePin(SPINORFLASH_CS_GPIO, SPINORFLASH_CS_PIN, GPIO_PIN_SET)
#define SPINORFLASH_CS_L                      HAL_GPIO_WritePin(SPINORFLASH_CS_GPIO, SPINORFLASH_CS_PIN, GPIO_PIN_RESET)
#else
#define SPINORFLASH_CS_H                      SPINORFLASH_CS_GPIO->BSRR = SPINORFLASH_CS_PIN
#define SPINORFLASH_CS_L                      SPINORFLASH_CS_GPIO->BSRR = (uint32_t)SPINORFLASH_CS_PIN << 16u
#endif

/*W25Q64 memory parameters*/

#define SPINORFLASH_SPI                       hspi2

#define MEMORY_FLASH_SIZE                     0x800000 /* 64 MBits => 8MBytes */
#define MEMORY_BLOCK_SIZE                     0x10000   /* 512 sectors of 64KBytes */
#define MEMORY_SECTOR_SIZE                    0x1000    /* 8162 subsectors of 4kBytes */
#define MEMORY_PAGE_SIZE                      0x100     /* 131072 pages of 256 bytes */

#define SPINORFLASH_READ_TIMEOUT              0xFFFF
#define SPINORFLASH_WRITE_TIMEOUT             0xFFFF

#define SPINORFLASH_WIP_BIT                   0x01

#define SPINORFLASH_WRITE_ENABLE              0x06

#define SPINORFLASH_PAGE_PROGRAM              0x02
#define SPINORFLASH_SECTOR_ERASE              0x20
#define SPINORFLASH_BLOCK_ERASE_32K           0x52
#define SPINORFLASH_BLOCK_ERASE_64K           0xD8
#define SPINORFLASH_CHIP_ERASE1               0xC7

#define SPINORFLASH_READ_STATUS_REG1          0x05

/* USER CODE END Private defines */

void MX_SPI2_Init(void);

/* USER CODE BEGIN Prototypes */

uint8_t CSP_SPI_Init(void);

uint8_t CSP_SPI_Erase_Chip(void);

uint8_t CSP_SPI_AutoPollingMemReady(uint32_t timeout);

uint8_t CSP_SPI_Configuration(void);

uint8_t CSP_SPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress);

uint8_t CSP_SPI_WriteMemory(uint8_t* buffer, uint32_t address,uint32_t buffer_size);

uint8_t CSP_SPI_ReadMemory(uint8_t* buffer, uint32_t address,uint32_t buffer_size);

uint8_t CSP_SPI_EnableMemoryMappedMode(void);

uint8_t CSP_SPI_ResetChip(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

