#include <Wire.h>
#include "PyroLightController.h"

PyroLightController::PyroLightController(int numberOfAddressPins, uint8_t *boardAddressPins, uint8_t ledPins[MAX_ZONES_PER_BOARD][COLOR_COUNT], int totalZoneCount) : _numberOfAddressPins(numberOfAddressPins), _boardAddressPins(boardAddressPins), _totalZoneCount(totalZoneCount), _boardNumber(NULL) {
}

bool PyroLightController::isMaster() {
  return boardNumber() == 0;
}

int PyroLightController::boardNumber() { // memoized
  if (_boardNumber == NULL) {
    _boardNumber = 0;
    for(int i = 0; i < _numberOfAddressPins; i++) {
      _boardNumber += (digitalRead(_boardAddressPins[i]) << i);
    }
  }
  return _boardNumber;
}

int PyroLightController::i2cAddress() {
  return boardNumber() == 0 ? 0 : 1;
}

void PyroLightController::begin(void) {
  if (isMaster()) {
    _zoneCount = (_totalZoneCount - 1) % MAX_ZONES_PER_BOARD + 1; // Gives back MAX_ZONES_PER_BOARD instead of zero
  } else {
    _zoneCount = MAX_ZONES_PER_BOARD;
  }

  for (int zone = 0; zone < _zoneCount; zone++) {
    for (int color = 0; color < COLOR_COUNT; color++) {
      pinMode(_ledPins[zone][color], OUTPUT);
    }
  }
  Wire.begin(i2cAddress());
}

void PyroLightController::setLEDs(uint8_t freqBfrPos, uint8_t sndBfrPos) {
  for (int zone = 0; zone < _zoneCount; zone++) {
    for (int color = 0; color < COLOR_COUNT; color++) {
      uint8_t freqValue = _freqBfrs[zone][freqBfrPos][color];
      uint8_t sndValue = _sndBfrs[zone][sndBfrPos][color];
      analogWrite(_ledPins[zone][color], max(freqValue, sndValue));
    }
  }
}
