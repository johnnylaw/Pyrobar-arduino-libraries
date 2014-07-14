#include "PyrobarLightValueMap.h"

PyrobarLightValueMap::PyrobarLightValueMap() : _frequency((float)DEFAULT_FREQUENCY / 1000.0), _soundSensitivity(0.0) {
}

void PyrobarLightValueMap::write(String type, int zone, int index, int color, uint8_t value) {
  if (type == pyrobarBfrTypeFreq) {
    _freqBfrs[zone][index][color] = value;
  } else if (type == pyrobarBfrTypeSnd) {
    _sndBfrs[zone][index][color] = value;
  }
}

uint8_t PyrobarLightValueMap::read(String type, int zone, int index, int color) {
  if (type == pyrobarBfrTypeFreq) {
    return _freqBfrs[zone][index][color];
  } else if (type == pyrobarBfrTypeSnd) {
    return _sndBfrs[zone][index][color];
  } else {
    return 0;
  }
}

void PyrobarLightValueMap::setFrequency(float frequency) {
  _frequency = frequency / 1000.0;
}

float PyrobarLightValueMap::frequency(void) {
  return _frequency;
}

void PyrobarLightValueMap::setSoundSensitivity(float soundSensitivity) {
  _soundSensitivity = soundSensitivity;
}

float PyrobarLightValueMap::soundSensitivity(void) {
  return _soundSensitivity;
}