#ifndef _PYROBAR_REQUEST_HANDLER_H
#define _PYROBAR_REQUEST_HANDLER_H

#include <Arduino.h>
#include <Ethernet.h>
#include "PyrobarLightValueMap.h"

class PyrobarRequestHandler {

public:
  PyrobarRequestHandler(PyrobarLightValueMap lightMap);

private:
  PyrobarLightValueMap _lightMap;

public:
  void handleRequest(EthernetClient client);

private:
  bool parseRequest(EthernetClient client);
  bool handleBuffer(EthernetClient client);
  bool handleScalar(EthernetClient client);
  bool handleFireSequence(EthernetClient client);
  bool loadBuffer(String type, int zone, EthernetClient client);

};

#endif