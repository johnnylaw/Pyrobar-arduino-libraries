#ifndef _PYROBAR_FIRE_CANNON_H
#define _PYROBAR_FIRE_CANNON_H

#define FIRE_CANNON_MAX_DURATION 3000

#include <Arduino.h>

class PyrobarFireCannon {
  public:
    PyrobarFireCannon(uint8_t pin);

  private:
    uint8_t _pin;
    long int _lastTurnedOn;
    long int _scheduledShutoff;

  public:
    void begin(void);
    void fire(long int duration);
    void adjust(void);
    void kill(void);
};

#endif

/* Example of usage

PyrobarFireCannon fireCannons[] = {PyrobarFireCannon(1), PyrobarFireCannon(2), PyrobarFireCannon(3)};
const int NUM_CANNONS = sizeof(fireCannons) / sizeof(PyrobarFireCannon);

void setup() {
  Serial.begin(9600);
}

void loop() {
  adjustCannons();

  // if HTTP request is coming {
  //   killCannons();
  //   processRequest();
  // }
  // if UDP packet comes in {
  //   parse for cannonNumber
  //   parse for duration
  //   fireCannons[cannonNumber.fire(duration))
  delay(1000);
}

void adjustCannons() {
  for (int i = 0; i < NUM_CANNONS; i++) {
    fireCannons[i].adjust();
    Serial.println(i);
  }
}

void killCannons() {
  for (int i = 0; i < NUM_CANNONS; i++) {
    fireCannons[i].kill();
  }
}
*/