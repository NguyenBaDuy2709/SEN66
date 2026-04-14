/*
 * Relaytime.h
 *
 *  Created on: Jun 3, 2025
 *      Author: DELL
 */

#ifndef LIBRARIES_RELAYTIME_RELAYTIME_H_
#define LIBRARIES_RELAYTIME_RELAYTIME_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t startHour;
    uint8_t startMin;
    uint8_t endHour;
    uint8_t endMin;
} KhungGio_t;

typedef struct {
    uint8_t hour;
    uint8_t min;
} Time_t;

extern Time_t currentTime;

bool KiemTraKhungGio(void);

#endif /* LIBRARIES_RELAYTIME_RELAYTIME_H_ */
