#include "common.h"
#include "rolling_code.h"
#include "Helpers.h"

void readCounterFromFlash(){
	//Open the file
	File file = LittleFS.open("/rollingcode.txt", "r");

	//Check if the file exists
	if(!file){
	Serial.println("rollingcode.txt doesn't exist. creating...");

	writeCounterToFlash();
	return;
	}

	rollingCodeCounter = file.parseInt();

	//Close the file
	file.close();
}

void writeCounterToFlash(){
	//Open the file 
	File file = LittleFS.open("/rollingcode.txt", "w");
	
	//Write to the file
	file.print(rollingCodeCounter);
	delay(1);
	//Close the file
	file.close();
	
	Serial.println("Write successful");
}

// returns true if status changes
void readRollingCode(byte rxRollingCode[CODE_LENGTH], uint8_t &door, uint8_t &light, uint8_t &lock, uint8_t &obstruction){
	// static bool skip = false;

	uint32_t rolling = 0;
	uint64_t fixed = 0;
	uint32_t data = 0;

	uint16_t cmd = 0;
	uint8_t nibble = 0;
	uint8_t byte1 = 0;
	uint8_t byte2 = 0;

	decode_wireline(rxRollingCode, &rolling, &fixed, &data);

	cmd = ((fixed >> 24) & 0xf00) | (data & 0xff);
			
	nibble = (data >> 8) & 0xf;
	byte1 = (data >> 16) & 0xff;
	byte2 = (data >> 24) & 0xff;

	printRollingCode(rxRollingCode);
	Serial.print(" cmd: ");
	Serial.print(cmd,HEX);
	Serial.print(" rolling: ");
	Serial.print(rolling,HEX);
	Serial.print(" fixed: ");
	Serial.print(fixed,HEX);
	Serial.print(" data: ");
	Serial.print(data,HEX);

	if(cmd == 0x81){
		door = nibble;
		light = (byte2 >> 1) & 1;
		lock = byte2 & 1;

		// if(skip){
		// 	skip = false;
		// }else{
			obstruction = (byte1 >> 6) & 1;
		// }

		Serial.print(" | STATUS:");
		Serial.print(" door:");
		Serial.print(nibble);
		Serial.print(" light:");
		Serial.print((byte2 >> 1) & 1);
		Serial.print(" lock:");
		Serial.print((byte2 & 1));
		Serial.print(" obs:");
		Serial.print((byte1 >> 6) & 1);

	}else if(cmd == 0x281){
		light ^= 1; // toggle bit

		Serial.print(" | LIGHT:");
		Serial.print(light);
	}else if(cmd == 0x84){
		obstruction = 0;
		// skip = true;
	}

	Serial.println("");
}

void getRollingCode(const char *command){
	Serial.print("rolling code for ");
	Serial.print(rollingCodeCounter);
	Serial.print("|");
	Serial.print(command);
	Serial.print(" : ");

	uint64_t id = 0x539;
	uint64_t fixed = 0;
	uint32_t data = 0;

	if(strcmp(command,"reboot1") == 0){
		fixed = 0x400000000;
		data = 0x0000618b;
	}else if(strcmp(command,"reboot2") == 0){
		fixed = 0;
		data = 0x01009080;
	}else if(strcmp(command,"reboot3") == 0){
		fixed = 0;
		data = 0x0000b1a0;
	}else if(strcmp(command,"reboot4") == 0){
		fixed = 0;
		data = 0x01009080;
	}else if(strcmp(command,"reboot5") == 0){
		fixed = 0x300000000;
		data = 0x00008092;
	}else if(strcmp(command,"reboot6") == 0){
		fixed = 0x300000000;
		data = 0x00008092;
	}else if(strcmp(command,"door1") == 0){
		fixed = 0x200000000;
		data = 0x01018280;
	}else if(strcmp(command,"door2") == 0){
		fixed = 0x200000000;
		data = 0x01009280;
	}else if(strcmp(command,"light") == 0){
		fixed = 0x200000000;
		data = 0x00009281;
	}else if(strcmp(command,"lock") == 0){
		fixed = 0x0100000000;
		data = 0x0000728c;
	}else{
		Serial.println("ERROR: Invalid command");
		return;
	}

	fixed = fixed | id;

	encode_wireline(rollingCodeCounter, fixed, data, txRollingCode);

	printRollingCode(txRollingCode);
	Serial.println("");

	if(strcmp(command,"door1") != 0){ // door2 is created with same counter and should always be called after door1
		rollingCodeCounter = (rollingCodeCounter + 1) & 0xfffffff;
	}
	return;
}

void printRollingCode(byte code[CODE_LENGTH]){
	for(int i = 0; i < CODE_LENGTH; i++){
		if(code[i] <= 0x0f) Serial.print("0");
		Serial.print(code[i],HEX);
	}
}