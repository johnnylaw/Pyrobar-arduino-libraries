#include "PyrobarLightValueMap.h"

PyrobarLightValueMap::PyrobarLightValueMap() : _frequency((float)DEFAULT_FREQUENCY / 1000.0), _soundSensitivity(0.0) {
  resetWritePtr();
}

bool PyrobarLightValueMap::write(String type, int zone, uint8_t value) {
  int index = _bfrWritePtr[1];
  int color = _bfrWritePtr[2];
  int bfrLength;
  if (zone != _bfrWritePtr[0] || color >= COLOR_COUNT) {
    resetWritePtr();
  }
  if (color >= COLOR_COUNT) {
    return false;
  }
  if (type == pyrobarBfrTypeFreq) {
    _freqBfrs[zone][index][color] = value;
    advanceWritePtr(BFR_SZ_FREQ);
  } else if (type == pyrobarBfrTypeSnd) {
    _sndBfrs[zone][index][color] = value;
    advanceWritePtr(BFR_SZ_SND);
  }
  return true;
}

bool PyrobarLightValueMap::writeHexString(String type, int zone, String hexString) {

}

void PyrobarLightValueMap::resetWritePtr(void) {
  _bfrWritePtr = {0, 0, 0};
}

void PyrobarLightValueMap::advanceWritePtr(int bfrLength) {
  if (_bfrWritePtr[1] == bfrLength - 1) {
    _bfrWritePtr[2]++;
    _bfrWritePtr[1] = 0;
  } else {
    _bfrWritePtr[1]++;
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

float PyrobarLightValueMap::frequency(void) {
  return _frequency;
}

float PyrobarLightValueMap::soundSensitivity(void) {
  return _soundSensitivity;
}

bool PyrobarLightValueMap::setScalar(String type, float value) {
  if (value <= 0.0 || value > 2.0) return false;
  if (type == pyrobarScalarTypeFrequency) {
    _frequency = value / 1000.0;
  } else if (type == pyrobarScalarTypeSoundSensitivity) {
    _soundSensitivity = value;
  } else {
    return false;
  }
  return true;
}

int PyrobarLightValueMap::zoneCount(void) {
  return TOTAL_ZONE_COUNT;
}
