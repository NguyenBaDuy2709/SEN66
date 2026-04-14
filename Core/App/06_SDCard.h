/*
 * SDCard.h
 *
 *  Created on: Sep 27, 2025
 *      Author: DELL
 */

#ifndef APP_06_SDCARD_H_
#define APP_06_SDCARD_H_

#include "Libraries/SD/SDCard.h"
#include "Libraries/DS3231/DS3231.h"

class SDCard {
public:
	static sd_card_t TheNhoSD;
public:
	void KhoiTaoSDCard(void);
	void Check_new_file(void);
	void LogData(void);
};

#endif /* APP_06_SDCARD_H_ */
