/*
 * PZEM 004T.h
 *
 *  Created on: Jun 3, 2025
 *      Author: DELL
 */

#ifndef LIBRARIES_PZEM_004T_PZEM004T_H_
#define LIBRARIES_PZEM_004T_PZEM004T_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t slave_addr;
    uint8_t rx_buffer[32];
    uint8_t rx_flag;
    uint8_t rx_len;
} PZEM_Handle;

typedef struct {
    float voltage;
    float current;
    float power;
    float energy;
    float frequency;
    float power_factor;
    uint16_t alarm;
} PZEM_Data;

void PZEM_Init(PZEM_Handle *pzem, UART_HandleTypeDef *huart, uint8_t addr);
HAL_StatusTypeDef PZEM_ReadAll(PZEM_Handle *pzem);
void PZEM_UART_RxCpltDMA(PZEM_Handle *pzem);  // Gọi trong HAL_UART_RxCpltCallback
void PZEM_ParseData(PZEM_Handle *pzem, PZEM_Data *data);
HAL_StatusTypeDef PZEM_ResetEnergy(PZEM_Handle *pzem);
HAL_StatusTypeDef PZEM_Calibrate(PZEM_Handle *pzem);
HAL_StatusTypeDef PZEM_WriteRegister(PZEM_Handle *pzem, uint16_t reg, uint16_t value);




#endif /* LIBRARIES_PZEM_004T_PZEM004T_H_ */
