#ifndef _PYROBAR_UDP_REQUEST_HANDLER_H
#define _PYROBAR_UDP_REQUEST_HANDLER_H

#include <Arduino.h>
#include <Ethernet.h>
#include "PyrobarLightValueMap.h"
#include "PyrobarPulseLightSet.h"

class PyrobarUDPRequestHandler {

public:
  PyrobarUDPRequestHandler(PyrobarLightValueMap *lightMap, PyrobarPulseLightSet *pulseLightSet);

private:
  PyrobarLightValueMap *_lightMap;
  PyrobarPulseLightSet *_pulseLightSet;

public:
  void handleRequest(unsigned char *buffer, int length);

};

#endif