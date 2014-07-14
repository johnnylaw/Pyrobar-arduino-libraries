#pragma once

#include "PyrobarFireSequence.h"

#define PYROBAR_FIRE_NOTE_UNFINISHED_STATE 0
#define PYROBAR_FIRE_NOTE_FINISHED_STATE 1

class PyrobarFireController {
  public:
    PyrobarFireController(int numberOfCannons, uint8_t *fireCannonPins);

  private:
    PyrobarFireSequence _sequence;
    int _numberOfCannons;
    uint8_t *_fireCannonPins;
    unsigned long _startTime;
    unsigned char _noteStates[MAX_NOTES];
    unsigned int _actualNoteStartTimes[MAX_NOTES];
    int _nextNoteIndex;

  public:
    void begin(void);
    bool play(void);
    void setSequence(PyrobarFireSequence sequence);
    void reset(void);

  private:
    void dumpSequence();
};