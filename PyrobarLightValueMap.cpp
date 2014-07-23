#include "PyrobarConstants.h"
#include "PyrobarLightValueMap.h"

PyrobarLightValueMap::PyrobarLightValueMap() : _frequency((float)DEFAULT_FREQUENCY / 1000.0), _soundSensitivity(0.0), _allOff(true) {
  resetWritePtr();
}

bool PyrobarLightValueMap::write(String type, int zone, uint8_t value) {
  _allOff = false;
  int bfrLength;
  if (zone != _bfrWritePtr[0] || _bfrWritePtr[2] >= COLOR_COUNT) {
    resetWritePtr();
  }
  _bfrWritePtr[0] = zone;
  int index = _bfrWritePtr[1];
  int color = _bfrWritePtr[2];
  if (color >= COLOR_COUNT) {
    Serial.print("Returning early because of color equal to ");
    Serial.print(color);
    return false;
  }
  if (type == pyrobarBfrTypeFreq) {
    if (DEBUG_LIGHT_MAP && color == 0) {
      Serial.print("Writing frequency buffer at address ");
      Serial.print((long)(&_freqBfrs[0][0][0]));
      Serial.print(" at zone ");
      Serial.print(zone);
      Serial.print(", index ");
      Serial.print(index);
      Serial.print(", color ");
      Serial.print(color);
      Serial.print(", value ");
      Serial.println(value);
    }
    _freqBfrs[zone][index][color] = value;
    advanceWritePtr(BFR_SZ_FREQ);
  } else if (type == pyrobarBfrTypeSnd) {
    if (DEBUG_LIGHT_MAP && color == 0) {
      Serial.print("Writing sound buffer at address ");
      Serial.print((long)(&_sndBfrs[0][0][0]));
      Serial.print(" at zone ");
      Serial.print(zone);
      Serial.print(", index ");
      Serial.print(index);
      Serial.print(", color ");
      Serial.print(color);
      Serial.print(", value ");
      Serial.println(value);
    }
    _sndBfrs[zone][index][color] = value;
    advanceWritePtr(BFR_SZ_SND);
  } else {
    Serial.print("Unknown type (in PyrobarLightValueMap.cpp): ");
    Serial.println(type);
  }
  return true;
}

bool PyrobarLightValueMap::writeHexString(String type, int zone, String hexString) {

}

void PyrobarLightValueMap::resetWritePtr(void) {
  _bfrWritePtr[1] = _bfrWritePtr[2] = 0;
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
    if (DEBUG_LIGHT_MAP && zone == 0) {
      Serial.print("Reading frequency buffer at address ");
      Serial.print((long)(&_freqBfrs[0][0][0]));
      Serial.print(" for zone ");
      Serial.print(zone);
      Serial.print(", index ");
      Serial.print(index);
      Serial.print(", color ");
      Serial.print(color);
      Serial.print(". Returning ");
      Serial.println(_freqBfrs[zone][index][color]);
    }
    return _freqBfrs[zone][index][color];
  } else if (type == pyrobarBfrTypeSnd) {
    return _sndBfrs[zone][index][color];
  } else {
    Serial.print("No buffer of type '");
    Serial.print(type);
    Serial.print("'. Choices are '");
    Serial.print(pyrobarBfrTypeFreq);
    Serial.print("' and '");
    Serial.print(pyrobarBfrTypeSnd);
    Serial.println("'.");
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
    if (DEBUG_LIGHT_MAP) {
      Serial.print("Frequency: ");
      Serial.println(_frequency);
    }
  } else if (type == pyrobarScalarTypeSoundSensitivity) {
    _soundSensitivity = value;
    if (DEBUG_LIGHT_MAP) {
      Serial.print("Sound sensitivity: ");
      Serial.print(_soundSensitivity);
    }
  } else {
    return false;
  }
  return true;
}

void PyrobarLightValueMap::turnLights(bool on) {
  _allOff = !on;
}

bool PyrobarLightValueMap::shouldDisplay(void) {
  return !_allOff;
}

int PyrobarLightValueMap::zoneCount(void) {
  return TOTAL_ZONE_COUNT;
}
