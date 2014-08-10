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

const char pyrobarDataTypeBuffer[] = "bfr";
const char pyrobarBfrTypeSnd[] = "sound";
const char pyrobarBfrTypeFreq[] = "frequency";

const char pyrobarDataTypeAerialSpotlight[] = "aerial-spot";
const char pyrobarDataTypeCraneSpotlights[] = "crane-spot";
const char pyrobarDataTypeMainLights[] = "main";
const char pyrobarDataTypeLights[] = "lights";

const char pyrobarLightsOut[] = "out";
const char pyrobarLightsOn[] = "on";

const char pyrobarDataTypeFire[] = "fire";

const char pyrobarDataTypeScalar[] = "sclr";
const char pyrobarScalarTypeSoundSensitivity[] = "sndSens";
const char pyrobarScalarTypeFrequency[] = "freq";

class PyrobarLightMap {

public:
  PyrobarLightMap();

private:
  uint8_t _freqBfrs[TOTAL_ZONE_COUNT][BFR_SZ_FREQ][COLOR_COUNT];
  uint8_t _sndBfrs[TOTAL_ZONE_COUNT][BFR_SZ_SND][COLOR_COUNT];
  int _bfrWritePtr[3];
  float _frequency;
  float _soundSensitivity;
  bool _mainOff;
  bool _craneSpotLightsOn;
  bool _aerialSpotLightOn;

public:
  bool write(String type, int zone, uint8_t value);
  bool writeHexString(String type, int zone, String hexString);
  uint8_t read(String type, int zone, int index, int color);
  float frequency(void);
  float soundSensitivity(void);
  bool setScalar(String type, float value);
  bool shouldDisplay(void);
  void turnLights(String type, bool on);
  bool lightIsOn(String type);

private:
  void resetWritePtr(void);
  void advanceWritePtr(int bfrLength);

};

#endif