#ifndef RELAY_H
#define RELAY_H

#include "stm32f4xx_hal.h"  // Hoặc dòng chip bạn dùng
#include <stdbool.h>
#include "main.h"
// Trạng thái relay
#define _OFF 0
#define _ON  1

// Chế độ điều khiển
#define _MANUAL 0
#define _AUTO   1

// Cấu trúc lưu trạng thái relay
typedef struct {
    int K1;
    int K2;
    int MODE;
} Relay_t;

// Khởi tạo các chân và trạng thái
void KhoiTaoCacChan(Relay_t *r);

// Tắt toàn bộ relay
void OFFCacRole(Relay_t *r);

// Bật toàn bộ relay
void ONCacRole(Relay_t *r);

// Điều khiển từng relay theo trạng thái hiện tại
bool ONOFFBangTayK1(Relay_t *r, int v);
bool ONOFFBangTayK2(Relay_t *r, int v);

#endif // RELAY_H
