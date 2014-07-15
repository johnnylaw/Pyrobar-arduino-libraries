#define TOTAL_ZONE_COUNT 9

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Wire.h>
#include "PyrobarFireCannon.h"
#include "PyrobarLightMaster.h"
#include "PyrobarRequestHandler.h"
#include "PyrobarFireController.h"

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(10, 1, 10, 101);
unsigned int localPortUDP = 8888;

EthernetUDP Udp;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

uint8_t firePins[] = {25, 26, 27};
PyrobarFireCannon fireCannons[] = {PyrobarFireCannon(firePins[0]), PyrobarFireCannon(firePins[1]), PyrobarFireCannon(firePins[2])};
const int NUM_FIRE_CANNONS = sizeof(fireCannons) / sizeof(PyrobarFireCannon);
static PyrobarLightValueMap lightMap = PyrobarLightValueMap();
static PyrobarLightMaster MasterCtrl = PyrobarLightMaster(lightMap);
static PyrobarRequestHandler PBRequestHandler = PyrobarRequestHandler(lightMap);
static PyrobarFireSequence fireSequence = PyrobarFireSequence();
static PyrobarFireController FireCtrl = PyrobarFireController(NUM_FIRE_CANNONS, firePins, fireSequence);

uint8_t freqBfrPos;
uint8_t sndBfrPos;

EthernetServer server(80);

void setup() {
  Ethernet.begin(mac, ip);
  Udp.begin(localPortUDP);
}

void loop() {
  if(!FireCtrl.play()) {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
      // parse packetBuffer info for cannon #, start/kill bit and duration if start
    }
    if (EthernetClient client = server.available()) {
      killCannons();
      PBRequestHandler.handleRequest(client);
    } else {
      adjustCannons();
    }
  }

  MasterCtrl.calculateBufferPositions(&freqBfrPos, &sndBfrPos);
  MasterCtrl.sendLightValues(freqBfrPos, sndBfrPos);
}

void adjustCannons() {
  for (int i = 0; i < NUM_FIRE_CANNONS; i++) {
    fireCannons[i].adjust();
  }
}

void killCannons() {
  for (int i = 0; i < NUM_FIRE_CANNONS; i++) {
    fireCannons[i].kill();
  }
}

