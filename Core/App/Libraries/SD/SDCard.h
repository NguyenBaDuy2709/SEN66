/*
 * SDCard.h
 *
 *  Created on: Sep 2, 2025
 *      Author: DELL
 */

#ifndef APP_02_SDCARD_H_
#define APP_02_SDCARD_H_
#ifdef __cplusplus
extern "C" {
#endif

//#define SD_SPI_HANDLE hspi1
#include "main.h"
#include "fatfs.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef struct {
	uint8_t card_detect;

	uint8_t card_mouted;

	DWORD free_sectors_KiB;

	DWORD total_sectors_KiB;

	uint8_t dir_ok;

	uint8_t file_opened;

}sd_card_t;

FRESULT card_init(sd_card_t *_sd, SPI_HandleTypeDef* hspi);

FRESULT card_unmount(sd_card_t *_sd);

FRESULT card_remount(sd_card_t *_sd);

FRESULT get_free_space(sd_card_t *_sd);

FRESULT file_open(sd_card_t *_sd, char *path, char *filename, BYTE mode);

FRESULT file_close(sd_card_t *_sd);

FRESULT file_write(sd_card_t *_sd, char *data);

		int file_size();

		FRESULT find_folder_name(sd_card_t *_sd, const char *path, uint16_t ordinal, char *folder_name);

		FRESULT delete_folder(char *path);

		FRESULT list_dir (const char *path);

//FRESULT card_write(sd_card_t *_sd, char *path, char *filename);

#ifdef __cplusplus
	}
#endif
#endif /* APP_02_SDCARD_H_ */
