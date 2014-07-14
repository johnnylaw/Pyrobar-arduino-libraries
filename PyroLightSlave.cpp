#include "PyroLightSlave.h"

PyroLightSlave::PyroLightSlave() {

}

void PyroLightSlave::readBufferPositions(uint8_t *freqBfrPos, uint8_t *sndBfrPos) {
  if (Wire.available()) *freqBfrPos = Wire.read();
  if (Wire.available()) *sndBfrPos = Wire.read();
}

PyroLightSlave PyroSlave = PyroLightSlave();
