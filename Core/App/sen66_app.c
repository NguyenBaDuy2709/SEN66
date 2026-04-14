/*
 * sen66_app.c
 * Created on: Mar 27, 2026
 * Author: NguyenBaDuy
 */

#include "sen66_app.h"
#include <stdio.h>

static SEN66_FloatData_t app_data = {0};
static SEN66_AppStatus_t app_status = SEN66_STATUS_UNINIT;

/* --- THỰC THI HÀM --- */

void SEN66_App_Init(void) {
    // Không cần gọi Config() nữa vì HAL (MX_I2C1_Init) đã cấu hình xong
    Start_Or_Stop_Sensor(Measure_continuous, 50);
    app_status = SEN66_STATUS_WARMING_UP;
}

SEN66_AppStatus_t SEN66_App_Update(void) {
    if (app_status == SEN66_STATUS_UNINIT) {
        return SEN66_STATUS_UNINIT;
    }

    // Biến tạm để hứng dữ liệu (đã là kiểu float) từ tầng Driver
    Sensor_data raw_data = {0};

    // Cập nhật dữ liệu từ tầng Driver
    Return_Value_Of_Sensor(&raw_data);

    // KIỂM TRA LỖI LOGIC: Tầng Driver trả về số âm nếu có lỗi CRC
    if (raw_data.pm1 < 0.0f || raw_data.temp < -40.0f || raw_data.temp > 125.0f) {
        app_status = SEN66_STATUS_ERROR;
    }
    else if (raw_data.co2 == 0) { // Cảm biến đang khởi động, chưa đo được CO2
        app_status = SEN66_STATUS_WARMING_UP;
    }
    else {
        app_status = SEN66_STATUS_OK;
    }

    // Nếu không bị lỗi, gán thẳng dữ liệu vào app_data
    if (app_status != SEN66_STATUS_ERROR) {
        app_data.pm1   = raw_data.pm1;
        app_data.pm2_5 = raw_data.pm2_5;
        app_data.pm4   = raw_data.pm4;
        app_data.pm10  = raw_data.pm10;
        app_data.temp  = raw_data.temp;
        app_data.humid = raw_data.humid;
        app_data.voc   = raw_data.voc;
        app_data.nox   = raw_data.nox;
        app_data.co2   = (float)raw_data.co2; // Ép kiểu riêng cho CO2 vì ở Driver nó là uint16_t
    } else {
        // Nếu lỗi, gán toàn bộ về -1.0 để phần mềm chính dễ nhận dạng
        app_data.pm1   = -1.0f;
        app_data.pm2_5 = -1.0f;
        app_data.pm4   = -1.0f;
        app_data.pm10  = -1.0f;
        app_data.temp  = -1.0f;
        app_data.humid = -1.0f;
        app_data.voc   = -1.0f;
        app_data.nox   = -1.0f;
        app_data.co2   = -1.0f;
    }

    return app_status;
}

SEN66_FloatData_t SEN66_App_GetData(void) {
    return app_data;
}

void SEN66_App_PrintData(void) {
    if (app_status == SEN66_STATUS_OK) {
        printf("=== SEN66 ENVIRONMENT DATA ===\r\n");
        printf("PM 1.0 : %.1f ug/m3\r\n", app_data.pm1);
        printf("PM 2.5 : %.1f ug/m3\r\n", app_data.pm2_5);
        printf("PM 4.0 : %.1f ug/m3\r\n", app_data.pm4);
        printf("PM 10.0: %.1f ug/m3\r\n", app_data.pm10);
        printf("Temp   : %.2f C\r\n", app_data.temp);
        printf("Humid  : %.2f %%RH\r\n", app_data.humid);
        printf("VOC    : %.1f\r\n", app_data.voc);
        printf("NOx    : %.1f\r\n", app_data.nox);
        printf("CO2    : %.0f ppm\r\n", app_data.co2); // CO2 không có số thập phân nên dùng %.0f
        printf("==============================\r\n\n");
    }
    else if (app_status == SEN66_STATUS_WARMING_UP) {
        printf("SEN66 is warming up... Please wait.\r\n");
    }
    else {
        printf("SEN66 Error: Sensor disconnected or CRC failed!\r\n");
    }
}
