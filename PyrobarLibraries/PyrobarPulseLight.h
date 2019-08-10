#ifndef _PYROBAR_PULSE_LIGHT_H
#define _PYROBAR_PULSE_LIGHT_H

#include <Arduino.h>
#include "PyrobarConstants.h"

class PyrobarPulseLight {
public:
  PyrobarPulseLight();

private:
  uint8_t _values[COLOR_COUNT];
  unsigned long _startedDecayingAt;
  float _decay;
  bool _alive;

public:
  void pulse(uint8_t red, uint8_t green, uint8_t blue, float decay);
  uint8_t read(int color);
  void kill(void);
  void startDecay(float decay);
};

#endif
