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

#define ON true
#define OFF false

#endif