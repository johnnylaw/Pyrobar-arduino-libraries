#include "PyrobarConstants.h"
#include "PyrobarHTTPRequestHandler.h"

PyrobarHTTPRequestHandler::PyrobarHTTPRequestHandler(PyrobarLightValueMap *lightMap) : _lightMap(lightMap) {

}

void PyrobarHTTPRequestHandler::handleRequest(EthernetClient client) {
  if (DEBUG_LIGHT_MAP) {
    Serial.print("Light map in PyrobarHTTPRequestHandler at address ");
    Serial.println((long)_lightMap);
  }
  if (DEBUG_REQUEST_HANDLER) {
    Serial.println("Handling request");
  }
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

bool PyrobarHTTPRequestHandler::parseRequest(EthernetClient client) {
  if (client.available()) {
    if(client.readStringUntil(' ') == "GET") {
      if(client.read() == '/') {
        String dataType = client.readStringUntil('/');
        if (DEBUG_REQUEST_HANDLER) {
          Serial.print("Data Type: ");
          Serial.println(dataType);
        }
        if(dataType == pyrobarDataTypeBuffer) {
          return handleBuffer(client);
        } else if(dataType == pyrobarDataTypeFire) {
          return handleFireSequence(client);
        } else if(dataType == pyrobarDataTypeScalar) {
          return handleScalar(client);
        } else if(dataType == pyrobarDataTypeLights) {
          return handleLightsOnOff(client);
        } else {
          return false;
        }
      }
    } else {
      return false;
    }
  }
}

bool PyrobarHTTPRequestHandler::handleLightsOnOff(EthernetClient client) {
  String instruction = client.readStringUntil(' ');
  if (DEBUG_REQUEST_HANDLER) {
    Serial.print("Turning lights ");
    Serial.println(instruction);
  }
  if (instruction == pyrobarLightsOut) _lightMap->turnLights(OFF);
  else if (instruction == pyrobarLightsOn) _lightMap->turnLights(ON);
  else return false;
  return true;
}

bool PyrobarHTTPRequestHandler::handleBuffer(EthernetClient client) {
  // E.g. /bfr/snd/0/000001...

  String bufferType = client.readStringUntil('/');
  if (DEBUG_REQUEST_HANDLER) {
    Serial.print("Buffer type: ");
    Serial.println(bufferType);
  }

  if (bufferType == pyrobarBfrTypeFreq || bufferType == pyrobarBfrTypeSnd) {
    int zone = atoi(client.readStringUntil('/').c_str());
    if (zone < _lightMap->zoneCount()) {
      loadBuffer(bufferType, zone, client);
      return true;
    } else {
      // add error notice to stream
      return false;
    }
  }
}

bool PyrobarHTTPRequestHandler::loadBuffer(String type, int zone, EthernetClient client) {
  if (DEBUG_REQUEST_HANDLER) {
    Serial.print("Loading buffer for zone ");
    Serial.println(zone);
  }
  char tempHex[3] = "00";
  bool success = true;
  while((tempHex[0] = client.read()) != ' ' && (tempHex[1] = client.read()) != ' ') {
    unsigned char value = strtoul(tempHex, NULL, 16);
    if (DEBUG_REQUEST_HANDLER && zone == 0) {
      Serial.print("Received '");
      Serial.print(tempHex);
      Serial.print("' -> ");
      Serial.println(value);
    }
    if (!_lightMap->write(type, zone, value)) success = false;
  }
  return success;
}

bool PyrobarHTTPRequestHandler::handleScalar(EthernetClient client) {
  // E.g. /sclr/sndSens/0.789
  String scalarType = client.readStringUntil('/');
  if (DEBUG_REQUEST_HANDLER) {
    Serial.print("Scalar type: ");
    Serial.println(scalarType);
  }
  float value = client.readStringUntil(' ').toFloat();
  return _lightMap->setScalar(scalarType, value);
}

bool PyrobarHTTPRequestHandler::handleFireSequence(EthernetClient client) {
  // E.g. /fire/012341234012341234012341234
}
