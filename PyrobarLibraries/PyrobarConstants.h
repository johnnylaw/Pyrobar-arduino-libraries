#ifndef _PYROBAR_CONSTANTS_H
#define _PYROBAR_CONSTANTS_H

#include <Arduino.h>

typedef struct SlaveZoneAddresses {
  uint8_t low, high;
} SlaveZoneAddresses;

#define BASE_I2C_ADDRESS 0x10
const uint8_t numberOfSlaves = 2;
const SlaveZoneAddresses slaveZoneAddresses[2] = {{0, 8}, {9, 10}};

#define _DEBUG true
#define DEBUG_REQUEST_HANDLER false
#define DEBUG_LIGHT_MAP false
#define DEBUG_UDP false
#define DEBUG_UDP_FIRE false
#define DEBUG_HTTP false
// #define DEBUG_LIGHT_OUTPUT
// #define DEBUG_LIGHT_VALUE_OUTPUT
#define DEBUG_PULSE_LIGHT false
#define DEBUG_FIRE_SEQUENCE false
#define DEBUG_SOUND_LEVEL false

#define UDP_TX_PACKET_MAX_SIZE 8
#define TOTAL_ZONE_COUNT 11
#define COLOR_COUNT 3
#define CANNON_COUNT 3

#define UDP_FIRE_ON 0
#define UDP_FIRE_OFF 1
#define UDP_PULSE_LIGHT_ON 2
#define UDP_PULSE_LIGHTS_DECAY_ON 3
#define UDP_FREQUENCY_SYNC 4
#define UDP_LIGHT_BALL 5

typedef enum udpMode {
  FIRE_ON, FIRE_OFF, PULSE_LIGHT_ON, PULSE_LIGHTS_DECAY_ON, FREQUENCY_SYNC, LIGHT_BALL
} UDPMode;

#define MAX_FIRE_DURATION 5000  // ms

const float minIncomingSoundLevelValue = 185.0; // ~0.6V
const float maxIncomingSoundLevelValue = 775.0; // ~2.5V
const float soundLevelRange = maxIncomingSoundLevelValue - minIncomingSoundLevelValue;

#define ON true
#define OFF false

typedef enum lightMode {
  PROGRAM, BALL_DRAG
} LightMode;

#endif