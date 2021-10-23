#include <RF24.h>
#include <FastCRC.h>
#include <FastCRC_tables.h>

#include <stdio.h>
#include <string.h>

#define ADDR_WIDTH 3
#define BUFSIZE (ADDR_WIDTH + 3)

#define COMMAND_LENGHT 20

int CHANNEL = 41;

RF24 radio(7, 8);
FastCRC16 CRC16;

uint8_t baseAddress[] = {0x56, 0x34, 0x12};
uint8_t clickerAdress[] = {0x64, 0x40, 0x9e};

byte outgoingData[BUFSIZE];
byte crcBuffer[BUFSIZE + 3];

char commandBuffer[COMMAND_LENGHT];
int cursor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("fackeClicker 1.0");

  //setup nrf24
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setAutoAck(false);
  radio.disableCRC();
  radio.setPayloadSize(BUFSIZE);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(CHANNEL);
  radio.setAddressWidth(3);
  radio.openWritingPipe(baseAddress);

  //setup crc
  crcBuffer[0] = baseAddress[2];
  crcBuffer[1] = baseAddress[1];
  crcBuffer[2] = baseAddress[0];

  cursor = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0 && cursor < COMMAND_LENGHT - 1){
    //setup serial commands interface
    commandBuffer[cursor] = Serial.read();
    
    if (commandBuffer[cursor] == '\n'){
      commandBuffer[cursor + 1] = "\0";
      parseCommand(commandBuffer);
      cursor = 0;
      memset(commandBuffer,' ',sizeof(commandBuffer));
    } else {
      cursor ++;
    }
  }
}

void parseCommand(char *commandtext) {
  int args[] = {0,0,0};
  char *arg;
  char *command = strsep(&commandtext," ");
  
  if (strcmp(command,"send") == 0){
    Serial.println("commandSend");
    if (((arg = strsep(&commandtext," ")) != NULL)){
      if (sscanf(arg,"%d",&args[0]) == 1) {
        sendResponse(0x30+args[0],clickerAdress);
      } else {
        Serial.println("argMustBeNumber");
      }
    } else {
      Serial.println("argRequired");
    }
  } else if (strcmp(command, "channel") == 0) {
    Serial.println("commandChannel");
    if (((arg = strsep(&commandtext," ")) != NULL)){
      if (sscanf(arg,"%d",&args[0]) == 1) {
        CHANNEL = args[0];
        radio.setChannel(CHANNEL);
      } else {
        Serial.println("argMustBeNumber");
        return;
      }
    } else {
      Serial.println("argRequired");
      return;
    }
  } else if (strcmp(command, "adress") == 0) {
    Serial.println("commandAdress");
    for (int i = 0; i < 3; i++){
      arg = strsep(&commandtext," ");
      if (arg == NULL){
        Serial.print("3argsRequired");
        return;
      } else if (sscanf(arg,"%d",&args[0]) == 1) {
        //nothing here
      } else {
        Serial.println("argsMustBeNumber");
        return;
      }
    }

    for (int i = 0; i<3; i++){
      clickerAdress[i] = args[i];
    }
  } else {
    Serial.println("unknown");
  }
}

uint16_t generateCheckSum(byte *data){
  memcpy(&crcBuffer[ADDR_WIDTH], data, ADDR_WIDTH + 1);
  uint16_t checksum = CRC16.ccitt(crcBuffer, 7);

  return checksum;
}

void sendResponse(byte b, uint8_t *address) {
  outgoingData[0] = address[0];
  outgoingData[1] = address[1];
  outgoingData[2] = address[2];
  outgoingData[3] = b;
  
  uint16_t checksum = generateCheckSum(outgoingData);
  outgoingData[4] = (checksum >> 8) & 0xFF;
  outgoingData[5] = checksum & 0xFF;

  Serial.print("send ");
  for (int i = 0; i < BUFSIZE; i++) {
    Serial.print(outgoingData[i],16);
  }
  Serial.println();
  
  radio.write(outgoingData, BUFSIZE);
  Serial.println("done");
}
