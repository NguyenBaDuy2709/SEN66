#pragma region Ghi chú
#ifdef GhiChu
/**
 ************************************************************************************************************************
 * @Ten Du An      :  ChuongTrinhChinh.cpp
 * @Nguoi Thiet Ke :  KS.NGUYEN DINH DAT
 * @Ngay Tao       :  03/09/2025
 ************************************************************************************************************************
 * Copyright (c) 2025 by KS.NGUYENDINHDAT
 * All rights reserved
 ***********************************************************************************************************************/
#endif
#pragma endregion

#include "../App/ChuongTrinhChinh.h"

// Các thư viện C struct
extern "C" {
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "01_Flags.h"
#include "token.h"

#include "Libraries/SD/SDCard.h"
#include "Libraries/GNSS/GNSS.h"
#include "Libraries/Sim7600x/Sim7600x.h"

#include "SATY_AWD.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include <inttypes.h>
}

// Các thư viện C++ class
#include "05_RTC.h"
#include "06_SDCard.h"
#include "07_Relay.h"
#include "sen66_app.h" // Thư viện tầng App của SEN66

/* USER CODE BEGIN PD */
#define UNIQUE_ID_BASE_ADDR (0x1FFF7A10U)



#define CHAY_CHUONG_TRINH_CHINH_DELAY_MS 1 //Delay 1ms
#define TASK_THUC_THI_TAC_VU_THEO_FLAG_DELAY_MS 10
#define TASK_DOC_CAM_BIEN_DELAY_MS 1000
/* USER CODE END PD */

// ==============================================================================
// KHAI BÁO BIẾN TOÀN CỤC
// ==============================================================================
flagType_t g_Flag;
TAU1201_t g_Gps;
sim7600_t g_sim7600;
server_t g_ketNoiServer;

RTCDS3231 g_DS3231;
SDCard g_SDCard;
DS3231_data_t ds3231;

MyRelay_t g_Relay1;
MyRelay_t g_Relay2;

uint8_t ServerConnected = 1;

int16_t   live_rx_count=0;
int16_t     live_relay_status=0;

uint16_t i2c_status = 0;
uint16_t my_co2 = 0;
float    nhietdo = 0.0;
float    NO2 = 0.0;
float    VOC = 0.0;
float    PM1 = 0.0;
float    PM2_5 = 0.0;
float    PM4 = 0.0;
float    PM10= 0.0;
float    humidity = 0.0;

static char command[256];
static char payload[256];



// ==============================================================================
// KHAI BÁO TASK RTOS & XỬ LÝ NGOẠI VI
// ==============================================================================
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef hdma_usart2_rx;

// KHAI BÁO I2C1 ĐỂ SỬ DỤNG CHO CẢM BIẾN SEN66
extern I2C_HandleTypeDef hi2c1;

osThreadId_t ThucThiTacVuTheoFlagHandle;
const osThreadAttr_t ThucThiTacVuTheoFlagTask_attributes = { .name = "ThucThiTacVuTheoFlagTask", .stack_size = 512 * 4, .priority = (osPriority_t) osPriorityNormal, };

osThreadId_t DocCamBienHandle;
const osThreadAttr_t DocCamBienTask_attributes = { .name = "DocCamBienTask", .stack_size = 512 * 4, .priority = (osPriority_t) osPriorityNormal, };

osThreadId_t TaskPostDataMQTTHandle;
const osThreadAttr_t TaskPostDataMQTTTask_attributes = { .name = "TaskPostDataMQTT", .stack_size = 512 * 4, .priority = (osPriority_t) osPriorityAboveNormal, };

osSemaphoreId_t myBinaryLiquidLevelHandle;
const osSemaphoreAttr_t myBinaryLiquidLevel_attributes = { .name = "myBinaryLiquidLevel" };

extern "C" void TaskThucThiTacVuTheoFlag(void *argument);
extern "C" void TaskDocCamBien(void *argument);
extern "C" void TaskPostDataMQTT(void *argument);
extern "C" void ThucThiTacVuTheoFlag(void);

void LayIDThietBi(char *uniqueID_str);

// ==============================================================================
// INTERRUPT CALLBACKS
// ==============================================================================
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	if (htim->Instance == TIM2) { //Interrupt 50ms
		Flag_CallBack(&g_Flag);
	}
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == g_Gps.huart) {
		GPS_UART_CallBack(&g_Gps);
	}
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart == g_sim7600.hardware.huart) {
        SIM_UART_DMA_CallBack(&g_sim7600);
    }
}

