#ifndef _PYRO_LIGHT_MASTER_H
#define _PYRO_LIGHT_MASTER_H

#include <Arduino.h>

class PyroLightMaster {
public:
  PyroLightMaster();

private:
  float _lastCyclePosition;
  unsigned long _lastMillis;
  float _frequency;

public:
  void calculateBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos);
  void sendBufferPositions(uint8_t freqBfrPos, uint8_t sndBfrPos);

private:
  void calculateFrequencyBufferPosition(uint8_t *freqBfrPos);
  void calculateSoundBufferPosition(uint8_t *sndBfrPos);
};

extern PyroLightMaster PyroMaster;

#endif