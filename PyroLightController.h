#ifndef _PYRO_LIGHT_CONTROLLER_H
#define _PYRO_LIGHT_CONTROLLER_H

#include <Arduino.h>

#define BFR_SZ_FREQ 256
#define BFR_SZ_SND 16
#define DEFAULT_FREQUENCY 0.25
#define MAX_ZONES_PER_BOARD 4
#define COLOR_COUNT 3

class PyroLightController {
public:
  PyroLightController(int numberOfAddressPins, uint8_t *boardAddressPins, uint8_t ledPins[MAX_ZONES_PER_BOARD][COLOR_COUNT], int totalZoneCount);

private:
  int _numberOfAddressPins;
  int _totalZoneCount;
  int _zoneCount;
  int _boardNumber;
  uint8_t *_boardAddressPins;
  uint8_t _freqBfrs[MAX_ZONES_PER_BOARD][BFR_SZ_FREQ][COLOR_COUNT];
  uint8_t _sndBfrs[MAX_ZONES_PER_BOARD][BFR_SZ_SND][COLOR_COUNT];
  uint8_t _ledPins[MAX_ZONES_PER_BOARD][COLOR_COUNT];

public:
  void begin(void);
  bool isMaster(void);
  int boardNumber(void);
  int i2cAddress(void);
  void setLEDs(uint8_t freqBfrPos, uint8_t sndBfrPos);
};

#endif