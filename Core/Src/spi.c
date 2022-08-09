/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi2;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB6     ------> SPI2_MISO
    PB7     ------> SPI2_MOSI
    PB8     ------> SPI2_SCK
    */
    GPIO_InitStruct.Pin = FLASH_SO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_SPI2;
    HAL_GPIO_Init(FLASH_SO_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = FLASH_SI_Pin|FLASH_SCK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB6     ------> SPI2_MISO
    PB7     ------> SPI2_MOSI
    PB8     ------> SPI2_SCK
    */
    HAL_GPIO_DeInit(GPIOB, FLASH_SO_Pin|FLASH_SI_Pin|FLASH_SCK_Pin);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#include "gpio.h"

static uint8_t CSP_SPI_WriteEnable(void);

uint8_t CSP_SPI_Init(void) {

    hspi2.Instance = SPI2;
    if(HAL_SPI_DeInit(&hspi2) != HAL_OK)
    {
        return HAL_ERROR;
    }

    MX_SPI2_Init();

    return HAL_OK;
}


uint8_t CSP_SPI_Erase_Chip(void) {

    uint8_t ret = HAL_OK;

    uint8_t cmd = SPINORFLASH_CHIP_ERASE1;

    CSP_SPI_WriteEnable();

    SPINORFLASH_CS_L;
    ret += HAL_SPI_Transmit(&SPINORFLASH_SPI, &cmd, 1, SPINORFLASH_WRITE_TIMEOUT);
    SPINORFLASH_CS_H;

    CSP_SPI_AutoPollingMemReady(SPINORFLASH_WRITE_TIMEOUT);

    return ret;
}

uint8_t CSP_SPI_AutoPollingMemReady(uint32_t timeout) {

    uint8_t ret = HAL_OK;

    uint8_t cmd = SPINORFLASH_READ_STATUS_REG1;
    uint8_t status = 0x00;

    do
    {
        SPINORFLASH_CS_L;
        ret += HAL_SPI_Transmit(&SPINORFLASH_SPI, &cmd, 1, SPINORFLASH_WRITE_TIMEOUT);
        ret += HAL_SPI_Receive(&SPINORFLASH_SPI, &status, 1, SPINORFLASH_READ_TIMEOUT);
        SPINORFLASH_CS_H;

        timeout--;
    }while((status & SPINORFLASH_WIP_BIT) && timeout);

    return ret;
}

static uint8_t CSP_SPI_WriteEnable(void) {

    uint8_t ret = HAL_OK;

    uint8_t cmd = SPINORFLASH_WRITE_ENABLE;
    SPINORFLASH_CS_L;
    ret = HAL_SPI_Transmit(&SPINORFLASH_SPI, &cmd, 1, SPINORFLASH_WRITE_TIMEOUT);
    SPINORFLASH_CS_H;

    return ret;
}

uint8_t CSP_SPI_Configuration(void) {

    return HAL_OK;
}

uint8_t CSP_SPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress) {

    uint8_t ret = HAL_OK;

    if (EraseStartAddress % SPINORFLASH_SECTOR_MEMORY_BYTES)
    {
        EraseStartAddress = ((EraseStartAddress / SPINORFLASH_SECTOR_MEMORY_BYTES)) * SPINORFLASH_SECTOR_MEMORY_BYTES;
        if (EraseStartAddress > SPINORFLASH_SECTOR_ADDRESS_MAX)
        {
            return HAL_ERROR;
        }
    }

    uint8_t w_addr[3] = {0};
    uint8_t cmd = SPINORFLASH_SECTOR_ERASE;

    do
    {
        w_addr[0] = (EraseStartAddress & 0xff0000) >> 16;
        w_addr[1] = (EraseStartAddress & 0x00ff00) >> 8;
        w_addr[2] = EraseStartAddress & 0x0000ff;

        CSP_SPI_WriteEnable();

        SPINORFLASH_CS_L;
        ret += HAL_SPI_Transmit(&SPINORFLASH_SPI, &cmd, 1, SPINORFLASH_WRITE_TIMEOUT);
        ret += HAL_SPI_Transmit(&SPINORFLASH_SPI, &w_addr[0], 3, SPINORFLASH_WRITE_TIMEOUT);
        SPINORFLASH_CS_H;

        CSP_SPI_AutoPollingMemReady(SPINORFLASH_WRITE_TIMEOUT);

        EraseStartAddress += SPINORFLASH_SECTOR_MEMORY_BYTES;
    }while(EraseStartAddress <= EraseEndAddress); //use <= here


    return ret;
}

uint8_t CSP_SPI_WriteMemory(uint8_t* buffer, uint32_t address,uint32_t buffer_size) {

    uint8_t ret = HAL_OK;

    return ret;
}

uint8_t CSP_SPI_ReadMemory(uint8_t* buffer, uint32_t address,uint32_t buffer_size) {

    uint8_t ret = HAL_OK;

    return ret;
}

uint8_t CSP_SPI_EnableMemoryMappedMode(void) {

    return HAL_OK;
}

uint8_t CSP_SPI_ResetChip(void) {

    return HAL_OK;
}
/* USER CODE END 1 */
