#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include "PyroLightMaster.h"
#include "PyroLightSlave.h"
#include "PyroLightController.h"

#define TOTAL_ZONE_COUNT 9

#define NUM_BOARD_ADDRESS_PINS 2
static uint8_t boardAddressPins[NUM_BOARD_ADDRESS_PINS] = {0, 1};
static uint8_t ledPins[MAX_ZONES_PER_BOARD][COLOR_COUNT] = {{2, 3, 4}, {5, 6, 7}, {8, 9, 10}, {11, 12, 13}};

static PyroLightController LightCtrl = PyroLightController(NUM_BOARD_ADDRESS_PINS, boardAddressPins, ledPins, TOTAL_ZONE_COUNT);

EthernetServer server(80);

uint8_t freqBfrPos;
uint8_t sndBfrPos;

void setup() {
  Serial.begin(9600);
  
  IPAddress ip(10, 1, 10, 100 + LightCtrl.boardNumber());
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE8 + LightCtrl.boardNumber() };
  Ethernet.begin(mac, ip);
  
  LightCtrl.begin();
  Serial.println(Ethernet.localIP());
  if (!LightCtrl.isMaster()) {
    Wire.onReceive(receiveBfrPositions);
  }
}

void loop() {
  if(LightCtrl.isMaster()) {
      PyroMaster.calculateBufferPositions(&freqBfrPos, &sndBfrPos);
      PyroMaster.sendBufferPositions(freqBfrPos, sndBfrPos);
  }
  LightCtrl.setLEDs(freqBfrPos, sndBfrPos);
}

void receiveBfrPositions(int howMany) {
  PyroSlave.readBufferPositions(&freqBfrPos, &sndBfrPos);
}
