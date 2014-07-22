#include <SPI.h>
#include <Ethernet.h>
//#include <EthernetUdp.h>
#include <Wire.h>
#include "PyrobarConstants.h"
#include "PyrobarFireCannon.h"
#include "PyrobarLightMaster.h"
#include "PyrobarRequestHandler.h"
#include "PyrobarFireController.h"

#define UDP_FIRE_ON 0
#define UDP_FIRE_OFF 1
#define UDP_PULSE_LIGHT_ON 2
#define UDP_PULSE_LIGHTS_DECAY_ON 3

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 101);
unsigned int localPortUDP = 8888;

EthernetUDP Udp;

#define UDP_TX_PACKET_MAX_SIZE 12
unsigned char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

uint8_t firePins[] = {25, 26, 27};
uint8_t ledPins[] = {2, 3, 4, 5, 6, 7};

PyrobarFireCannon fireCannons[] = {PyrobarFireCannon(firePins[0]), PyrobarFireCannon(firePins[1]), PyrobarFireCannon(firePins[2])};
const int NUM_FIRE_CANNONS = sizeof(fireCannons) / sizeof(PyrobarFireCannon);
static PyrobarLightValueMap lightMap = PyrobarLightValueMap();
static PyrobarLightMaster MasterCtrl = PyrobarLightMaster(lightMap, ledPins);
static PyrobarRequestHandler PBRequestHandler = PyrobarRequestHandler(&lightMap);
static PyrobarFireSequence fireSequence = PyrobarFireSequence();
static PyrobarFireController FireCtrl = PyrobarFireController(NUM_FIRE_CANNONS, firePins, fireSequence);

uint8_t freqBfrPos;
uint8_t sndBfrPos;

EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  FireCtrl.begin();
  Udp.begin(localPortUDP);
  Serial.println(Ethernet.localIP());
  pinMode(38, INPUT);
  
  if (DEBUG_LIGHT_MAP) {
    Serial.print("Light map in main at address ");
    Serial.println((long)&lightMap);
  }
}

void loop() {
  if(!FireCtrl.play()) {
    if (int packetSize = Udp.parsePacket()) {
      if (DEBUG_UDP) {
        Serial.print("UDP packet received with packet size: ");
        Serial.println(packetSize);
      }
      Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
      if (DEBUG_UDP) {
        for (int i = 0; i < packetSize; i++) Serial.println(packetBuffer[i]);
      }
      // parse packetBuffer info for cannon #, start/kill bit and duration if start
    }
    if(digitalRead(38) && _DEBUG) {
      printDiagnostics();
    }
    if (EthernetClient client = server.available()) {
      killCannons();
      if (_DEBUG) Serial.println("\nClient exists!");
      PBRequestHandler.handleHTTPRequest(client);
    } else {
      adjustCannons();
    }
  }

//  MasterCtrl.calculateBufferPositions(&freqBfrPos, &sndBfrPos);
//  MasterCtrl.sendLightValues(freqBfrPos, sndBfrPos);
  delay(50);
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

void printDiagnostics() {
  Serial.println("\nFrequency buffer values (every 4th):");
  for(int ptr = 0; ptr < 256; ptr += 4) {
    for(int zone = 0; zone < TOTAL_ZONE_COUNT; zone++) {
      Serial.print("(");
      Serial.print(lightMap.read("frequency", zone, ptr, 0));
      Serial.print(", ");
      Serial.print(lightMap.read("frequency", zone, ptr, 1));
      Serial.print(", ");
      Serial.print(lightMap.read("frequency", zone, ptr, 2));
      Serial.print(")\t");
    }
    Serial.println();
  }
  Serial.println("\nSound buffer values:");
  for(int ptr = 0; ptr < 16; ptr++) {
    for(int zone = 0; zone < TOTAL_ZONE_COUNT; zone++) {
      Serial.print("(");
      Serial.print(lightMap.read("sound", zone, ptr, 0));
      Serial.print(", ");
      Serial.print(lightMap.read("sound", zone, ptr, 1));
      Serial.print(", ");
      Serial.print(lightMap.read("sound", zone, ptr, 2));
      Serial.print(")\t");
    }
    Serial.println();
  }  
  
  Serial.print("Sound senstivity: ");
  Serial.print(lightMap.soundSensitivity());
  Serial.print(", Frequency: ");
  Serial.println(lightMap.frequency() * 1000.0);
}

