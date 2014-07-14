#ifndef _PYROBAR_LIGHT_VALUE_MAP_H
#define _PYROBAR_LIGHT_VALUE_MAP_H

#include <Arduino.h>
#include <String.h>

#define BFR_SZ_FREQ 256
#define BFR_SZ_SND 16
#define MAX_ZONES_PER_BOARD 4
#define COLOR_COUNT 3
#define BFR_TYPE_SOUND
#define DEFAULT_FREQUENCY 0.25

const String pyrobarDataTypeBuffer = "bfr";
const String pyrobarBfrTypeSnd = "snd";
const String pyrobarBfrTypeFreq = "freq";

const String pyrobarDataTypeScalar = "sclr";
const String pyrobarScalarTypeSoundSensitivity = "sndSens";
const String pyrobarScalarTypeFrequency = "freq";

class PyrobarLightValueMap {

public:
  PyrobarLightValueMap();

private:
  uint8_t _freqBfrs[MAX_ZONES_PER_BOARD][BFR_SZ_FREQ][COLOR_COUNT];
  uint8_t _sndBfrs[MAX_ZONES_PER_BOARD][BFR_SZ_SND][COLOR_COUNT];
  float _frequency;
  float _soundSensitivity;

public:
  void write(String type, int zone, int index, int color, uint8_t value);
  uint8_t read(String type, int zone, int index, int color);
  float frequency(void);
  void setFrequency(float frequency);
  float soundSensitivity(void);
  void setSoundSensitivity(float soundSensitivity);

};

#endif