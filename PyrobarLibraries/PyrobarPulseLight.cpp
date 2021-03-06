#include "PyrobarPulseLight.h"

PyrobarPulseLight::PyrobarPulseLight() : _alive(false) {
}

void PyrobarPulseLight::pulse(uint8_t red, uint8_t green, uint8_t blue, float decay) {
  _values[0] = red;
  _values[1] = green;
  _values[2] = blue;
  _alive = true;
  _startedDecayingAt = 0;
  if (decay > 0.0) {
    startDecay(decay);
  }
  if (DEBUG_PULSE_LIGHT) {
    Serial.print("RGB: ");
    Serial.print(red);
    Serial.print(", ");
    Serial.print(green);
    Serial.print(", ");
    Serial.print(blue);
    Serial.print(", ");
    Serial.println(decay);
  }
}

void PyrobarPulseLight::startDecay(float decay) {
  _decay = decay * 8;
  if (_startedDecayingAt == 0 && _alive) _startedDecayingAt = millis();
}

uint8_t PyrobarPulseLight::read(int color) {
  uint8_t value = 0;
  if (_alive) {
    float multiplier = 1.0;
    if (_startedDecayingAt) {
      multiplier = pow(0.5, (millis() - _startedDecayingAt) / _decay);
    }
    value = _values[color] * multiplier;
    if (DEBUG_PULSE_LIGHT) {
      Serial.print("Value for color"); Serial.print(color); Serial.print(": ");
      Serial.println(value);
    }
  }
  return value;
}

void PyrobarPulseLight::kill(void) {
  _alive = false;
}
