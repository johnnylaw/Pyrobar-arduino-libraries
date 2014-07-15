#ifndef _PYRO_LIGHT_MASTER_H
#define _PYRO_LIGHT_MASTER_H

#include <Arduino.h>
#include "PyrobarLightValueMap.h"

#define BASE_I2C_ADDRESS 16 // should include in something shared by master and slave

class PyrobarLightMaster {
public:
  PyrobarLightMaster(PyrobarLightValueMap lightMap);

private:
  float _lastCyclePosition;
  unsigned long _lastMillis;
  PyrobarLightValueMap _lightMap;
  int _numberOfSlaves;

public:
  void calculateBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos);
  void sendLightValues(uint8_t freqBfrPos, uint8_t sndBfrPos);

private:
  void calculateFrequencyBufferPosition(uint8_t *freqBfrPos);
  void calculateSoundBufferPosition(uint8_t *sndBfrPos);
};

#endif