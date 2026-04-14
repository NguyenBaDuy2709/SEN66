/*
 * Sen66.h
 *
 * Created on: Mar 27, 2026
 * Author: NguyenBaDuy
 */

#ifndef INC_SEN66_H_
#define INC_SEN66_H_

#include "stdio.h"
#include "stdint.h"
#include "main.h"

#define SEN66_ADDR 0x6B


#define Measure_continuous 0x0021 // delay 50ms
#define Stop_measure 0x0104 // delay 1400ms

#define Get_data_ready 0x0202 // 20ms
#define DEVICE_STATUS 0xD206 // 20ms
#define DEVICE_STATUS_CLEAR 0xD210

#define Read_Measure_Value 0x0300 // 27 byte -  2,5,8,11,14,17,20,23,26(CRC) = 16 byte data

// Lệnh cấu hình / điều khiển khác
#define DEVICE_RST 0xD304 // 1200ms
#define Start_Fan_cleaning 0x5607 // 20ms
// After using this cmd. The fan will be run with max speed in 10s then stop.
// Wait at least 10s after this cmd was used -> Start measuring
#define Active_SHT_Heater 0x6765
#define C02_Factory_RST 0x6754

// Cấu trúc dữ liệu
// Cấu trúc dữ liệu (ĐÃ SỬA SANG FLOAT)
typedef struct {
    float pm1, pm2_5, pm4, pm10;
    float humid, temp, voc, nox;
    uint16_t co2;
} Sensor_data;

// Khai báo hàm
void delay_ms(uint32_t delay);
void Config(void);
uint8_t CRC_cal(uint8_t *data, int len);
uint8_t Scan_Address(void);
void Read_Register(uint8_t slave_id ,uint16_t reg, uint8_t *data, int byte_length, uint32_t ms);
uint32_t See_Status_Of_Device(uint32_t ds);
void Send_command_write(uint8_t slave_id, uint8_t *data, int length);
void Start_Or_Stop_Sensor(uint16_t cmd, uint32_t wait_ms);
uint16_t combine_data(uint8_t data1,uint8_t data2);
void Return_Value_Of_Sensor(Sensor_data *data);

#endif /* INC_SEN66_H_ */
