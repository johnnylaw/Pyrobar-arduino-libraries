#ifndef _PYRO_LIGHT_MASTER_H
#define _PYRO_LIGHT_MASTER_H

#include <Arduino.h>
#include "PyrobarLightValueMap.h"

class PyroLightMaster {
public:
  PyroLightMaster(PyrobarLightValueMap lightMap);

private:
  float _lastCyclePosition;
  unsigned long _lastMillis;
  PyrobarLightValueMap _lightMap;

public:
  void calculateBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos);
  void sendBufferPositions(uint8_t freqBfrPos, uint8_t sndBfrPos);

private:
  void calculateFrequencyBufferPosition(uint8_t *freqBfrPos);
  void calculateSoundBufferPosition(uint8_t *sndBfrPos);
};

#endif