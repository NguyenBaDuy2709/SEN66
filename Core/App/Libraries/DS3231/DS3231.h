/*
 * DS3231.h
 *
 *  Created on: Apr 8, 2024
 *      Author: ADMIN
 */

#ifndef DS3231_H_
#define DS3231_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

typedef struct {
	int Day;
	int Date;
	int Month;
	int Year;
	int Hour;
	int Minute;
	int Second;
} DS3231_data_t;

uint8_t Decimal2BCD(uint8_t num);
uint8_t BCD2Decimal(uint8_t num);
HAL_StatusTypeDef DS3231_write_time(DS3231_data_t *dt);
HAL_StatusTypeDef DS3231_read_time(DS3231_data_t *dt);
HAL_StatusTypeDef set_currently_timer_ds3231(DS3231_data_t *st);
// uint8_t rtc_read_temp();
HAL_StatusTypeDef rtc_init(I2C_HandleTypeDef *i2c);

#ifdef __cplusplus
}
#endif
#endif /* DS3231_H_ */
