#include "PyrobarConstants.h"
#include "PyrobarUDPRequestHandler.h"

PyrobarUDPRequestHandler::PyrobarUDPRequestHandler(PyrobarLightMap *lightMap, PyrobarPulseLightSet *pulseLightSet, PyrobarFireSequence *fireSequence) : _lightMap(lightMap), _pulseLightSet(pulseLightSet), _fireSequence(fireSequence) {
}

void PyrobarUDPRequestHandler::handleRequest(unsigned char *buffer, int length) {
  if (DEBUG_UDP) Serial.println("Handling UDP request");
  float decay = 0;
  switch (buffer[0]) {
    case UDP_FIRE_ON:
      _fireSequence->addNote(buffer[1], 0, buffer[2] * 256 + buffer[3]);

      if (DEBUG_UDP_FIRE) {
        Serial.print("ON cannon ");
        Serial.println(buffer[1]);
      }
      break;
    case UDP_FIRE_OFF:
      _fireSequence->kill(buffer[1]);

      if (DEBUG_UDP_FIRE) {
        Serial.print("OFF cannon ");
        Serial.println(buffer[1]);
      }
      break;
    case UDP_PULSE_LIGHT_ON:
      _lightMap->turnLights(pyrobarDataTypeMainLights, OFF);
      if (length == 6) decay = buffer[5];
      _pulseLightSet->pulse(buffer[1], buffer[2], buffer[3], buffer[4], decay);

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
      break;
    case UDP_PULSE_LIGHTS_DECAY_ON:
      _pulseLightSet->startDecayAll(buffer[1]);

      if (DEBUG_UDP) {
        Serial.print("Starting decay all: ");
        Serial.println(buffer[1]);
      }
      break;
    case UDP_LIGHT_BALL:  // need to write info to a buffer and send next loop
      _lightMap->turnLights(pyrobarDataTypeMainLights, OFF);
      break;
    default:
      break;
  }
}