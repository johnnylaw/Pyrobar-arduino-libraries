#include "PyrobarFireCannon.h"

PyrobarFireCannon::PyrobarFireCannon(uint8_t pin) : _pin(pin), _lastTurnedOn(0) {}

void PyrobarFireCannon::begin(void) {
  pinMode(_pin, OUTPUT);
}

void PyrobarFireCannon::fire(long int duration) {
  if (duration == 0 || duration > FIRE_CANNON_MAX_DURATION) duration = FIRE_CANNON_MAX_DURATION;
  _lastTurnedOn = millis();
  _scheduledShutoff = _lastTurnedOn + duration;
  digitalWrite(_pin, HIGH);
}

void PyrobarFireCannon::adjust(void) {
  long int currentTime = millis();
  if (_lastTurnedOn > currentTime || currentTime > _scheduledShutoff) {
    kill();
  }
}

void PyrobarFireCannon::kill(void) {
  digitalWrite(_pin, LOW);
}
