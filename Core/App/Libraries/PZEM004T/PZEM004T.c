#include "PZEM004T.h"
#include <string.h>

static uint32_t u32_be(uint8_t *p) {
    return ((uint32_t)p[0] << 24) |
           ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8)  |
           ((uint32_t)p[3]);
}

uint16_t ModRTU_CRC(uint8_t *buf, int len) {
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];
        for (int i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void PZEM_Init(PZEM_Handle *pzem, UART_HandleTypeDef *huart, uint8_t addr) {
    pzem->huart = huart;
    pzem->slave_addr = addr;
    pzem->rx_flag = 0;
    pzem->rx_len = 0;
    memset(pzem->rx_buffer, 0, sizeof(pzem->rx_buffer));
}

HAL_StatusTypeDef PZEM_ReadAll(PZEM_Handle *pzem) {
    uint8_t cmd[8] = {
        pzem->slave_addr, 0x04, 0x00, 0x00, 0x00, 0x0A
    };
    uint16_t crc = ModRTU_CRC(cmd, 6);
    cmd[6] = crc & 0xFF;
    cmd[7] = crc >> 8;

    pzem->rx_flag = 0;
    memset(pzem->rx_buffer, 0, sizeof(pzem->rx_buffer));

    HAL_UART_Transmit(pzem->huart, cmd, 8, 100);
    return HAL_UART_Receive_DMA(pzem->huart, pzem->rx_buffer, 25);  // DMA nhận 25 byte
}

// Gọi trong HAL_UART_RxCpltCallback() của ứng dụng
void PZEM_UART_RxCpltDMA(PZEM_Handle *pzem) {
    pzem->rx_flag = 1;
    pzem->rx_len = 25;
}

void PZEM_ParseData(PZEM_Handle *pzem, PZEM_Data *data) {
    uint8_t *buf = pzem->rx_buffer;

    // CRC
    uint16_t recv_crc = (buf[24] << 8) | buf[23];
    uint16_t calc_crc = ModRTU_CRC(buf, 23);
    if (recv_crc != calc_crc) return;

    // Parse
    data->voltage      = ((buf[3] << 8) | buf[4]) * 0.1f;
    data->current      = u32_be(&buf[5])  * 0.001f;
    data->power        = u32_be(&buf[9])  * 0.1f;
    data->energy       = u32_be(&buf[13]) * 1.0f;
    data->frequency    = ((buf[17] << 8) | buf[18]) * 0.1f;
    data->power_factor = ((buf[19] << 8) | buf[20]) * 0.01f;
    data->alarm        = (buf[21] << 8) | buf[22];
}

HAL_StatusTypeDef PZEM_ResetEnergy(PZEM_Handle *pzem) {
    uint8_t cmd[4] = {
        pzem->slave_addr, 0x42, 0x00, 0x00
    };
    uint16_t crc = ModRTU_CRC(cmd, 2);
    cmd[2] = crc & 0xFF;
    cmd[3] = crc >> 8;

    return HAL_UART_Transmit(pzem->huart, cmd, 4, 100);
}

HAL_StatusTypeDef PZEM_Calibrate(PZEM_Handle *pzem) {
    uint8_t cmd[6] = {
        0xF8, 0x41, 0x37, 0x21, 0x00, 0x00
    };
    uint16_t crc = ModRTU_CRC(cmd, 4);
    cmd[4] = crc & 0xFF;
    cmd[5] = crc >> 8;

    return HAL_UART_Transmit(pzem->huart, cmd, 6, 100);
}

HAL_StatusTypeDef PZEM_WriteRegister(PZEM_Handle *pzem, uint16_t reg, uint16_t value) {
    uint8_t cmd[8];
    cmd[0] = pzem->slave_addr;
    cmd[1] = 0x06;
    cmd[2] = reg >> 8;
    cmd[3] = reg & 0xFF;
    cmd[4] = value >> 8;
    cmd[5] = value & 0xFF;
    uint16_t crc = ModRTU_CRC(cmd, 6);
    cmd[6] = crc & 0xFF;
    cmd[7] = crc >> 8;

    return HAL_UART_Transmit(pzem->huart, cmd, 8, 100);
}
