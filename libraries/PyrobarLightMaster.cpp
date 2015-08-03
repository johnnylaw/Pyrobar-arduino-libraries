#include <Wire.h>
#include "PyrobarConstants.h"
#include "PyrobarLightMaster.h"

PyrobarLightMaster::PyrobarLightMaster(PyrobarLightMap *lightMap, PyrobarPulseLightSet *pulseLightSet, uint8_t soundLevelPin, uint8_t aerialSpotLightPin, uint8_t craneSpotLightPin) : _lastCyclePosition(0.0), _lightMap(lightMap), _pulseLightSet(pulseLightSet), _soundLevelPin(soundLevelPin), _aerialSpotLightPin(aerialSpotLightPin), _craneSpotLightPin(craneSpotLightPin) {
}

void PyrobarLightMaster::sendLightProgramInfo(uint8_t freqBfrPos) {
  for (int slaveInd = 0; slaveInd < numberOfSlaves; slaveInd++) {
    sendSlaveInfo(slaveInd, freqBfrPos);
  }
  digitalWrite(_aerialSpotLightPin, _lightMap->lightIsOn(pyrobarDataTypeAerialSpotlight) ? HIGH : LOW);
  digitalWrite(_craneSpotLightPin, _lightMap->lightIsOn(pyrobarDataTypeCraneSpotlights) ? HIGH : LOW);
}

void PyrobarLightMaster::begin(void) {
  Wire.begin();
  pinMode(_aerialSpotLightPin, OUTPUT);
  pinMode(_craneSpotLightPin, OUTPUT);
}

void PyrobarLightMaster::calculateBufferPositions(uint8_t *freqBfrPos) {
  calculateFrequencyBufferPosition(freqBfrPos);
}

// private
void PyrobarLightMaster::sendSlaveInfo(uint8_t slaveInd, uint8_t freqBfrPos) {
  uint8_t value;
#ifdef DEBUG_LIGHT_OUTPUT
  int byteCounter = 0;
#endif
  Wire.beginTransmission(BASE_I2C_ADDRESS + slaveInd);
#ifdef DEBUG_LIGHT_VALUE_OUTPUT
  Serial.println("\n\nZone0 output:");
#endif

  for (int zoneInd = slaveZoneAddresses[slaveInd].low; zoneInd <= slaveZoneAddresses[slaveInd].high; zoneInd++) {
    for (int colorInd = 0; colorInd < COLOR_COUNT; colorInd++) {
      if (_lightMap->shouldDisplay()) {
        value = _lightMap->read(pyrobarBfrTypeFreq, zoneInd, freqBfrPos, colorInd);
      } else {
        value = _pulseLightSet->read(zoneInd, colorInd);
      }
#ifdef DEBUG_LIGHT_VALUE_OUTPUT
      if (!slaveInd && !zoneInd) {
        Serial.print(value);
        if (colorInd != 3) {
          Serial.print(", ");
        }
      }
#endif
      Wire.write(value);
#ifdef DEBUG_LIGHT_OUTPUT
      byteCounter++;
#endif
    }
  }

  Wire.endTransmission();
#ifdef DEBUG_LIGHT_OUTPUT
  Serial.print("Sent ");
  Serial.print(byteCounter);
  Serial.print(" bytes to ");
  Serial.print(BASE_I2C_ADDRESS + slaveInd);
  Serial.print(", zones ");
  Serial.print(slaveZoneAddresses[slaveInd].low);
  Serial.print(" thru ");
  Serial.println(slaveZoneAddresses[slaveInd].high);
#endif
}

void PyrobarLightMaster::calculateFrequencyBufferPosition(uint8_t *freqBfrPos) {
  float currentMillis = millis();
  float timeElapsed = currentMillis - _lastMillis;
  _lastMillis = currentMillis;
  _lastCyclePosition += (timeElapsed * _lightMap->frequency());
  _lastCyclePosition = fmod(_lastCyclePosition, 1.0);
  *freqBfrPos = (uint8_t)(_lastCyclePosition * BFR_SZ_FREQ);
}

