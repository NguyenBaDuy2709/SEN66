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
 *  \file     App / 05_RTC.h
 *  \brief    Implementation of the Pump
 *  \details  see functional description below
 *
 ***********************************************************************************************************************/
#endif
#pragma endregion

#ifndef APP_05_RTC_H_
#define APP_05_RTC_H_

#include "main.h"
#include "string.h"
#include "Libraries/DS3231/DS3231.h"

class RTCDS3231 {
public:
	static DS3231_data_t g_dateTime;
public:
	void KhoiTaoRTC(void); //ok
	void LayRTCTuDS3231(void); //ok
public:
	void LayRTCTuLBSSim7600(void); //ok
	void LayRTCTuServerIoTVision(void); //ok
public:
	void CapNhatRTCChoDS3231(uint8_t thuThuc, uint8_t ngayThuc,
			uint8_t thangThuc, uint8_t namThuc, uint8_t gioThuc,
			uint8_t phutThuc, uint8_t giayThuc);
	void CaiDatRTCChoDS3231(void);
	char* ChuanHoaChuoiRTCDeGuiVeServer(void);
};

#endif /* APP_05_RTC_H_ */
