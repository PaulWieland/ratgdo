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

  rollingCode[0] = 0x55;
  rollingCode[1] = 0x01;
  rollingCode[2] = 0x00;

  byte codeSet[CODE_SET_LENGTH];
  unsigned int commandOffset;

  // File starts at counter 0. Each set of 6 codes for a given counter is 82 bytes long
  // The first code in the file can be a different counter value (such as 128) so a counter offset is needed
  unsigned int seekTo = (rollingCodeCounter - BIN_COUNT_OFFSET) * CODE_SET_LENGTH;

  rollingCodeCounter++;

  // read codeSetLength bytes starting from the seekTo position
  LittleFS.begin();
  File file = LittleFS.open("ratgdo_codes.hex", "r");

  if(!file){
    Serial.println("ERROR: Failed to open ratgdo_codes.hex for reading");
    //LittleFS.end();
    return;
  }

  if(rollingCodeCounter < BIN_COUNT_OFFSET){
    Serial.println("ERROR: Counter too small. Code not found in ratgdo_codes.hex");
    //LittleFS.end();
    return;
  }

  if(file.size() < seekTo){
    Serial.println("ERROR: Counter too large. Code not found in ratgdo_codes.hex");
    //LittleFS.end();
    return;
  }
  
  Serial.print("Command Location: ");
  Serial.println(seekTo);

  file.seek(seekTo, SeekSet);
  file.read(codeSet,CODE_SET_LENGTH);
  //LittleFS.end();

  // copy the first 16 bytes into the code
  // this is the valid code for reboot1
  for(int i = 0; i < 16; i++){
    rollingCode[i+3] = codeSet[i];
  }

  if(strcmp(command,"reboot1") == 0){
    printRollingCode();
    return;
  }
    
    
  if(strcmp(command,"reboot2") == 0){
    commandOffset = 0;
  }else if(strcmp(command,"reboot3") == 0){
    commandOffset = 1;
  }else if(strcmp(command,"reboot4") == 0 || strcmp(command,"reboot5") == 0){
    commandOffset = 2;
  }else if(strcmp(command,"door1") == 0){
    commandOffset = 3;
  }else if(strcmp(command,"light") == 0){
    commandOffset = 4;
  }else{
    Serial.println("ERROR: Invalid command");
    return;
  }
    
  // each command after reboot1 has 6 unique bytes stored in a row
  byte replacement[6];
  unsigned int j = 0;
  for(unsigned int i = 16 + (commandOffset * 6); i < (16 + (commandOffset * 6)) + 6; i++){
    replacement[j] = codeSet[i];
    j++;
  }

  rollingCode[6] = replacement[0];
  rollingCode[7] = replacement[1];
  rollingCode[10] = replacement[2];
  rollingCode[16] = replacement[3];
  rollingCode[17] = replacement[4];
  rollingCode[18] = replacement[5];
  printRollingCode();
  return;
}

void printRollingCode(){
  for(int i = 0; i < CODE_LENGTH; i++){
    Serial.print(rollingCode[i],HEX);
  }
  Serial.println("");
}


