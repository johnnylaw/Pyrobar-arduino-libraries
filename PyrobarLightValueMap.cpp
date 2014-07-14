#include "PyrobarLightValueMap.h"

PyrobarLightValueMap::PyrobarLightValueMap() {

}

void PyrobarLightValueMap::write(String type, int zone, int index, int color, uint8_t value) {

}

uint8_t PyrobarLightValueMap::read(String type, int zone, int index, int color) {
  if (type == pyrobarBfrTypeFreq) {
    return _freqBfrs[zone][index][color];
  } else if (type == pyrobarBfrTypeSnd) {
    return _sndBfrs[zone][index][color];
  } else {
    return 0;
  }
}
