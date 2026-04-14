#pragma region Ghi chú
#ifdef GhiChu
/**
 ************************************************************************************************************************
 * @Ten Du An      :  ChuongTrinhChinh.h
 * @Nguoi Thiet Ke :  KS.NGUYEN DINH DAT
 * @Ngay Tao       :  26/09/2025
 ************************************************************************************************************************
 * Copyright (c) 2025 by KS.NGUYENDINHDAT
 *        All rights reserved
 *  ---------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ---------------------------------------------------------------------------------------------------------------------
 *  \file     App / ChuongTrinhChinh.h
 *  \brief    Implementation of the Pump
 *  \details  see functional description below
 *
 ***********************************************************************************************************************/
#endif
#pragma endregion

#ifndef APP_01_FLAGS_H_
#define APP_01_FLAGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "main.h"
//----------------------------------------------------//
//-------- Begin: Các define cho cờ thời gian --------//
//----------------------------------------------------//
// Lưu ý: muốn sử dụng cờ nào thì mở define cho cờ đó.
#define FLAG_50ms
#define FLAG_100ms
#define FLAG_250ms
#define FLAG_500ms
//#define FLAG_800ms
#define FLAG_1s
//#define FLAG_1500ms
//#define FLAG_2s
//#define FLAG_3s
//#define FLAG_4s
//#define FLAG_5s
//#define FLAG_6s
//#define FLAG_10s
//#define FLAG_15s
//#define FLAG_30s
//#define FLAG_35s
//#define FLAG_15p
//#define FLAG_30p
//----------------------------------------------------//
//-------- End: Các define cho cờ thời gian ----------//
//----------------------------------------------------//

// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//-------------- Begin: KHAI BÁO CÁC BIẾN CỜ -------------------//
// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
typedef struct {
#ifdef FLAG_50ms
	uint8_t t50ms;
	uint16_t t50ms_count;
#endif // FLAG_50ms

#ifdef FLAG_100ms
	uint8_t t100ms;
	uint16_t t100ms_count;
#endif // FLAG_100ms

#ifdef FLAG_250ms
	uint8_t t250ms;
	uint16_t t250ms_count;
#endif // FLAG_250ms

#ifdef FLAG_500ms
	uint8_t t500ms;
	uint16_t t500ms_count;
#endif // FLAG_500ms

#ifdef FLAG_800ms
	uint8_t t800ms;
	uint16_t t800ms_count;
#endif // FLAG_800ms

#ifdef FLAG_1s
	uint8_t t1s;
	uint16_t t1s_count;
#endif // FLAG_1s

#ifdef FLAG_1500ms
	uint8_t t1500ms;
	uint16_t t1500ms_count;
#endif // FLAG_1500ms

#ifdef FLAG_2s
	uint8_t t2s;
	uint16_t t2s_count;
#endif // FLAG_2s

#ifdef FLAG_3s
	uint8_t t3s;
	uint16_t t3s_count;
#endif // FLAG_3s

#ifdef FLAG_4s
	uint8_t t4s;
	uint16_t t4s_count;
#endif // FLAG_4s

#ifdef FLAG_5s
	uint8_t t5s;
	uint16_t t5s_count;
#endif // FLAG_5s

#ifdef FLAG_6s
	uint8_t t6s;
	uint16_t t6s_count;
#endif // FLAG_6s

#ifdef FLAG_10s
	uint8_t t10s;
	uint16_t t10s_count;
#endif // FLAG_10s

#ifdef FLAG_15s
	uint8_t t15s;
	uint16_t t15s_count;
#endif // FLAG_15s

#ifdef FLAG_30s
	uint8_t t30s;
	uint16_t t30s_count;
#endif // FLAG_30s

#ifdef FLAG_35s
	uint8_t t35s;
	uint16_t t35s_count;
#endif // FLAG_35s

#ifdef FLAG_15p
	uint8_t t15p;
	uint32_t t15p_count;
#endif // FLAG_15p

#ifdef FLAG_30p
	uint8_t t30p;
	uint32_t t30p_count;
#endif // FLAG_15p
} flagType_t;

void SetTimerCount(flagType_t *Flag);

void Flag_CallBack(flagType_t *Flag);

#ifdef __cplusplus
}
#endif
#endif /* APP_01_FLAGS_H_ */
