#ifndef _PYRO_LIGHT_MASTER_H
#define _PYRO_LIGHT_MASTER_H

#include <Arduino.h>
#include "PyrobarLightValueMap.h"

#define BASE_I2C_ADDRESS 16 // should include in something shared by master and slave

class PyrobarLightMaster {
public:
  PyrobarLightMaster(PyrobarLightValueMap *lightMap, uint8_t *ledPins);

private:
  float _lastCyclePosition;
  unsigned long _lastMillis;
  PyrobarLightValueMap *_lightMap;
  int _numberOfSlaves;
  uint8_t *_ledPins;

public:
  void calculateBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos);
  void sendLightValues(uint8_t freqBfrPos, uint8_t sndBfrPos);
  void begin(void);

private:
  void calculateFrequencyBufferPosition(uint8_t *freqBfrPos);
  void calculateSoundBufferPosition(uint8_t *sndBfrPos);
};

#endif