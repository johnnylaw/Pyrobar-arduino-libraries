#ifndef _PYRO_LIGHT_MASTER_H
#define _PYRO_LIGHT_MASTER_H

#include <Arduino.h>
#include "PyrobarLightMap.h"
#include "PyrobarPulseLightSet.h"

#define BASE_I2C_ADDRESS 0x10 // should include in something shared by master and slave

class PyrobarLightMaster {
public:
  PyrobarLightMaster(PyrobarLightMap *lightMap, PyrobarPulseLightSet *pulseLightSet, uint8_t soundLevelPin, uint8_t aerialSpotLightPin, uint8_t craneSpotLightPin);

private:
  float _lastCyclePosition;
  unsigned long _lastMillis;
  PyrobarLightMap *_lightMap;
  PyrobarPulseLightSet *_pulseLightSet;
  int _numberOfSlaves;
  uint8_t _aerialSpotLightPin;
  uint8_t _craneSpotLightPin;
  uint8_t _soundLevelPin;
  uint16_t _stripBuffer[120];

public:
  void calculateBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos);
  void sendLightProgramInfo(uint8_t freqBfrPos, uint8_t sndBfrPos);
  void begin(void);

private:
  void calculateFrequencyBufferPosition(uint8_t *freqBfrPos);
  void calculateSoundBufferPosition(uint8_t *sndBfrPos);
};

#endif