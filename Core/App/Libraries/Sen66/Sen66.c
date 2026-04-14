/*
 * Sen66.c
 *
 *  Created on: Mar 27, 2026
 *      Author: NguyenBaDuy
 */



#include "Sen66.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;
void delay_ms(uint32_t delay){
    HAL_Delay(delay);
}

void Config(void){

}


uint8_t Scan_Address(void){
   // Dùng hàm check địa chỉ siêu gọn của HAL
    for(uint8_t i = 1; i < 128; i++){
        if(HAL_I2C_IsDeviceReady(&hi2c1, (i << 1), 1, 100) == HAL_OK){
            return i;
        }
    }
    return 0; // Không tìm thấy
}

uint8_t CRC_cal(uint8_t *data, int len){
    uint8_t crc = 0xFF;
    for(int i = 0; i < len;i++){
        crc ^= data[i];
        for(int j = 0;j < 8;j++){
            if(crc & 0x80){
                crc = (crc << 1) ^ 0x31;
            }
            else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void Read_Register(uint8_t slave_id ,uint16_t reg, uint8_t *data, int byte_length, uint32_t ms){
    uint8_t reg_buf[2];
    reg_buf[0] = (uint8_t)((reg >> 8) & 0xFF);
    reg_buf[1] = (uint8_t)(reg & 0xFF);

    // 1. Gửi địa chỉ thanh ghi (lệnh) cần đọc xuống cảm biến
    HAL_I2C_Master_Transmit(&hi2c1, (slave_id << 1), reg_buf, 2, 1000);

    // 2. Chờ cảm biến chuẩn bị dữ liệu
    delay_ms(ms);

    // 3. Đọc dữ liệu từ cảm biến lên
    HAL_I2C_Master_Receive(&hi2c1, (slave_id << 1), data, byte_length, 1000);
}

uint32_t See_Status_Of_Device(uint32_t ds){
    uint8_t status[6];
    Read_Register(SEN66_ADDR, DEVICE_STATUS, status, sizeof(status),20);
    return ds = ((uint32_t)status[0] << 24) |
                      ((uint32_t)status[1] << 16) |
                      ((uint32_t)status[3] << 8)  |
                      ((uint32_t)status[4]);
      // Value is 0x00 -> Normal operation
}

void Send_command_write(uint8_t slave_id, uint8_t *data, int length){
    // Viết lệnh gửi dữ liệu cực kỳ an toàn
    HAL_I2C_Master_Transmit(&hi2c1, (slave_id << 1), data, length, 1000);
}

void Start_Or_Stop_Sensor(uint16_t cmd, uint32_t wait_ms){ // Start and stop measuring
    uint8_t frame[2];
    frame[0] = (uint8_t)((cmd >> 8) & 0xFF); // high byte
    frame[1] = (uint8_t)(cmd & 0xFF); // low byte

    Send_command_write(SEN66_ADDR, frame, sizeof(frame));
    delay_ms(wait_ms);
}

uint16_t combine_data(uint8_t data1,uint8_t data2){
    return (uint16_t)((data1 << 8)|(data2));
}

void Return_Value_Of_Sensor(Sensor_data *data){
    uint8_t buff[27]; // 27 byte
    uint8_t response[2];
    uint8_t crc_cal[9], crc_check[9]; // 9byte crc

    Read_Register(SEN66_ADDR, Read_Measure_Value, buff, sizeof(buff), 30);

    for(int i = 0; i < sizeof(crc_cal); i++){
        response[0] = buff[i + (i*2)];
        response[1] = buff[i + (i*2 + 1)];
        crc_cal[i] = CRC_cal(response, sizeof(response));

        crc_check[i] = buff[i + 2*(i+1)];

        if(crc_cal[i] != crc_check[i]){
            // LỖI CRC: Gán giá trị âm hoặc 0 để tầng App/Main dễ nhận biết
            switch(i){
                case 0: data->pm1   = -1.0f; break;
                case 1: data->pm2_5 = -1.0f; break;
                case 2: data->pm4   = -1.0f; break;
                case 3: data->pm10  = -1.0f; break;
                case 4: data->humid = -1.0f; break;
                case 5: data->temp  = -100.0f; break; // Tránh nhầm với nhiệt độ âm thực tế
                case 6: data->voc   = -1.0f; break;
                case 7: data->nox   = -1.0f; break;
                case 8: data->co2   = 0; break;
                default: break;
            };
        }
        else {
            // ĐÚNG CRC: Ép kiểu và chia số thực (thêm .0f ở mẫu số)
            switch(i){
                case 0: data->pm1   = (float)((uint16_t)combine_data(response[0], response[1])) / 10.0f; break;
                case 1: data->pm2_5 = (float)((uint16_t)combine_data(response[0], response[1])) / 10.0f; break;
                case 2: data->pm4   = (float)((uint16_t)combine_data(response[0], response[1])) / 10.0f; break;
                case 3: data->pm10  = (float)((uint16_t)combine_data(response[0], response[1])) / 10.0f; break;

                // Các thông số môi trường (dùng int16_t theo datasheet của Sensirion)
                case 4: data->humid = (float)((int16_t)combine_data(response[0], response[1])) / 100.0f; break;
                case 5: data->temp  = (float)((int16_t)combine_data(response[0], response[1])) / 200.0f; break;
                case 6: data->voc   = (float)((int16_t)combine_data(response[0], response[1])) / 10.0f; break;
                case 7: data->nox   = (float)((int16_t)combine_data(response[0], response[1])) / 10.0f; break;

                // CO2 không có số thập phân
                case 8: data->co2   = (uint16_t)combine_data(response[0], response[1]); break;
                default: break;
            };
        }
    }
}
