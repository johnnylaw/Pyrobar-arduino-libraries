#ifndef _PYRO_LIGHT_CONTROLLER_H
#define _PYRO_LIGHT_CONTROLLER_H

#include <Arduino.h>
#include "PyrobarLightValueMap.h"

class PyroLightController {
public:
  PyroLightController(int numberOfAddressPins, uint8_t *boardAddressPins, uint8_t ledPins[MAX_ZONES_PER_BOARD][COLOR_COUNT], int totalZoneCount, PyrobarLightValueMap lightMap);

private:
  int _numberOfAddressPins;
  int _totalZoneCount;
  int _zoneCount;
  int _boardNumber;
  PyrobarLightValueMap _lightMap;
  uint8_t *_boardAddressPins;
  uint8_t _ledPins[MAX_ZONES_PER_BOARD][COLOR_COUNT];

public:
  void begin(void);
  bool isMaster(void);
  int boardNumber(void);
  int i2cAddress(void);
  void setLEDs(uint8_t freqBfrPos, uint8_t sndBfrPos);
};

#endif