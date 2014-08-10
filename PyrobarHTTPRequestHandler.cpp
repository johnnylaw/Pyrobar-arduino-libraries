#include "PyrobarConstants.h"
#include "PyrobarHTTPRequestHandler.h"

PyrobarHTTPRequestHandler::PyrobarHTTPRequestHandler(PyrobarLightMap *lightMap, PyrobarFireSequence *fireSequence) : _lightMap(lightMap), _fireSequence(fireSequence) {

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
  String type = client.readStringUntil('/');
  String instruction = client.readStringUntil(' ');
  if (DEBUG_REQUEST_HANDLER) {
    Serial.print("Turning lights ");
    Serial.println(instruction);
  }
  if (instruction == pyrobarLightsOut) _lightMap->turnLights(type, OFF);
  else if (instruction == pyrobarLightsOn) _lightMap->turnLights(type, ON);
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
    if (zone < TOTAL_ZONE_COUNT) {
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
  // E.g. /fire/00000af90000aa..
  // _fireSequence->addNote(int cannon, unsigned long startTime, unsigned int duration);
  if (DEBUG_HTTP) Serial.println("Incoming fire sequence...");
  bool finished = false;
  char cannonHex[3] = "0";
  char startTimeHex[6] = "00000";
  char durationHex[5] = "0000";
  while (!finished) {
    for (int i = 0; i < 1; i++) {
      cannonHex[i] = client.read();
      if (cannonHex[i] == ' ') finished = true;
    }
    for (int i = 0; i < 5; i++) {
      startTimeHex[i] = client.read();
      if (startTimeHex[i] == ' ') finished = true;
    }
    for (int i = 0; i < 4; i++) {
      durationHex[i] = client.read();
      if (durationHex[i] == ' ') finished = true;
    }
    if (!finished) {
      int cannon = strtoul(cannonHex, NULL, 16);
      unsigned long startTime = strtoul(startTimeHex, NULL, 16);
      unsigned int duration = strtoul(durationHex, NULL, 16);
      _fireSequence->addNote(cannon, startTime, duration);
      if (DEBUG_HTTP) {
        Serial.print("Cannon: ");
        Serial.print(cannon);
        Serial.print(", start time: ");
        Serial.print(startTime);
        Serial.print(", duration: ");
        Serial.println(duration);
      }
    }
  }
}
