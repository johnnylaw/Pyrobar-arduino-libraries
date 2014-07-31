#include "PyrobarPulseLightSet.h"

PyrobarPulseLightSet::PyrobarPulseLightSet() {
}

uint8_t PyrobarPulseLightSet::read(int zone, int color) {
  return _lights[zone].read(color);
}

void PyrobarPulseLightSet::startDecayAll(float decay) {
  for (int i = 0; i < TOTAL_ZONE_COUNT; i++) {
    _lights[i].startDecay(decay);
  }
}

void PyrobarPulseLightSet::pulse(int zone, uint8_t red, uint8_t green, uint8_t blue, float decay) {
  _lights[zone].pulse(red, green, blue, decay);
}
