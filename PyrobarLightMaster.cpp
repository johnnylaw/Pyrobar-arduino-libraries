#include <Wire.h>
#include "PyrobarConstants.h"
#include "PyrobarLightMaster.h"

PyrobarLightMaster::PyrobarLightMaster(PyrobarLightMap *lightMap, PyrobarPulseLightSet *pulseLightSet, uint8_t soundLevelPin, uint8_t aerialSpotLightPin, uint8_t craneSpotLightPin) : _lastCyclePosition(0.0), _lightMap(lightMap), _pulseLightSet(pulseLightSet), _soundLevelPin(soundLevelPin), _aerialSpotLightPin(aerialSpotLightPin), _craneSpotLightPin(craneSpotLightPin) {
  _numberOfSlaves = (TOTAL_ZONE_COUNT - 1) / ZONES_PER_SLAVE_BOARD;
}

void PyrobarLightMaster::sendLightProgramInfo(uint8_t freqBfrPos, uint8_t sndBfrPos) {
  uint8_t value;
  Wire.beginTransmission(BASE_I2C_ADDRESS);

  if (DEBUG_LIGHT_OUTPUT) {
    Serial.print("Transmitting to ");
    Serial.println(BASE_I2C_ADDRESS);
  }

  for (int zoneInd = 0; zoneInd < TOTAL_ZONE_COUNT; zoneInd++) {
    for (int colorInd = 0; colorInd < COLOR_COUNT; colorInd++) {
      if (_lightMap->shouldDisplay()) {
        uint8_t freqValue = _lightMap->read(pyrobarBfrTypeFreq, zoneInd, freqBfrPos, colorInd);
        uint8_t sndValue = _lightMap->read(pyrobarBfrTypeSnd, zoneInd, sndBfrPos, colorInd);
        value = max(freqValue, sndValue);
      } else {
        value = _pulseLightSet->read(zoneInd, colorInd);
      }
      if (DEBUG_LIGHT_OUTPUT && zoneInd == 0) {
        Serial.println(value);
      }
      Wire.write(value);
    }
  }

  Wire.endTransmission();

  digitalWrite(_aerialSpotLightPin, _lightMap->lightIsOn(pyrobarDataTypeAerialSpotlight) ? HIGH : LOW);
  digitalWrite(_craneSpotLightPin, _lightMap->lightIsOn(pyrobarDataTypeCraneSpotlights) ? HIGH : LOW);
}

void PyrobarLightMaster::begin(void) {
  Wire.begin();
  pinMode(_soundLevelPin, INPUT);
  pinMode(_aerialSpotLightPin, OUTPUT);
  pinMode(_craneSpotLightPin, OUTPUT);
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
  float pinLevel = analogRead(_soundLevelPin);
  float fraction = _lightMap->soundSensitivity() * (pinLevel - minIncomingSoundLevelValue) / soundLevelRange;
  *sndBfrPos = min( 15.0,
    max(16.0 * fraction, 0)
  );

  if (DEBUG_SOUND_LEVEL) {
    Serial.print("Sound pin level: ");
    Serial.print(pinLevel);
    Serial.print(", buffer position: ");
    Serial.println(*sndBfrPos);
  }
}
