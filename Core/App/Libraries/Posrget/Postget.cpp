/*
 * PostGet.cpp
 *
 * Created on: Jan 17, 2026
 * Author: vinhv
 */

#include "Postget.h"
#include <stdio.h>
#include <string.h>
#include "05_RTC.h"
#include "../App/ChuongTrinhChinh.h"
#include "Libraries/Sim7600x/Sim7600x.h"
#include "Libraries/PZEM004T/PZEM004T.h"

extern server_t g_ketNoiServer;
extern sim7600_t g_sim7600;
extern RTCDS3231 g_DS3231;
extern PZEM_Data pzem_data;

// --- BIẾN TOÀN CỤC (Đã đổi tên tiếng Việt) ---
// Target: Lệnh mong muốn từ Server (App gửi xuống)
int TrangThaiRelay_MucTieu = 0; // Cũ: Target_Relay_State
int CheDo_MucTieu = 0;          // Cũ: Target_Mode_State
// Current: Trạng thái thực tế phần cứng đang chạy
int TrangThaiRelay_HienTai = 0; // Cũ: Current_Relay_State

PostGet::PostGet() { }

/* 02_PostGet.cpp - Đã tối ưu logic và đồng bộ tên biến */

// --- HÀM 1: GET API ---
// Lưu ý: Hàm này được gọi trong Task đã có Mutex bao bọc bên ngoài
// Trả về: 1 nếu có lệnh mới hợp lệ, 0 nếu không
int Get_API(void) {
    char url_get[256] = {0};
    sprintf(url_get, "%s", (char*) API_GET);

    // Reset bộ đệm
    memset(g_ketNoiServer.HTTPGetResponse, 0, sizeof(g_ketNoiServer.HTTPGetResponse));
    g_ketNoiServer.HTTPGetCode = 0;

    // Thực hiện lệnh AT lấy dữ liệu
    http_get(&g_sim7600, url_get, g_ketNoiServer.HTTPGetResponse, &g_ketNoiServer.HTTPGetCode);

    if (g_ketNoiServer.HTTPGetCode == 200) {
        char *response = g_ketNoiServer.HTTPGetResponse;
        char *pStart = strstr(response, "\"S\":\"");

        if (pStart != NULL) {
            char *pValue = pStart + 5;
            int relay_moi = -1, che_do_moi = -1;

            if (pValue[0] >= '0' && pValue[0] <= '9') relay_moi = pValue[0] - '0';
            if (pValue[1] >= '0' && pValue[1] <= '9') che_do_moi = pValue[1] - '0';

            if (relay_moi != -1 && che_do_moi != -1) {
                // Cập nhật trực tiếp vào biến toàn cục tiếng Việt
                TrangThaiRelay_MucTieu = relay_moi;
                CheDo_MucTieu = che_do_moi;

                // Lưu lại mã code để debug nếu cần
                g_ketNoiServer.CodeGet[0] = (char)relay_moi;
                g_ketNoiServer.CodeGet[1] = (char)che_do_moi;

                return 1; // Thành công, có dữ liệu mới
            }
        }
    }
    return 0; // Thất bại
}

// --- HÀM 2: POST DATA ---
// Hàm này cũng được gọi trong Task đã có Mutex bao bọc
void Post_DataRTC(void) {
    char du_lieu_gui[512] = {0}; // Buffer lớn
    DS3231_data_t thoi_gian_tam = RTCDS3231::g_dateTime;

    // Tính toán số liệu điện năng
    float dong_dien = ((pzem_data.current) * 0.00001444);
    int dien_ap = pzem_data.voltage;
    float cong_suat = dien_ap * dong_dien;
    int do_manh_song = g_sim7600.RSSI;

    // Đóng gói JSON gửi lên Server
    sprintf(du_lieu_gui,
        "{\"ID\": \"572346353230\",\"S\": \"%d%d1%d;%.2f;10;10;%d;%.2f;15;0.00;%02d:%02d:%02d %02d/%02d/%04d\"}",
            TrangThaiRelay_HienTai, // Gửi trạng thái thực tế
            CheDo_MucTieu,          // Gửi chế độ mong muốn (để App biết đang ở Mode nào)
            do_manh_song,
            dong_dien,
            dien_ap,
            cong_suat,
            thoi_gian_tam.Hour, thoi_gian_tam.Minute, thoi_gian_tam.Second,
            thoi_gian_tam.Date, thoi_gian_tam.Month, thoi_gian_tam.Year
            );

    http_post(&g_sim7600, (char*) API_POST_URL, du_lieu_gui);
}

