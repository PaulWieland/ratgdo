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
  if(rollingCodeCounter > 0 && rollingCodeCounter < BIN_COUNT_OFFSET){
    return;
  }
  //Open the file 
  File file = LittleFS.open("/rollingcode.txt", "w");
  
  //Write to the file
  file.print(rollingCodeCounter);
  delay(1);
  //Close the file
  file.close();
  
  Serial.println("Write successful");
}

void getRollingCode(const char *command){
  Serial.print("get rolling code for ");
  Serial.print(rollingCodeCounter);
  Serial.print(" : ");
  Serial.println(command);

  uint64_t fixed = 0;
  uint32_t data = 0;


  if(strcmp(command,"reboot1") == 0){
    fixed = 0;
    data = 0x01009080;
  }else if(strcmp(command,"reboot2") == 0){
    fixed = 0x0400000000;
    data = 0x0000618b;
  }else if(strcmp(command,"reboot3") == 0){
    fixed = 0;
    data = 0x0000b180;
  }else if(strcmp(command,"reboot4") == 0 || strcmp(command,"reboot5") == 0){
    fixed = 0x0300000000;
    data = 0x00008092;
  }else if(strcmp(command,"door1") == 0){
    fixed = 0x0200000000;
    data = 0x01018280;
  }else if(strcmp(command,"light") == 0){
    fixed = 0x0200000000;
    data = 0x00009281;
  }else{
    Serial.println("ERROR: Invalid command");
    return;
  }

  encode_wireline(rollingCodeCounter, fixed, data, rollingCode);

  printRollingCode();

  rollingCodeCounter++;
  return;
}

void printRollingCode(){
  for(int i = 0; i < CODE_LENGTH; i++){
    if(rollingCode[i] <= 0x0f) Serial.print("0");
    Serial.print(rollingCode[i],HEX);
  }
  Serial.println("");
}