#ifndef _PYROBAR_PULSE_LIGHT_SET_H
#define _PYROBAR_PULSE_LIGHT_SET_H

#include <Arduino.h>
#include "PyrobarConstants.h"
#include "PyrobarPulseLight.h"

class PyrobarPulseLightSet {
public:
  PyrobarPulseLightSet();

private:
  PyrobarPulseLight _lights[TOTAL_ZONE_COUNT];

public:
  uint8_t read(int zone, int color);
  void startDecayAll(float decay);
  void pulse(int zone, uint8_t red, uint8_t green, uint8_t blue, float decay);

};

#endif
