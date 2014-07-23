#include "PyrobarPulseLightSet.h"

PyrobarPulseLightSet::PyrobarPulseLightSet() {
}

uint8_t PyrobarPulseLightSet::read(int zone, int color) {
  return _lights[zone].read(color);
}

void PyrobarPulseLightSet::startDecayAll(void) {
  for (int i = 0; i < TOTAL_ZONE_COUNT; i++) {
    _lights[i].startDecay();
  }
}

void PyrobarPulseLightSet::pulse(int zone, uint8_t red, uint8_t green, uint8_t blue, float decay) {

}
