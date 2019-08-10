#ifndef _PYRO_LIGHT_MASTER_H
#define _PYRO_LIGHT_MASTER_H

#include <Arduino.h>
#include "PyrobarLightMap.h"
#include "PyrobarPulseLightSet.h"

class PyrobarLightMaster {
public:
  PyrobarLightMaster(PyrobarLightMap *lightMap, PyrobarPulseLightSet *pulseLightSet, uint8_t soundLevelPin, uint8_t aerialSpotLightPin, uint8_t craneSpotLightPin);

private:
  float _lastCyclePosition;
  unsigned long _lastMillis;
  PyrobarLightMap *_lightMap;
  PyrobarPulseLightSet *_pulseLightSet;
  uint8_t _aerialSpotLightPin;
  uint8_t _craneSpotLightPin;
  uint8_t _soundLevelPin;

public:
  void calculateBufferPositions(uint8_t *freqBfrPos);
  void sendLightProgramInfo(uint8_t freqBfrPos);
  void begin(void);

private:
  void sendSlaveInfo(uint8_t slaveInd, uint8_t freqBfrPos);
  void calculateFrequencyBufferPosition(uint8_t *freqBfrPos);
};

#endif