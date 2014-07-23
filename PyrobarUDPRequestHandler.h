#ifndef _PYROBAR_UDP_REQUEST_HANDLER_H
#define _PYROBAR_UDP_REQUEST_HANDLER_H

#include <Arduino.h>
#include <Ethernet.h>
#include "PyrobarLightValueMap.h"

class PyrobarUDPRequestHandler {

public:
  PyrobarUDPRequestHandler(PyrobarLightValueMap *lightMap);

#endif