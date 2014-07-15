#include "PyrobarRequestHandler.h"

PyrobarRequestHandler::PyrobarRequestHandler(PyrobarLightValueMap lightMap) : _lightMap(lightMap) {
}

void PyrobarRequestHandler::handleRequest(EthernetClient client) {
  bool currentLineIsBlank = true;
  bool success = parseRequest(client);
  while (client.connected()) {
    char c = client.read();
    if (c == '\n' && currentLineIsBlank) {
      client.print("HTTP/1.1 ");
      client.println(success ? "200 OK" : "401 BAD REQUEST");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");  // the connection will be closed after completion of the response
      break;
    }
    if (c == '\n') currentLineIsBlank = true;
    else if (c != '\r') currentLineIsBlank = false;
  }
  client.stop();
}

bool PyrobarRequestHandler::parseRequest(EthernetClient client) {
  if (client.available()) {
    if(client.readStringUntil(' ') == "GET") {
      if(client.read() == '/') {
        String dataType = client.readStringUntil('/');
        Serial.print("\nData Type: ");
        Serial.println(dataType);
        if(dataType == pyrobarDataTypeBuffer) {
          return handleBuffer(client);
        } else if(dataType == pyrobarDataTypeFire) {
          return handleFireSequence(client);
        } else if(dataType == pyrobarDataTypeScalar) {
          return handleScalar(client);
        } else {
          return "401";
        }
      }
    } else {
      return "401";
    }
  }
}

bool PyrobarRequestHandler::handleBuffer(EthernetClient client) {
  // E.g. /bfr/snd/0/000001...

  String bufferType = client.readStringUntil('/');

  if (bufferType == pyrobarBfrTypeFreq || bufferType == pyrobarBfrTypeSnd) {
    int zone = atoi(client.readStringUntil('/').c_str());
    if (zone < _lightMap.zoneCount()) {
      loadBuffer(bufferType, zone, client);
      return true;
    } else {
      // add error notice to stream
      return false;
    }
  }
}

bool PyrobarRequestHandler::loadBuffer(String type, int zone, EthernetClient client) {
  char tempHex[3] = "00";
  bool success = true;
  while((tempHex[0] = client.read()) != ' ' && (tempHex[0] = client.read()) != ' ') {
    if (!_lightMap.write(type, zone, strtoul(tempHex, NULL, 16))) success = false;
  }
  return success;
}

bool PyrobarRequestHandler::handleScalar(EthernetClient client) {
  // E.g. /sclr/sndSens/0.789
  String scalarType = client.readStringUntil('/');
  float value = client.readStringUntil(' ').toFloat();
  return _lightMap.setScalar(scalarType, value);
}

bool PyrobarRequestHandler::handleFireSequence(EthernetClient client) {
  // E.g. /fire/012341234012341234012341234
}
