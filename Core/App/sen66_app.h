/*
 * sen66_app.h
 * Created on: Mar 27, 2026
 * Author: NguyenBaDUy
 */

#ifndef INC_SEN66_APP_H_
#define INC_SEN66_APP_H_

/* --- NHÚNG THƯ VIỆN TẦNG DRIVER --- */
#include "Libraries/Sen66/Sen66.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- CẤU TRÚC DỮ LIỆU TẦNG APP (KIỂU FLOAT) --- */
// Đã đồng nhất tên thành SEN66_FloatData_t để không bị lỗi đỏ
typedef struct {
    float pm1;
    float pm2_5;
    float pm4;
    float pm10;
    float temp;
    float humid;
    float voc;
    float nox;
    float co2;
} SEN66_FloatData_t;

/* --- CÁC TRẠNG THÁI CỦA ỨNG DỤNG --- */
typedef enum {
    SEN66_STATUS_OK = 0,
    SEN66_STATUS_WARMING_UP,
    SEN66_STATUS_ERROR,
    SEN66_STATUS_UNINIT
} SEN66_AppStatus_t;

/* --- KHAI BÁO HÀM API TẦNG APP --- */
void SEN66_App_Init(void);
SEN66_AppStatus_t SEN66_App_Update(void);
SEN66_FloatData_t SEN66_App_GetData(void); // Trả về cấu trúc Float chuẩn
void SEN66_App_PrintData(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_SEN66_APP_H_ */
