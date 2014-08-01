#pragma once

#include "PyrobarFireSequence.h"

#define PYROBAR_FIRE_NOTE_UNFINISHED_STATE false
#define PYROBAR_FIRE_NOTE_FINISHED_STATE true

class PyrobarFireController {
  public:
    PyrobarFireController(int numberOfCannons, uint8_t *fireCannonPins, PyrobarFireSequence *sequence);

  private:
    PyrobarFireSequence *_sequence;
    int _numberOfCannons;
    uint8_t *_fireCannonPins;
    unsigned long _startTime;
    bool _noteStates[MAX_NOTES];
    unsigned long _actualNoteStartTimes[MAX_NOTES];
    int _nextNoteIndex;
    unsigned long _currentSequenceTime;
    bool _cannonsOn[CANNON_COUNT];

  public:
    void begin(void);
    bool play(void);
    void reset(void);

  private:
    void dumpSequence();
    bool shouldStartNoteAtIndex(int index);
    bool shouldStopNoteAtIndex(int index);
    bool noteAtIndexStillGoing(int index);
    bool noteAtIndexExpired(int index);
    void turnOnNoteAtIndex(int index);
    void turnOffNoteAtIndex(int index);
    void killAll(void);
    bool allCannonsAreOff(void);

};