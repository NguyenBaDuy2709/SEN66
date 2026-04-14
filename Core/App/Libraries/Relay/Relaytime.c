/*
 * Relay.c
 *
 *  Created on: Jun 3, 2025
 *      Author: DELL
 */

#include "RelayTime.h"
Time_t currentTime = {0, 0};  // 🔧 Định nghĩa thật sự ở đây!
// Danh sách các khung giờ tưới
const KhungGio_t khungGioList[] = {
    {6, 0, 7, 30},    // Sáng: 06:00 → 07:30
    {17, 0, 18, 30}   // Chiều: 17:00 → 18:30
};

bool KiemTraKhungGio(void)
{
    uint16_t current = currentTime.hour * 60 + currentTime.min;

    for (int i = 0; i < sizeof(khungGioList) / sizeof(KhungGio_t); i++)
    {
        uint16_t start = khungGioList[i].startHour * 60 + khungGioList[i].startMin;
        uint16_t end   = khungGioList[i].endHour * 60 + khungGioList[i].endMin;

        if (current >= start && current <= end)
            return true;
    }

    return false;
}

