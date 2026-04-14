#pragma region Ghi chú
#ifdef GhiChu
/**
 ************************************************************************************************************************
 * @Ten Du An      :  ChuongTrinhChinh.cpp
 * @Nguoi Thiet Ke :  KS.NGUYEN DINH DAT
 * @Ngay Tao       :  03/09/2025
 ************************************************************************************************************************
 * Copyright (c) 2025 by KS.NGUYENDINHDAT
 *        All rights reserved
 *  ---------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ---------------------------------------------------------------------------------------------------------------------
 *  \file     App / ChuongTrinhChinh.cpp
 *  \brief    Implementation of the Pump
 *  \details  see functional description below
 *
 ***********************************************************************************************************************/
/**
 ************************************************************************************************************************
 |-----------------------------------------------------------------------------------------------------------------------
 |               R E V I S I O N   H I S T O R Y
 |-----------------------------------------------------------------------------------------------------------------------
 | Date       Ver   Author  Description
 | ---------  ---  ------  ----------------------------------------------------------------------------------------------
 | 2025-09-26 1.3  Dat	   Thiết kế form code C++
 | 2025-09-26 1.3  Dat     Lập trình Cấu hình giao tiếp UART, I2C, SPI, GPIO.
 | 2025-09-26 1.3  Dat     Lập trình RTOS 3 Task "TaskThucThiTacVuTheoFlag".
 |												 "TaskDocCamBien".
 |												 "TaskPostGetDuLieuTuBoard".
 | 2025-09-26 1.3  Dat     Lập trình lấy ID thiết bị
 | 2025-09-26 1.3  Dat     Lập trình
 | 2025-09-26 1.3  Dat     Lập trình
 | 2025-09-26 1.3  Dat     Lập trình
 | 2025-09-26 1.3  Dat     Lập trình
 | 2025-09-26 1.3  Dat     Lập trình
 ************************************************************************************************************************
 *@Mo ta:
 *  &&TIMER THỰC HIỆN TÁC VỤ FLAG
 *   void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
 *      //	Interrupt 50ms
 *      if (htim->Instance >= TIM2) {
 *      	Flag_CallBack(&_Flag); // using Flag
 *      }
 *   }
 *  &&Gọi hàm này ở main.c or ChayChuongTrinhChinh lưu ý them extern "C"
 *****************************************************************************
 *@Mo ta:
 *	&&Cấu hình timer 64MHz
 *		htim2.Instance = TIM2;
 *  	htim2.Init.Prescaler = 63999;
 * 	 	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
 *  	htim2.Init.Period = 499;
 *  	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
 *  	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
 *  &&Chạy hàm này trong KhoiTao();
 *  	HAL_TIM_Base_Start_IT(&htim2);
 ******************************************************************************
 */
#endif
#pragma endregion

#include "05_RTC.h"
extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ChuongTrinhChinh.h"
#include "Libraries/DS3231/DS3231.h"
#include "Libraries/Sim7600x/Sim7600x.h"

#define API_GET_TIME_IOTVISION "http://App.IoTVision.vn/api/ThoiGian"
}

extern I2C_HandleTypeDef hi2c1;
extern sim7600_t g_sim7600;
extern server_t g_ketNoiServer;

DS3231_data_t RTCDS3231::g_dateTime;
DS3231_data_t *ThoiGian = &RTCDS3231::g_dateTime;

