#ifndef _PYROBAR_LIGHT_MAP_H
#define _PYROBAR_LIGHT_MAP_H

#include <Arduino.h>
#include <String.h>

#define BFR_SZ_FREQ 256
#define BFR_SZ_SND 16
#define ZONES_PER_SLAVE_BOARD 4

#ifndef COLOR_COUNT
#define COLOR_COUNT 3
#endif

#define BFR_TYPE_SOUND
#define DEFAULT_FREQUENCY 0.25

#ifndef TOTAL_ZONE_COUNT
#define TOTAL_ZONE_COUNT 9
#endif

const String pyrobarDataTypeBuffer = "bfr";
const String pyrobarBfrTypeSnd = "sound";
const String pyrobarBfrTypeFreq = "frequency";

const String pyrobarDataTypeLights = "lights";
const String pyrobarLightsOut = "out";
const String pyrobarLightsOn = "on";

const String pyrobarDataTypeFire = "fire";

const String pyrobarDataTypeScalar = "sclr";
const String pyrobarScalarTypeSoundSensitivity = "sndSens";
const String pyrobarScalarTypeFrequency = "freq";

class PyrobarLightMap {

public:
  PyrobarLightMap();

private:
  uint8_t _freqBfrs[TOTAL_ZONE_COUNT][BFR_SZ_FREQ][COLOR_COUNT];
  uint8_t _sndBfrs[TOTAL_ZONE_COUNT][BFR_SZ_SND][COLOR_COUNT];
  int _bfrWritePtr[3];
  float _frequency;
  float _soundSensitivity;
  bool _allOff;

public:
  bool write(String type, int zone, uint8_t value);
  bool writeHexString(String type, int zone, String hexString);
  uint8_t read(String type, int zone, int index, int color);
  float frequency(void);
  float soundSensitivity(void);
  bool setScalar(String type, float value);
  bool shouldDisplay(void);
  void turnLights(bool on);

private:
  void resetWritePtr(void);
  void advanceWritePtr(int bfrLength);

};

#endif