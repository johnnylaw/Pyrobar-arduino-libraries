#include "PyrobarConstants.h"
#include "PyrobarLightMap.h"

PyrobarLightMap::PyrobarLightMap() : _frequency((float)DEFAULT_FREQUENCY / 1000.0), _soundSensitivity(1.0), _mainOff(true) {
  resetWritePtr();
}

bool PyrobarLightMap::write(String type, int zone, uint8_t value) {
  int bfrLength;
  if (zone != _bfrWritePtr[0] || _bfrWritePtr[2] >= COLOR_COUNT) {
    resetWritePtr();
  }
  _bfrWritePtr[0] = zone;
  int index = _bfrWritePtr[1];
  int color = _bfrWritePtr[2];
  if (color >= COLOR_COUNT) {
    //Serial.print("Returning early because of color equal to ");
    //Serial.print(color);
    return false;
  }
  if (type == pyrobarBfrTypeFreq) {
    if (DEBUG_LIGHT_MAP && color == 0) {
      //Serial.print("Writing frequency buffer at address ");
      //Serial.print((long)(&_freqBfrs[0][0][0]));
      //Serial.print(" at zone ");
      //Serial.print(zone);
      //Serial.print(", index ");
      //Serial.print(index);
      //Serial.print(", color ");
      //Serial.print(color);
      //Serial.print(", value ");
      //Serial.println(value);
    }
    _freqBfrs[zone][index][color] = value;
    advanceWritePtr(BFR_SZ_FREQ);
  } else if (type == pyrobarBfrTypeSnd) {
    if (DEBUG_LIGHT_MAP && color == 0) {
      //Serial.print("Writing sound buffer at address ");
      //Serial.print((long)(&_sndBfrs[0][0][0]));
      //Serial.print(" at zone ");
      //Serial.print(zone);
      //Serial.print(", index ");
      //Serial.print(index);
      //Serial.print(", color ");
      //Serial.print(color);
      //Serial.print(", value ");
      //Serial.println(value);
    }
    _sndBfrs[zone][index][color] = value;
    advanceWritePtr(BFR_SZ_SND);
  } else {
    //Serial.print("Unknown type (in PyrobarLightMap.cpp): ");
    //Serial.println(type);
  }
  return true;
}

bool PyrobarLightMap::writeHexString(String type, int zone, String hexString) {

}

void PyrobarLightMap::resetWritePtr(void) {
  _bfrWritePtr[1] = _bfrWritePtr[2] = 0;
}
//
void PyrobarLightMap::advanceWritePtr(int bfrLength) {
  if (_bfrWritePtr[1] == bfrLength - 1) {
    _bfrWritePtr[2]++;
    _bfrWritePtr[1] = 0;
  } else {
    _bfrWritePtr[1]++;
  }
}

uint8_t PyrobarLightMap::read(String type, int zone, int index, int color) {
  if (type == pyrobarBfrTypeFreq) {
    if (DEBUG_LIGHT_MAP && zone == 0) {
      //Serial.print("Reading frequency buffer at address ");
      //Serial.print((long)(&_freqBfrs[0][0][0]));
      //Serial.print(" for zone ");
      //Serial.print(zone);
      //Serial.print(", index ");
      //Serial.print(index);
      //Serial.print(", color ");
      //Serial.print(color);
      //Serial.print(". Returning ");
      //Serial.println(_freqBfrs[zone][index][color]);
    }
    return _freqBfrs[zone][index][color];
  } else if (type == pyrobarBfrTypeSnd) {
    return _sndBfrs[zone][index][color];
  } else {
    //Serial.print("No buffer of type '");
    //Serial.print(type);
    //Serial.print("'. Choices are '");
    //Serial.print(pyrobarBfrTypeFreq);
    //Serial.print("' and '");
    //Serial.print(pyrobarBfrTypeSnd);
    //Serial.println("'.");
    return 0;
  }
}

float PyrobarLightMap::frequency(void) {
  return _frequency;
}

float PyrobarLightMap::soundSensitivity(void) {
  return _soundSensitivity;
}

bool PyrobarLightMap::setScalar(String type, float value) {
  if (value <= 0.0 || value > 2.0) return false;
  if (type == pyrobarScalarTypeFrequency) {
    _frequency = value / 1000.0;
    if (DEBUG_LIGHT_MAP) {
      //Serial.print("Frequency: ");
      //Serial.println(_frequency);
    }
  } else if (type == pyrobarScalarTypeSoundSensitivity) {
    _soundSensitivity = value;
    if (DEBUG_LIGHT_MAP) {
      //Serial.print("Sound sensitivity: ");
      //Serial.print(_soundSensitivity);
    }
  } else {
    return false;
  }
  return true;
}

void PyrobarLightMap::turnLights(String type, bool on) {
  bool _craneSpotLightsOn;
  bool _aerialSpotLightOn;
  if (type == pyrobarDataTypeMainLights) {
    _mainOff = !on;
  } else if (type == pyrobarDataTypeAerialSpotlight) {
    _aerialSpotLightOn = !on;
  } else if (type == pyrobarDataTypeCraneSpotlights) {
    _craneSpotLightsOn = !on;
  }
}

bool PyrobarLightMap::shouldDisplay(void) {
  return !_mainOff;
}

bool PyrobarLightMap::lightIsOn(String type) {
  if (type == pyrobarDataTypeCraneSpotlights) {
    return _aerialSpotLightOn;
  } else if (type == pyrobarDataTypeAerialSpotlight) {
    return _craneSpotLightsOn;
  }
}