void RTCDS3231::KhoiTaoRTC(void) {
	if (rtc_init(&hi2c1) != HAL_OK) {
#ifdef debug_RTC
		printf("Init RTC module error\r\n");
#endif
	}
}
void RTCDS3231::LayRTCTuDS3231(void) {
	DS3231_read_time(&RTCDS3231::g_dateTime);
#ifdef debug_RTC
	printf("Read time RTC module error\r\n");
#endif
}
void RTCDS3231::LayRTCTuLBSSim7600(void) {
	if (GetLBS(&g_sim7600) == SIM7600_OK) {
		if (g_sim7600.LBS_Date.Year > 0) {
			ThoiGian->Day = g_sim7600.LBS_Date.Day;
			ThoiGian->Date = g_sim7600.LBS_Date.Date;
			ThoiGian->Month = g_sim7600.LBS_Date.Month;
			ThoiGian->Year = g_sim7600.LBS_Date.Year;

			ThoiGian->Hour = g_sim7600.LBS_Date.Hour;
			ThoiGian->Minute = g_sim7600.LBS_Date.Minute;
			ThoiGian->Second = g_sim7600.LBS_Date.Second;

		}
	}
#ifdef debug_RTC
	printf("LayRTCTuLBSSim7600 RTC module error\r\n");
#endif
}
void RTCDS3231::LayRTCTuServerIoTVision(void) {
	if (http_get(&g_sim7600, (char*) API_GET_TIME_IOTVISION,
			g_ketNoiServer.HTTPGetResponse,
			&g_ketNoiServer.HTTPGetCode) == SIM7600_OK) {
		if (g_ketNoiServer.HTTPGetCode == 200) {
			int foundRes = 0;
			for (int i = 0; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ':') {
					foundRes = i + 1;
					break;
				}
			}
			g_ketNoiServer.rtc.Day = (g_ketNoiServer.HTTPGetResponse[foundRes]
					- 48) + 1;
			foundRes += 1;
			//		Find Day
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ':') {
					foundRes = i + 1;
					break;
				}
			}
			g_ketNoiServer.rtc.Date = 0;
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ',') {
					foundRes = i + 1;
					break;
				}
				g_ketNoiServer.rtc.Date *= 10;
				g_ketNoiServer.rtc.Date += g_ketNoiServer.HTTPGetResponse[i]
						- 48;
			}
			//		Find Month
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ':') {
					foundRes = i + 1;
					break;
				}
			}

			g_ketNoiServer.rtc.Month = 0;
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ',') {
					foundRes = i + 1;
					break;
				}
				g_ketNoiServer.rtc.Month *= 10;
				g_ketNoiServer.rtc.Month += g_ketNoiServer.HTTPGetResponse[i]
						- 48;
			}

			//		Find Year
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ':') {
					foundRes = i + 1;
					break;
				}
			}

			g_ketNoiServer.rtc.Year = 0;
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ',') {
					foundRes = i + 1;
					g_ketNoiServer.rtc.Year -= 2000;
					break;
				}
				g_ketNoiServer.rtc.Year *= 10;
				g_ketNoiServer.rtc.Year += g_ketNoiServer.HTTPGetResponse[i]
						- 48;
			}

			//		Find Hour
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ':') {
					foundRes = i + 1;
					break;
				}
			}

			g_ketNoiServer.rtc.Hour = 0;
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ',') {
					foundRes = i + 1;
					break;
				}
				g_ketNoiServer.rtc.Hour *= 10;
				g_ketNoiServer.rtc.Hour += g_ketNoiServer.HTTPGetResponse[i]
						- 48;
			}

			//		Find Minute
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ':') {
					foundRes = i + 1;
					break;
				}
			}

			g_ketNoiServer.rtc.Minute = 0;
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ',') {
					foundRes = i + 1;
					break;
				}
				g_ketNoiServer.rtc.Minute *= 10;
				g_ketNoiServer.rtc.Minute += g_ketNoiServer.HTTPGetResponse[i]
						- 48;
			}

			//		Find Second
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == ':') {
					foundRes = i + 1;
					break;
				}
			}

			g_ketNoiServer.rtc.Second = 0;
			for (int i = foundRes; i <= 256; ++i) {
				if (g_ketNoiServer.HTTPGetResponse[i] == '}')
					break;
				g_ketNoiServer.rtc.Second *= 10;
				g_ketNoiServer.rtc.Second += g_ketNoiServer.HTTPGetResponse[i]
						- 48;
			}

			if (g_ketNoiServer.rtc.Year > 0) {
				ThoiGian->Day = g_ketNoiServer.rtc.Day;
				ThoiGian->Date = g_ketNoiServer.rtc.Date;
				ThoiGian->Month = g_ketNoiServer.rtc.Month;
				ThoiGian->Year = g_ketNoiServer.rtc.Year;

				ThoiGian->Hour = g_ketNoiServer.rtc.Hour;
				ThoiGian->Minute = g_ketNoiServer.rtc.Minute;
				ThoiGian->Second = g_ketNoiServer.rtc.Second;

			}
		}
		memset(g_ketNoiServer.HTTPGetResponse, 0,
				sizeof(g_ketNoiServer.HTTPGetResponse));
	}
#ifdef debug_RTC
	printf("LayRTCTuServerIoTVision RTC module error\r\n");
#endif
}
void RTCDS3231::CapNhatRTCChoDS3231(uint8_t thuThuc, uint8_t ngayThuc,
		uint8_t thangThuc, uint8_t namThuc, uint8_t gioThuc, uint8_t phutThuc,
		uint8_t giayThuc) {

	ThoiGian->Day = thuThuc;
	ThoiGian->Date = ngayThuc;
	ThoiGian->Month = thangThuc;
	ThoiGian->Year = namThuc;

	ThoiGian->Hour = gioThuc;
	ThoiGian->Minute = phutThuc;
	ThoiGian->Second = giayThuc;

#ifdef debug_RTC
	printf("CapNhatRTCChoDS3231 RTC module OK\r\n");
#endif
}
void RTCDS3231::CaiDatRTCChoDS3231(void) {
	DS3231_write_time(&RTCDS3231::g_dateTime);
}
char* RTCDS3231:: ChuanHoaChuoiRTCDeGuiVeServer(void) {
	static char s[24]; // "HH:MM:SS DD/MM/YYYY" + '\0' = 20, để rộng tay 24

	snprintf(s, sizeof s, "%02d:%02d:%02d %02d/%02d/%04d", ThoiGian->Hour,
			ThoiGian->Minute, ThoiGian->Second, ThoiGian->Date, ThoiGian->Month,
			ThoiGian->Year);
	return s;
}
