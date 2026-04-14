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

#ifndef APP_CHUONGTRINHCHINH_H_
#define APP_CHUONGTRINHCHINH_H_

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
} time_t;

typedef struct {
	char CodeGet[5];

	int HTTPGetCode;
	int HTTPPostCode;
	char HTTPGetResponse[1024];
	char HTTPPostResponse[1024];

	time_t rtc;
} server_t;
typedef struct {
	int date;
	int month;
	int year;
} install_time_t;

typedef struct {
	uint8_t update_status;
//	Board's ID
	char ID[15];
//  Ngày thiết lập board
	install_time_t install_time;
//	Last Latitude
	float LastLatitude;
//	Last LongiTude
	float LastLongitude;

} board_parameter_t;

typedef enum {
	PUMP_OFF = 0,       // Máy bơm dừng
	PUMP_RUNNING = 1,   // Máy bơm đang chạy
	PUMP_ERROR = 2,     // Máy bơm gặp lỗi
	PUMP_WAITING = 3    // Máy bơm trong trạng thái chờ
} PumpStatus_t;

typedef enum {
	THONG_BAO_BINH_THUONG = 0,
	THONG_BAO_QUA_TAI = 1,
	THONG_BAO_KHOI_DONG = 2,
	THONG_BAO_MUC_NUOC_THAP = 3,
	THONG_BAO_LOI_CAM_BIEN = 4
// Bạn có thể mở rộng thêm...
} PumpAlertCode_t;

typedef struct {
	int SoLuongBoPhunSuong;   // Tổng số bơm hoặc số lần phun
	float DongDien;              // Dòng điện hoạt động
	PumpStatus_t TrangThai;     // Trạng thái hiện tại (enum)
	int SoLanQuaTai;            // Đếm số lần quá tải
	int MucNuoc;                // Mực nước hiện tại
	float temperature;          // Nhiệt độ đo được
	float humidity;             // Độ ẩm đo được
	PumpAlertCode_t ThongBao;   // Mã cảnh báo (enum, dùng ánh xạ ra chuỗi)
} PumpControl_t;

typedef struct {
	char Device[32];
	char Lati[16];
	char Longti[24];
	char Mobile[8];
	char Wifi[8];
	char Tech[16];
	char Time[32];
} IoT_Data_t;

void KhoiTao(void);
void ChayChuongTrinhChinh(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_CHUONGTRINHCHINH_H_ */
