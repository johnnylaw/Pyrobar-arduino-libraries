#include <Wire.h>
#include "PyrobarConstants.h"
#include "PyrobarLightMaster.h"

PyrobarLightMaster::PyrobarLightMaster(PyrobarLightMap *lightMap, PyrobarPulseLightSet *pulseLightSet, uint8_t *ledPins) : _lastCyclePosition(0.0), _lightMap(lightMap), _pulseLightSet(pulseLightSet), _ledPins(ledPins) {
  _numberOfSlaves = (TOTAL_ZONE_COUNT - 1) / ZONES_PER_SLAVE_BOARD;
}

void PyrobarLightMaster::sendLightProgramInfo(uint8_t freqBfrPos, uint8_t sndBfrPos) {
  // go thru slave boards, write ZONES_PER_SLAVE_BOARD * COLOR_COUNT values to Wire for each
  uint8_t value;
  // Write zones on slave boards
  for (unsigned char board = 0; board < _numberOfSlaves; board++) {
    Wire.beginTransmission(BASE_I2C_ADDRESS + board);
    if (DEBUG_LIGHT_OUTPUT) {
      Serial.print("Transmitting to ");
      Serial.println(BASE_I2C_ADDRESS + board);
    }
    for (unsigned char boardZone = 0; boardZone < ZONES_PER_SLAVE_BOARD; boardZone++) {
      unsigned char zoneIndex = board * ZONES_PER_SLAVE_BOARD + boardZone;
      for (unsigned char color = 0; color < COLOR_COUNT; color++) {
        if (_lightMap->shouldDisplay()) {
          uint8_t freqValue = _lightMap->read(pyrobarBfrTypeFreq, zoneIndex, freqBfrPos, color);
          uint8_t sndValue = _lightMap->read(pyrobarBfrTypeSnd, zoneIndex, sndBfrPos, color);
          value = max(freqValue, sndValue);
        } else {
          value = _pulseLightSet->read(zoneIndex, color);
        }
        if (DEBUG_LIGHT_OUTPUT && boardZone == 0) {
          Serial.println(value);
        }
        Wire.write(value);
      }
    }
    Wire.endTransmission();
  }

  // Write zones that are on master board
  for (int zone = ZONES_PER_SLAVE_BOARD * _numberOfSlaves; zone < TOTAL_ZONE_COUNT; zone++) {
    int localZoneIndex = zone % ZONES_PER_SLAVE_BOARD;
    for (int color = 0; color < COLOR_COUNT; color++) {
      if (_lightMap->shouldDisplay()) {
        uint8_t freqValue = _lightMap->read(pyrobarBfrTypeFreq, zone, freqBfrPos, color);
        uint8_t sndValue = _lightMap->read(pyrobarBfrTypeSnd, zone, sndBfrPos, color);
        value = max(freqValue, sndValue);
      } else {
        value = _pulseLightSet->read(zone, color);
      }
      analogWrite(_ledPins[localZoneIndex * COLOR_COUNT + color], value);
    }
  }
}

void PyrobarLightMaster::begin(void) {
  Wire.begin();
  for (int i = 0; i < sizeof(_ledPins); i++) {
    pinMode(_ledPins[i], OUTPUT);
  }
}

void PyrobarLightMaster::calculateBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos) {
  calculateFrequencyBufferPosition(freqBfrPos);
  calculateSoundBufferPosition(sndBfrPos);
}

// private
void PyrobarLightMaster::calculateFrequencyBufferPosition(uint8_t *freqBfrPos) {
  float currentMillis = millis();
  float timeElapsed = currentMillis - _lastMillis;
  _lastMillis = currentMillis;
  _lastCyclePosition += (timeElapsed * _lightMap->frequency());
  _lastCyclePosition = fmod(_lastCyclePosition, 1.0);
  *freqBfrPos = (uint8_t)(_lastCyclePosition * BFR_SZ_FREQ);
}

void PyrobarLightMaster::calculateSoundBufferPosition(uint8_t *sndBfrPos) {
  *sndBfrPos = 0;
}
