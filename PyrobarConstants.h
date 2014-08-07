#ifndef _PYROBAR_CONSTANTS_H
#define _PYROBAR_CONSTANTS_H

#define _DEBUG true
#define DEBUG_REQUEST_HANDLER false
#define DEBUG_LIGHT_MAP false
#define DEBUG_UDP false
#define DEBUG_UDP_FIRE false
#define DEBUG_HTTP false
#define DEBUG_LIGHT_OUTPUT false
#define DEBUG_PULSE_LIGHT false
#define DEBUG_FIRE_SEQUENCE false
#define DEBUG_SOUND_LEVEL false

#define UDP_TX_PACKET_MAX_SIZE 8
#define TOTAL_ZONE_COUNT 9
#define COLOR_COUNT 3
#define CANNON_COUNT 3

#define UDP_FIRE_ON 0
#define UDP_FIRE_OFF 1
#define UDP_PULSE_LIGHT_ON 2
#define UDP_PULSE_LIGHTS_DECAY_ON 3
#define UDP_FREQUENCY_SYNC 4

#define MAX_FIRE_DURATION 5000  // ms

#define MIN_INCOMING_SOUND_LEVEL_VALUE
#define MAX_INCOMING_SOUND_LEVEL_VALUE
const float minIncomingSoundLevelValue = 185.0; // ~0.6V
const float maxIncomingSoundLevelValue = 775.0; // ~2.5V
const float soundLevelRange = maxIncomingSoundLevelValue - minIncomingSoundLevelValue;

#define ON true
#define OFF false

#endif