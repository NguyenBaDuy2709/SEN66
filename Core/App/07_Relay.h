#ifndef _07_RELAY_H_
#define _07_RELAY_H_

#include "main.h" // Thư viện này chứa GPIO_PIN_SET
#include <stdbool.h>

typedef struct {
    GPIO_TypeDef *Port; // Thêm dòng này để struct biết chân ở đâu
    uint16_t Pin;       // Thêm dòng này
    bool run;
    uint32_t mocthoigian; // Đã đổi tên chuẩn
} MyRelay_t;
typedef struct {
	uint8_t Gio;
	uint8_t Phut;
}Timer_t;
typedef struct {
	uint8_t giobatdau;
	uint8_t gioketthuc;
	uint8_t phutbatdau;
	uint8_t phutketthuc;
}Khunggio_t;

void MyRelay_Init(MyRelay_t *me, GPIO_TypeDef *RelayPort, uint16_t RelayPin);
void MyRelay_Toggle(MyRelay_t *me);
void MyRelay_Time(MyRelay_t *me);
bool MyRelay_TimeControll(MyRelay_t *me,Timer_t currenttime);

#endif