// Hàm in console
int _write(int file, char *ptr, int len) {
    for(int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}

// ==============================================================================
// KHỞI TẠO HỆ THỐNG CHÍNH
// ==============================================================================
extern "C" void KhoiTao(void) {
	printf("--- KHOI TAO HE THONG --- \r\n");

	if (SIMInit(&g_sim7600, &huart2, &hdma_usart2_rx,
	SIM_PWR_Pin, SIM_PWR_GPIO_Port, SIM_RST_Pin, SIM_RST_GPIO_Port,
	SIM_HRST_Pin, SIM_HRST_GPIO_Port, SIM_STATUS_Pin, SIM_STATUS_GPIO_Port) == SIM7600_OK) {
		printf("Init SIMCOM module completed\r\n");
	} else {
		printf("Init SIMCOM module error\r\n");
	}
	if (GetSignal(&g_sim7600) != SIM7600_OK) {
		printf("Get Signal failed\r\n");
	}
	if (ServerConnected == 1) {
		HAL_GPIO_TogglePin(LED_SERVER_GPIO_Port, LED_SERVER_Pin);
	} else {
		HAL_GPIO_WritePin(LED_SERVER_GPIO_Port, LED_SERVER_Pin, GPIO_PIN_RESET);
	}
//	// KHỞI TẠO RTOS
//	osKernelInitialize();
	myBinaryLiquidLevelHandle = osSemaphoreNew(1, 1, &myBinaryLiquidLevel_attributes);

	ThucThiTacVuTheoFlagHandle = osThreadNew(TaskThucThiTacVuTheoFlag, NULL, &ThucThiTacVuTheoFlagTask_attributes);
	DocCamBienHandle = osThreadNew(TaskDocCamBien, NULL, &DocCamBienTask_attributes);
	TaskPostDataMQTTHandle = osThreadNew(TaskPostDataMQTT, NULL, &TaskPostDataMQTTTask_attributes);

//	osKernelStart();
}

void ChayChuongTrinhChinh(void) {
	osDelay(CHAY_CHUONG_TRINH_CHINH_DELAY_MS);
}

// ==============================================================================
// TASK 1: THỰC THI TÁC VỤ THEO CỜ & THỜI GIAN THỰC
// ==============================================================================
extern "C" void TaskThucThiTacVuTheoFlag(void *argument) {
	if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK) {
		printf("Failed to start TIM2 interrupt\r\n");
		Error_Handler();
	}
	g_SDCard.KhoiTaoSDCard();

//    Timer_t currenttime;
    g_DS3231.KhoiTaoRTC();

//    g_DS3231.CapNhatRTCChoDS3231(7, 4, 4, 26, 9, 37, 0);
//        g_DS3231.CaiDatRTCChoDS3231();
    HAL_Delay(1000);

    for (;;) {
//        DS3231_read_time(&ds3231);
//        currenttime.Gio = ds3231.Hour;
//        currenttime.Phut = ds3231.Minute
//
     ThucThiTacVuTheoFlag();
      osDelay(TASK_THUC_THI_TAC_VU_THEO_FLAG_DELAY_MS);
    }
}

// ==============================================================================
// TASK 2: ĐỌC CẢM BIẾN SEN66 TỪ I2C1 (THÔNG QUA TẦNG APP)
// ==============================================================================
extern "C" void TaskDocCamBien(void *argument) {

    osDelay(1200);

    SEN66_App_Init();
    osDelay(100);

    for (;;) {
        SEN66_AppStatus_t status = SEN66_App_Update();

        if (status == SEN66_STATUS_OK) {
            i2c_status = 1111;
                        SEN66_FloatData_t sen66_data = SEN66_App_GetData();
                        PM1      = sen66_data.pm1;
                        PM2_5    = sen66_data.pm2_5;
                        PM4      = sen66_data.pm4;
                        PM10     = sen66_data.pm10;
                        my_co2   = (uint16_t)sen66_data.co2;
                        NO2      = sen66_data.nox;
                        VOC      = sen66_data.voc;
                        nhietdo  = sen66_data.temp;
                        humidity = sen66_data.humid;
        }
        else if (status == SEN66_STATUS_ERROR || status == SEN66_STATUS_UNINIT)   i2c_status = 9999;
        SEN66_App_PrintData();
        osDelay(TASK_DOC_CAM_BIEN_DELAY_MS);
    }
}


