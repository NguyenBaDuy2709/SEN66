/*
 * DS3231.c
 *
 *  Created on: Apr 8, 2024
 *      Author: Bui Duong Quoc Bao
 */
#include "DS3231.h"
#define RTC_ADDR (0x68 << 1)

// Cach 1: Khai Bao Ham "extern I2C_HandleTypeDef hi2c1"
// extern I2C_HandleTypeDef hi2c1;

// Cach 2: Khai Bao Ham "extern I2C_HandleTypeDef hi2c1"

// static: khi khai bao bien static thi chi cac bien trong file nay moi duoc su dung
static I2C_HandleTypeDef *ds_i2c;

HAL_StatusTypeDef rtc_init(I2C_HandleTypeDef *i2c) {
	ds_i2c = i2c;
	return HAL_OK;
}

uint8_t Decimal2BCD(uint8_t num) {
	return (num / 10) << 4 | (num % 10);
}

uint8_t BCD2Decimal(uint8_t num) {
	return (num >> 4) * 10 + (num & 0x0F);
}

// Gán thời gian hiện tại (Year = năm dương lịch đầy đủ)
HAL_StatusTypeDef set_currently_timer_ds3231(DS3231_data_t *st) {
	st->Day = 5;       // Thứ (1=CN, 2=T2, …, 7=T7)
	st->Date = 23;      // Ngày trong tháng
	st->Month = 5;       // Tháng
	st->Year = 2024;    // Năm (full)
	st->Hour = 17;
	st->Minute = 30;
	st->Second = 0;
	return HAL_OK;
}

// Ghi time vào DS3231
HAL_StatusTypeDef DS3231_write_time(DS3231_data_t *dt) {
	uint8_t data_tran[8];

	data_tran[0] = 0x00;                            // start register

	data_tran[1] = Decimal2BCD(dt->Second);         // giây
	data_tran[2] = Decimal2BCD(dt->Minute);         // phút
	data_tran[3] = Decimal2BCD(dt->Hour);           // giờ

	data_tran[4] = Decimal2BCD(dt->Day & 0x07);    // thứ (1–7)
	data_tran[5] = Decimal2BCD(dt->Date & 0x3F);    // ngày (1–31)

	// Tháng + Century bit
	{
		uint8_t m = dt->Month & 0x1F;               // giữ 5 bit thấp cho tháng
		if (dt->Year >= 2000)
			m |= 0x80;            // nếu năm >=2000 thì set bit7
		data_tran[6] = Decimal2BCD(m);
	}

	// Năm (2 chữ số cuối)
	{
		uint8_t yy = dt->Year % 100;                // 2024 → 24
		data_tran[7] = Decimal2BCD(yy);
	}

	// Ghi qua I2C
	if (HAL_I2C_Master_Transmit(ds_i2c, RTC_ADDR, data_tran, sizeof(data_tran),
	HAL_MAX_DELAY) != HAL_OK) {
		// TODO: xử lý lỗi I2C (retry, báo LED, log, ...)
	}
	return HAL_OK;
}

HAL_StatusTypeDef DS3231_read_time(DS3231_data_t *dt) {
	uint8_t data_receive[7];
	uint8_t add_reg = 0x00; // Dia Chi Bat Dau Muon Doc
	HAL_I2C_Master_Transmit(ds_i2c, RTC_ADDR, &add_reg, 1, 100);
	HAL_I2C_Master_Receive(ds_i2c, RTC_ADDR, data_receive, 7, 100);
	dt->Second = BCD2Decimal(data_receive[0]);
	dt->Minute = BCD2Decimal(data_receive[1]);
	dt->Hour = BCD2Decimal(data_receive[2]);
	dt->Day = BCD2Decimal(data_receive[3]);
	dt->Date = BCD2Decimal(data_receive[4]);
	dt->Month = BCD2Decimal(data_receive[5]&0x7F);
	dt->Year = BCD2Decimal(data_receive[6]);
	return HAL_OK;
}

// uint8_t rtc_read_temp()//DateTime *dt
//{
//	uint8_t data_temp[2];
//	uint8_t add_reg = 0x11;
//	HAL_I2C_Master_Transmit(ds_i2c, RTC_ADDR, &add_reg, 1, 100);
//	HAL_I2C_Master_Receive(ds_i2c, RTC_ADDR, data_temp, 2, 100);
////	temp = ((uint16_t)data_temp[0]<<2)|(data_temp[1]>>6);
//
//	return data_temp[0];
//}
//
