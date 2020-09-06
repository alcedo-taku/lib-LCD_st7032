/*
 * lcdst7032.hpp
 *
 *  Created on: Apr 8, 2020
 *      Author: Satoshi Ohya
 */

#ifndef INC_LCDST7032_HPP_
#define INC_LCDST7032_HPP_

#include <vector>
#include "main.h"

class lcdSt7032 {
public:
	lcdSt7032();
//	lcdSt7032(I2C_HandleTypeDef *hi2c);
	lcdSt7032(I2C_HandleTypeDef *hi2c, uint32_t timeout);
	void clearDisplay();
	void returnHome();
	void setEntryMode(bool direction);
	void switchDisplay(bool enableDisplay, bool enableCursor, bool enableCursorBrinck);
	void setFunction(bool enable8bits, bool enable2Line, bool enableDoubleHight, bool enableExtentionInstruction);
	void setDDRAMaddress(uint8_t address);
	void writeData(uint8_t data);
	void shiftCursorOrDisplay(bool enableDisplayCursor, bool enableShiftLeft);
	void setCGRAM(uint8_t address);
	void setInternalOSCFrequency(bool selectBias, uint8_t frequency);
	void setICONAddress(uint8_t address);
	void setPowerICONContrast(bool enableIconDisplay, bool enableBoosterCircuit, uint8_t contrst);
	void setFollowerControll(bool enableFollowerCircuit, uint8_t followerAmplifiedRatio);
	void setContrast(uint8_t contrast);
	void setCursor(uint8_t x, uint8_t y);
//	void print(const char *string);
//	void print(const char *string, uint8_t length);
	void initLCD(uint8_t contrast);
private:
	I2C_HandleTypeDef *i2c;
	uint8_t timeout=100;
	const uint8_t addres = 0b0111110;

	void writeCommand(uint8_t command);
//	void sendCommand(uint8_t *buf, uint8_t size);
};

#endif /* INC_LCDST7032_HPP_ */
