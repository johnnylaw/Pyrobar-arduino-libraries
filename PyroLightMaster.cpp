#include <Wire.h>
#include "PyroLightMaster.h"
#include "PyroLightController.h"

PyroLightMaster::PyroLightMaster(PyrobarLightValueMap lightMap) : _lastCyclePosition(0.0), _lightMap(lightMap) {
}

void PyroLightMaster::sendBufferPositions(uint8_t freqBfrPos, uint8_t sndBfrPos) {
  Wire.beginTransmission(1);
  Wire.write(freqBfrPos);
  Wire.write(sndBfrPos);
  Wire.endTransmission();
}

void PyroLightMaster::calculateBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos) {
  calculateFrequencyBufferPosition(freqBfrPos);
  calculateSoundBufferPosition(sndBfrPos);
}

// private
void PyroLightMaster::calculateFrequencyBufferPosition(uint8_t *freqBfrPos) {
  float currentMillis = millis();
  float timeElapsed = currentMillis - _lastMillis;
  _lastMillis = currentMillis;
  _lastCyclePosition += (timeElapsed * _lightMap.frequency());
  _lastCyclePosition = fmod(_lastCyclePosition, 1.0);
  *freqBfrPos = (uint8_t)(_lastCyclePosition * BFR_SZ_FREQ);
}

void PyroLightMaster::calculateSoundBufferPosition(uint8_t *sndBfrPos) {
  *sndBfrPos = 0;
}
