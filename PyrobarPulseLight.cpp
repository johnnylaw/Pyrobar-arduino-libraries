#include "PyrobarPulseLight.h"

PyrobarPulseLight::PyrobarPulseLight(uint8_t redPin, uint8_t greenPin, uint8_t bluePin) : _alive(false) {
  _pins[0] = redPin;
  _pins[1] = greenPin;
  _pins[2] = bluePin;
}

void PyrobarPulseLight::pulse(uint8_t red, uint8_t green, uint8_t blue, float decay) {
  _values[0] = red;
  _values[1] = green;
  _values[2] = blue;
  _decay = decay;
  _alive = true;
  _startedAt = millis();
}

void PyrobarPulseLight::adjust(void) {
  if (_alive) {
    float multiplier = pow(millis() - _startedAt, _decay);
    for (int i = 0; i < COLOR_COUNT; i++) {
      analogWrite(_pins[i], _values[i] * multiplier);
    }
  }
}

void PyrobarPulseLight::kill(void) {
  _alive = false;
}
