#ifndef _PYROBAR_HTTP_REQUEST_HANDLER_H
#define _PYROBAR_HTTP_REQUEST_HANDLER_H

#include <Arduino.h>
#include <Ethernet.h>
#include "PyrobarLightMap.h"
#include "PyrobarFireSequence.h"

class PyrobarHTTPRequestHandler {

public:
  PyrobarHTTPRequestHandler(PyrobarLightMap *lightMap, PyrobarFireSequence *fireSequence);

private:
  PyrobarLightMap *_lightMap;
  PyrobarFireSequence *_fireSequence;

public:
  void handleRequest(EthernetClient client);

private:
  bool parseRequest(EthernetClient client);
  bool handleBuffer(EthernetClient client);
  bool handleScalar(EthernetClient client);
  bool handleFireSequence(EthernetClient client);
  bool loadBuffer(String type, int zone, EthernetClient client);
  bool handleLightsOnOff(EthernetClient client);

};

#endif