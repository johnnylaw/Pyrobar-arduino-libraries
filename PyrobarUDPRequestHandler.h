#ifndef _PYROBAR_UDP_REQUEST_HANDLER_H
#define _PYROBAR_UDP_REQUEST_HANDLER_H

#include <Arduino.h>
#include <Ethernet.h>
#include "PyrobarLightMap.h"
#include "PyrobarPulseLightSet.h"

class PyrobarUDPRequestHandler {

public:
  PyrobarUDPRequestHandler(PyrobarLightMap *lightMap, PyrobarPulseLightSet *pulseLightSet);

private:
  PyrobarLightMap *_lightMap;
  PyrobarPulseLightSet *_pulseLightSet;

public:
  void handleRequest(unsigned char *buffer, int length);

};

#endif