extern "C" void TaskPostDataMQTT(void *argument) {

//  static char command[256];
//  static char payload[256];

  uint8_t mqtt_is_connected = 0;
  static int auto_pub_timer = 0;
static uint8_t rtc_synced =0;
  for(;;) {
      // --------------------------------------------------------
      // STATE 1: KHỞI TẠO VÀ KẾT NỐI MQTT
      // --------------------------------------------------------
      if (mqtt_is_connected == 0) {
          GetSignal(&g_sim7600);

          if (g_sim7600.Signal > 0) {

        	  if (rtc_synced == 0) {
        	                    printf("Dang dong bo thoi gian qua API IoTVision...\r\n");


        	                    g_DS3231.LayRTCTuServerIoTVision();
        	                    osDelay(500);


        	                    if (RTCDS3231::g_dateTime.Year > 20 && RTCDS3231::g_dateTime.Year < 99) {
        	                        g_DS3231.CaiDatRTCChoDS3231();
        	                        rtc_synced = 1; // Đánh dấu đã đồng bộ xong
        	                        printf("=> Dong bo RTC tu API THANH CONG!\r\n");
        	                    } else {
        	                        printf("=> Lay API that bai, se thu lai vao vong lap sau...\r\n");

        	                    }
        	                }
              ClearBuffer(&g_sim7600);

              SendCommand(&g_sim7600, (char*)"AT+CMQTTDISC=0,60\r\n", (char*)"OK", 1000, 1);
              SendCommand(&g_sim7600, (char*)"AT+CMQTTREL=0\r\n", (char*)"OK", 1000, 1);
              SendCommand(&g_sim7600, (char*)"AT+CMQTTSTOP\r\n", (char*)"OK", 1000, 1);
              osDelay(2000);

              SendCommand(&g_sim7600, (char*)"AT+CMQTTSTART\r\n", (char*)"+CMQTTSTART: 0", 2000, 1);
              SendCommand(&g_sim7600, (char*)"AT+CMQTTACCQ=0,\"STM32_IoT\"\r\n", (char*)"OK", 2000, 1);

              sprintf(command, "AT+CMQTTCONNECT=0,\"%s\",60,1,\"%s\"\r\n", THINGSBOARD_HOST, THINGSBOARD_TOKEN);
              if (SendCommand(&g_sim7600, command, (char*)"+CMQTTCONNECT: 0,0", 10000, 1) != -1) {
                  mqtt_is_connected = 1;
                  auto_pub_timer = 100; // Ép gửi ngay lần đầu
                  printf("ThingsBoard MQTT Connected!\r\n");
                  char sub_topic[] = "v1/devices/me/rpc/request/+";
                  sprintf(command, "AT+CMQTTSUB=0,%d,1\r\n", strlen(sub_topic));
                  if (SendCommand(&g_sim7600, command, (char*)">", 2000, 1) != -1) {
                      osDelay(50);
                      SendCommand(&g_sim7600, sub_topic, (char*)"+CMQTTSUB: 0,0", 3000, 1);
                      printf("=> Da Subscribe kenh dieu khien RPC!\r\n");
                  }
                  ClearBuffer(&g_sim7600);
              } else {
                  osDelay(3000);
                  continue;
              }
          }
      }

      // --------------------------------------------------------
      // STATE 2: LÀM VIỆC KHI ĐÃ KẾT NỐI (GỬI & NHẬN)
      // --------------------------------------------------------
      if (mqtt_is_connected == 1) {

          // --- LIÊN TỤC QUÉT BUFFER ĐỂ BẮT LỆNH BẬT/TẮT ---
          if (strstr(g_sim7600.buffer, "setState") != NULL) {
              live_rx_count++;
              if (strstr(g_sim7600.buffer, "true") != NULL) {
                  live_relay_status = 1;
                  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
              }
              else if (strstr(g_sim7600.buffer, "false") != NULL) {
                  live_relay_status = 0;
                  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
              }
              ClearBuffer(&g_sim7600);
          }

          // --- BỘ ĐẾM TỰ ĐỘNG GỬI TELEMETRY ---
          auto_pub_timer++;
          if (auto_pub_timer >= 10) {
                        auto_pub_timer = 0;
                        g_DS3231.LayRTCTuDS3231();
                        memset(payload, 0, sizeof(payload));
                           sprintf(payload,
                                                    "{"
                                                    "\"NhietDo\":%.2f,"
                                                    "\"KhiNO\":%.2f,"
                                                    "\"KhiCo2\":%u,"
                                                    "\"Pm1\":%.2f,"
                                                    "\"Pm2_5\":%.2f,"
                                                    "\"Pm4\":%.2f,"
                                                    "\"Pm10\":%.2f,"
                                                    "\"HopChatKhiVOC\":%.2f,"
                                                    "\"DoAm\":%.2f,"
                                                    "\"ThoiGian\":\"%02d:%02d:%02d %02d/%02d/20%02d\""
                                                    "}",
                                                    nhietdo, NO2, my_co2, PM1, PM2_5, PM4, PM10, VOC, humidity,

                                                    RTCDS3231::g_dateTime.Hour, RTCDS3231::g_dateTime.Minute, RTCDS3231::g_dateTime.Second,
                                                    RTCDS3231::g_dateTime.Date, RTCDS3231::g_dateTime.Month, RTCDS3231::g_dateTime.Year
                                                );
              char topic[] = "v1/devices/me/telemetry";
              int payload_len = strlen(payload);
              int topic_len = strlen(topic);
              uint8_t pub_success = 1;

              ClearBuffer(&g_sim7600);

              sprintf(command, "AT+CMQTTTOPIC=0,%d\r\n", topic_len);
              if (SendCommand(&g_sim7600, command, (char*)">", 2000, 1) != -1) {
                  osDelay(50);
                  if (SendCommand(&g_sim7600, topic, (char*)"OK", 2000, 1) == -1) pub_success = 0;
              } else pub_success = 0;

              if (pub_success == 1) {
                  memset(command, 0, sizeof(command));
                  sprintf(command, "AT+CMQTTPAYLOAD=0,%d\r\n", payload_len);
                  if (SendCommand(&g_sim7600, command, (char*)">", 2000, 1) != -1) {
                      osDelay(50);
                      if (SendCommand(&g_sim7600, payload, (char*)"OK", 2000, 1) == -1) pub_success = 0;
                  } else pub_success = 0;
              }

              if (pub_success == 1) {
                  if (SendCommand(&g_sim7600, (char*)"AT+CMQTTPUB=0,1,60\r\n", (char*)"+CMQTTPUB: 0,0", 8000, 1) != -1) {
                      printf("Publish Telemetry SUCCESS: %s\r\n", payload);
                  } else pub_success = 0;
              }

              if (pub_success == 0) {
                  mqtt_is_connected = 0;
                  printf("Publish FAILED! Resetting MQTT...\r\n");
              }
              ClearBuffer(&g_sim7600);
          } // KẾT THÚC KHỐI LỆNH GỬI DATA
      }
      osDelay(100);
  }
}
extern "C" void ThucThiTacVuTheoFlag(void) {
#ifdef FLAG_100ms
	if (g_Flag.t100ms) {
		//g_DS3231.LayRTCTuDS3231();
		g_Flag.t100ms = 0;
	}
#endif
#ifdef FLAG_1s
	if (g_Flag.t1s) {
		if ((SDCard::TheNhoSD.card_detect == 1) && (SDCard::TheNhoSD.card_mouted == 0)) {
			card_remount(&SDCard::TheNhoSD);
		}
		g_Flag.t1s = 0;
	}
#endif
}

//void LayIDThietBi(char *uniqueID_str) {
//	uint32_t unique_id[3];
//	unique_id[0] = *(uint32_t*) (UNIQUE_ID_BASE_ADDR);
//	unique_id[1] = *(uint32_t*) (UNIQUE_ID_BASE_ADDR + 4U);
//	unique_id[2] = *(uint32_t*) (UNIQUE_ID_BASE_ADDR + 8U);
//	sprintf(uniqueID_str, "%08" PRIX32 "%08" PRIX32 "%08" PRIX32, unique_id[2], unique_id[1], unique_id[0]);
//}

//char device_id_str[20];
//
//void Get_STM32_DeviceID(void) {
//    uint32_t uid_word0 = HAL_GetUIDw0();
//    // Bỏ qua word1
//    uint32_t uid_word2 = HAL_GetUIDw2();
//
//    // 2. Chỉ truyền tên biến vào hàm sprintf.
//    // Lưu ý: Với uint32_t, bạn nên dùng %08X và %05X (bỏ chữ 'l') để chuẩn form nhất.
//    sprintf(device_id_str, "%08X%05X", (unsigned int)uid_word0, (unsigned int)(uid_word2 & 0xFFFFF));
//}

