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

#include "06_SDCard.h"
extern "C" {
#include "main.h"
#include "ChuongTrinhChinh.h"
#include "Libraries/SD/SDCard.h"
}
#include "05_RTC.h"

#define BACKUP_PATH									"/BACKUP"
#define TEMP_PATH									"/TEMP"
#define FIRMWARE_PATH								"/FIRMWARE"

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;

sd_card_t SDCard::TheNhoSD;
sd_card_t *SD = &SDCard::TheNhoSD;
//sd_card_t *SD = &g_TheNhoSD;

extern RTCDS3231 g_DS3231;
uint8_t sd_card_found;
/*
 * GPIO Interrupt function
 * */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == SD_CD_Pin) {
		if (HAL_GPIO_ReadPin(SD_CD_GPIO_Port, SD_CD_Pin) == 0) {
			SD->card_detect = 1;
		} else {
			SDCard::TheNhoSD.card_detect = 0;
			if (SD->card_mouted == 1) {
				card_unmount(&SDCard::TheNhoSD);
			}
		}
	}
}
void SDCard::KhoiTaoSDCard(void) {
	FRESULT fres = card_init(&SDCard::TheNhoSD, &hspi1);
	if (fres == FR_OK) {
#ifdef debug_SDCard
		printf("Init SD card completed, sd card size: %ld/%ld (Kb)\r\n",
				(g_TheNhoSD.total_sectors_KiB - g_TheNhoSD.free_sectors_KiB),
				g_TheNhoSD.total_sectors_KiB);
#endif
	} else {
#ifdef debug_SDCard
		printf("Init SD card error: %d\r\n", fres);
#endif
	}
}
void SDCard::Check_new_file(void) {
	if (file_size() == 0) {
		file_write(&SDCard::TheNhoSD,
				(char*) "Time,Latitude,Longitude,Speed(km/h),Satelite,ACC,NFC ID,Fuel,Fuel Temp(*C),Temperature(*C),Humidity(%),Location link\n");
	}
}
//void SDCard::LogData(void) {
//
//	char folderPath[40];
//	char fileName[20];
//	char dataWrite[200];
//
//	if (g_DS3231.g_dateTime.Month < 10) {
//		sprintf(folderPath, "%s/%d_0%d", BACKUP_PATH,
//				(g_DS3231.g_dateTime.Year + 2000), g_DS3231.g_dateTime.Month);
//	} else
//		sprintf(folderPath, "%s/%d_%d", BACKUP_PATH,
//				(g_DS3231.g_dateTime.Year + 2000), g_DS3231.g_dateTime.Month);
//	//	sprintf(folderPath, "%s/2024_10", BACKUP_PATH);
//	//
//	//	sprintf(folderPath, "%s", TEMP_PATH);
//	//	sprintf(fileName,"%d_%d_%d.csv", (date_time.Year + 2000), date_time.Month, date_time.Date);
//
//	sprintf(fileName, "%d.csv", g_DS3231.g_dateTime.Date);
//
//	if (file_open(&SDCard::TheNhoSD, folderPath, fileName,
//	FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
//		this->Check_new_file();
//		sprintf(dataWrite,
//				"%d:%d:%d,%f,%f,%.2f,%f,%d,%s,%d,%d,NS,NS,\"https://www.google.com/maps/?q=%f,%f\"\n",
//				date_time.Hour, date_time.Minute, date_time.Second, Latitude,
//				Longitude, Speed, Satelites, ACC_State, BoardParameter.driverID,
//				Latitude, Longitude);
//		file_write(&sd, dataWrite);
//	}
//
//	file_close (&sd);
//}