// Hàm phụ trợ: Đổi thời gian ra tổng số giây (để so sánh chênh lệch)
long long DateTimeToSeconds(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    // Mảng số ngày trong các tháng (không nhuận)
    const uint16_t ngay_trong_thang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    long long tong_so_giay = 0;

    // 1. Cộng số giây của các năm trước đó (Tính từ năm 2000)
    for (uint16_t y = 2000; y < year; y++) {
        if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
            tong_so_giay += 366 * 86400;
        else
            tong_so_giay += 365 * 86400;
    }

    // 2. Cộng số giây của các tháng trong năm hiện tại
    for (uint8_t m = 1; m < month; m++) {
        if (m == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
            tong_so_giay += 29 * 86400;
        else
            tong_so_giay += ngay_trong_thang[m] * 86400;
    }

    // 3. Cộng ngày, giờ, phút, giây
    tong_so_giay += (day - 1) * 86400;
    tong_so_giay += hour * 3600;
    tong_so_giay += min * 60;
    tong_so_giay += sec;

    return tong_so_giay;
}

// --- HÀM 3: GET TIME & SYNC ---
void Get_DataRTC(void)
{
    char url_time[128];
    memset(url_time, 0, sizeof(url_time));
    sprintf(url_time, "%s", (char*) API_GET_TIME);

    // Lấy dữ liệu thời gian từ Server
    http_get(&g_sim7600, url_time, g_ketNoiServer.HTTPGetResponse, &g_ketNoiServer.HTTPGetCode);

    if (g_ketNoiServer.HTTPGetCode == 200)
    {
        int thu, ngay, thang, nam, gio, phut, giay;
        char* pJsonStart = strchr(g_ketNoiServer.HTTPGetResponse, '{');

        if (pJsonStart != NULL)
        {
            int items = sscanf(pJsonStart,
                "{\"Thu\":%d ,\"Ngay\":%d ,\"Thang\":%d ,\"Nam\":%d ,\"Gio\":%d ,\"Phut\":%d ,\"Giay\":%d}",
                &thu, &ngay, &thang, &nam, &gio, &phut, &giay);

            if (items == 7)
            {
                // --- KIỂM TRA ĐỘ LỆCH THỜI GIAN ---

                // 1. Đọc thời gian hiện tại trong chip DS3231
               g_DS3231.LayRTCTuDS3231();

                // 2. Quy đổi ra giây
                // Thời gian Server
                long long giay_server = DateTimeToSeconds(nam, thang, ngay, gio, phut, giay);

                // Thời gian DS3231
                uint16_t nam_rtc = RTCDS3231::g_dateTime.Year;
                if (nam_rtc < 100) nam_rtc += 2000;

                long long giay_rtc = DateTimeToSeconds(nam_rtc,
                                                    RTCDS3231::g_dateTime.Month,
                                                    RTCDS3231::g_dateTime.Date,
                                                    RTCDS3231::g_dateTime.Hour,
                                                    RTCDS3231::g_dateTime.Minute,
                                                    RTCDS3231::g_dateTime.Second);

                // 3. Tính độ lệch (Lấy trị tuyệt đối)
                long long do_lech = giay_server - giay_rtc;
                if (do_lech < 0) do_lech = -do_lech;

                // 4. So sánh: Nếu lệch > 180 giây (3 phút) thì cập nhật
                if (do_lech > 180)
                {
                    g_DS3231.CapNhatRTCChoDS3231(thu, ngay, thang, nam, gio, phut, giay);
                }
            }
        }
    }
}
