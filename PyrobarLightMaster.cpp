#include <Wire.h>
#include "PyrobarConstants.h"
#include "PyrobarLightMaster.h"

PyrobarLightMaster::PyrobarLightMaster(PyrobarLightValueMap lightMap, uint8_t *ledPins) : _lastCyclePosition(0.0), _lightMap(lightMap), _ledPins(ledPins) {
  _numberOfSlaves = (_lightMap.zoneCount() - 1) / ZONES_PER_SLAVE_BOARD + 1;
}

void PyrobarLightMaster::sendLightValues(uint8_t freqBfrPos, uint8_t sndBfrPos) {
  // go thru slave boards, write ZONES_PER_SLAVE_BOARD * COLOR_COUNT values to Wire for each
  uint8_t aggregateValue;

  for (unsigned char board = 0; board < _numberOfSlaves; board++) {
    Wire.beginTransmission(BASE_I2C_ADDRESS + board);
    for (unsigned char boardZone = 0; boardZone < ZONES_PER_SLAVE_BOARD; boardZone++) {
      for (unsigned char color = 0; color < COLOR_COUNT; color++) {
        if (_lightMap.shouldDisplay()) {
          unsigned char zoneIndex = board * ZONES_PER_SLAVE_BOARD + boardZone;
          uint8_t freqValue = _lightMap.read(pyrobarBfrTypeFreq, zoneIndex, freqBfrPos, color);
          uint8_t sndValue = _lightMap.read(pyrobarBfrTypeSnd, zoneIndex, sndBfrPos, color);
          aggregateValue = max(freqValue, sndValue);
        } else {
          aggregateValue = 0;
        }
        Wire.write(aggregateValue);
      }
    }
    Wire.endTransmission();
  }

  // Write zones that are on master board
  for (int zone = ZONES_PER_SLAVE_BOARD * _numberOfSlaves; zone < _lightMap.zoneCount(); zone++) {
    for (int color = 0; color < COLOR_COUNT; color++) {
      int localZoneIndex = zone % ZONES_PER_SLAVE_BOARD;
      if (_lightMap.shouldDisplay()) {
        uint8_t freqValue = _lightMap.read(pyrobarBfrTypeFreq, zone, freqBfrPos, color);
        uint8_t sndValue = _lightMap.read(pyrobarBfrTypeSnd, zone, sndBfrPos, color);
        aggregateValue = max(freqValue, sndValue);
      } else {
        aggregateValue = 0;
      }
      analogWrite(_ledPins[localZoneIndex * COLOR_COUNT + color], aggregateValue);
    }
  }
}

void PyrobarLightMaster::begin(void) {
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
  _lastCyclePosition += (timeElapsed * _lightMap.frequency());
  _lastCyclePosition = fmod(_lastCyclePosition, 1.0);
  *freqBfrPos = (uint8_t)(_lastCyclePosition * BFR_SZ_FREQ);
}

void PyrobarLightMaster::calculateSoundBufferPosition(uint8_t *sndBfrPos) {
  *sndBfrPos = 0;
}
