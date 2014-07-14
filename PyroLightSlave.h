#ifndef _PYRO_LIGHT_SLAVE_H
#define _PYRO_LIGHT_SLAVE_H

#include <Arduino.h>
#include <Wire.h>

class PyroLightSlave {
public:
  PyroLightSlave();

public:
  void readBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos);
};

extern PyroLightSlave PyroSlave;

#endif

