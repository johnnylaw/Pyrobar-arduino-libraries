#include "PyrobarConstants.h"
#include "PyrobarUDPRequestHandler.h"

PyrobarUDPRequestHandler::PyrobarUDPRequestHandler(PyrobarLightValueMap *lightMap, PyrobarPulseLightSet *pulseLightSet) : _lightMap(lightMap), _pulseLightSet(pulseLightSet) {
}

void PyrobarUDPRequestHandler::handleRequest(unsigned char *buffer, int length) {
  float decay = 0;
  switch (buffer[0]) {
    case UDP_FIRE_ON:
      // zone
      // duration
      break;
    case UDP_FIRE_OFF:
      // zone
      break;
    case UDP_PULSE_LIGHT_ON:
      _lightMap->turnLights(OFF);
      if (length == 6) decay = buffer[5] * 8;
      if (DEBUG_UDP) {
        Serial.print("PULSING LIGHT at zone ");
        Serial.print(buffer[1]);
        Serial.print(", RGB: ");
        Serial.print(buffer[2]);
        Serial.print(", ");
        Serial.print(buffer[3]);
        Serial.print(", ");
        Serial.print(buffer[4]);
        Serial.print(", DECAY: ");
        Serial.println(decay);
      }
      _pulseLightSet->pulse(buffer[1], buffer[2], buffer[3], buffer[4], decay);
      break;
    case UDP_PULSE_LIGHTS_DECAY_ON:
      _pulseLightSet->startDecayAll();
      break;
    default:
      break;
  }
}