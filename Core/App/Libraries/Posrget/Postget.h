/*
 * Postget.h
 *
 *  Created on: Apr 4, 2026
 *      Author: Administrator
 */

#ifndef APP_LIBRARIES_POSRGET_POSTGET_H_
#define APP_LIBRARIES_POSRGET_POSTGET_H_

#ifdef __cplusplus
extern "C" {
#endif

// --- INCLUDE CÁC THƯ VIỆN ---
#include "main.h"
#include "cmsis_os.h"
#include "Libraries/Sim7600x/Sim7600x.h>
#include "Libraries/DS3231/DS3231.h"
#include "Libraries/PZEM004T/PZEM004T.h"

// --- CẤU HÌNH API ---
#define API_GET_TIME  "http://App.IoTVision.vn/api/ThoiGian"

// Thay vào file main.c hoặc file cấu hình của bạn
#define API_POST_URL   "http://App.IoTVision.vn/api/NY23_DuLieu?="
#define API_GET       "http://App.IoTVision.vn/api/NY23_DuLieuGuiXuongBoard?CheDo=1&key=572346353230"


// --- KHAI BÁO BIẾN TOÀN CỤC ---

extern int TrangThaiRelay_HienTai;             // Current_Relay_State
    extern int TrangThaiRelay_MucTieu;             // Target_Relay_State
    extern int CheDo_MucTieu;
extern osMutexId_t Sim7600MutexHandle;


int Get_API(void);
void Post_DataRTC(void);
void Get_DataRTC(void);


#ifdef __cplusplus
}

class PostGet {
public:
	PostGet();
};
#endif

#endif /* APP_LIBRARIES_POSRGET_POSTGET_H_ */
