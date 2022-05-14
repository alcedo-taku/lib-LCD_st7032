/*
 * lcdst7032.cpp
 *
 *  Created on: Apr 8, 2020
 *      Author: Satoshi Ohya
 */

#include <lcdst7032.hpp>

lcdSt7032::lcdSt7032(){
}

lcdSt7032::	lcdSt7032(I2C_HandleTypeDef *hi2c, uint32_t timeout){
	i2c = hi2c;
	this->timeout = timeout;

}

//display instruction
void lcdSt7032::clearDisplay(){
	writeCommand(0b1);
}

/**
 * カーソルをホームポジションに移動する関数
 */
void lcdSt7032::returnHome(){
	writeCommand(0b10);
}

void lcdSt7032::setEntryMode(bool direction){
	writeCommand(0b100 | direction<<1);
}

void lcdSt7032::switchDisplay(bool enableDisplay, bool enableCursor, bool enableCursorBrinck){
	writeCommand(0b1000 | enableDisplay<<2 | enableCursor<<1 | enableCursorBrinck);
}

void lcdSt7032::setFunction(bool is8bits, bool is2Line, bool isDoubleHight, bool enableExtentionInstruction){
	writeCommand(0b100000 | is8bits<<4 | is2Line<<3 | (isDoubleHight & !is2Line) << 2 | enableExtentionInstruction);
}

/**
 * DDRAM=液晶画面の表示アドレスを指定する関数
 * @param address
 */
void lcdSt7032::setDDRAMaddress(uint8_t address){
	writeCommand(0b1<<7 | address);
}

/*
void lcdSt7032::writeData(uint8_t data){
	uint8_t buf[]={0x40, data};
	sendCommand(buf, 2);
}
*/
void lcdSt7032::writeData(uint8_t t_data){
	uint8_t controlByte = 0x40; //01000000 Co=0 RS=1
	uint8_t buf[] = {controlByte, t_data};
	HAL_I2C_Master_Transmit(i2c, addres << 1, buf, 2, timeout);// data byte i2cの番号注意
}

void lcdSt7032::shiftCursorOrDisplay(bool isDisplayShift, bool isRightShift){
	writeCommand(0b1<<4 | isDisplayShift<<3 | isRightShift<<2);
}

void lcdSt7032::setCGRAM(uint8_t address){
	if(address <= 0b111111){
		writeCommand(0b1<<6 | address);
	}
}

void lcdSt7032::setInternalOSCFrequency(bool selectBias, uint8_t frequency){
	if(frequency <= 0b111){
		writeCommand(0b1<<4 | selectBias<<3 | frequency);
	}
}

void lcdSt7032::setICONAddress(uint8_t address){
	if(address <= 0b1111){
		writeCommand(0b1<<6 | address);
	}
}

void lcdSt7032::setPowerICONContrast(bool enableIconDisplay, bool enableBoosterCircuit, uint8_t contrst){
	writeCommand(0b101<<4 | enableIconDisplay<<3 | enableBoosterCircuit<<2 | (contrst>>4&0b11));
}

void lcdSt7032::setFollowerControll(bool enableFollowerCircuit, uint8_t followerAmplifiedRatio){
	if(followerAmplifiedRatio <= 0b111){
		writeCommand(0b11<<5 | enableFollowerCircuit<<3 | followerAmplifiedRatio);
	}
}

void lcdSt7032::setContrast(uint8_t contrast){
	writeCommand(0b111<<4 | (contrast&0b1111));
}
//display instruction end

/**
 * カーソルの位置を指定する関数
 * @param x
 * @param y
 */
void lcdSt7032::setCursor(uint8_t x, uint8_t y){
	writeCommand(0b1<<7 | y<<6 | x);
}

void lcdSt7032::writeCommand(uint8_t t_command){
	uint8_t controlByte = 0x00; //00000000 Co=0 RS=0
	uint8_t buf[] = {controlByte, t_command};
	HAL_I2C_Master_Transmit(i2c, addres<<1, buf, 2, timeout);
}
/*
void lcdSt7032::writeCommand(uint8_t *buf, uint8_t size){
	HAL_I2C_Master_Transmit(i2c, addres<<1, buf, 2, timeout);
}
*/




void lcdSt7032::print(const char *string){
	uint8_t buf[2]={0x40, 0};
	while(*string){
		buf[1]=*string++;
		HAL_I2C_Master_Transmit(i2c, addres<<1, buf, 2, timeout);
	}
}

void lcdSt7032::print(const char *string, uint8_t length){
	uint8_t buf[2]={0x40, 0};
	for(uint8_t n=0; n<length; n++){
		buf[1]=*(string+n);
		HAL_I2C_Master_Transmit(i2c, addres<<1, buf, 2, timeout);
	}
}

void lcdSt7032::print(uint8_t num){
    std::string tmp = std::to_string(num);
    char const *num_char = tmp.c_str();
	print(num_char);
}
//*/

void lcdSt7032::initLCD(uint8_t contrast){
	setFunction(1, 1, 0, 0); //00111000
	HAL_Delay(1);
	setFunction(1, 1, 0, 1); //00111001
	HAL_Delay(1);
	setInternalOSCFrequency(0, 0b100); //00010100
	HAL_Delay(1);
	setContrast(contrast); //01110001
	HAL_Delay(1);
	setPowerICONContrast(0, 1, contrast); //01010110
	HAL_Delay(1);
	setFollowerControll(1, 0b100); //01101100
	HAL_Delay(200);
	setFunction(1, 1, 0, 0); //00111000
	HAL_Delay(1);
	clearDisplay(); //00000001
	HAL_Delay(1);
	switchDisplay(1, 0, 0); //00001100
	HAL_Delay(1);
}
