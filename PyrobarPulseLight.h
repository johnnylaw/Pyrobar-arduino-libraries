#ifndef _PYROBAR_PULSE_LIGHT_H
#define _PYROBAR_PULSE_LIGHT_H

#include <Arduino.h>
#include "PyrobarConstants.h"

class PyrobarPulseLight {
public:
  PyrobarPulseLight(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);

private:
  uint8_t _values[COLOR_COUNT];
  uint8_t _pins[COLOR_COUNT];
  unsigned long _startedAt;
  float _decay;
  bool _alive;

public:
  void pulse(uint8_t red, uint8_t green, uint8_t blue, float decay);
  void adjust(void);
  void kill(void);
};

#endif
