#pragma once

#include "PyrobarConstants.h"

#define MAX_NOTES 512

class PyrobarFireSequence {
  public:
    PyrobarFireSequence(void);

  private:
    unsigned long _startTimes[MAX_NOTES];
    unsigned int _durations[MAX_NOTES];
    int _cannons[MAX_NOTES];
    bool _cannonsAlive[CANNON_COUNT];
    unsigned int _noteIndex;

  public:
    void addNote(int cannon, unsigned long startTime, unsigned int duration);
    void reset(void);
    unsigned int numberOfNotes();
    int cannonAtIndex(int index);
    unsigned int durationAtIndex(int index);
    unsigned int startTimeAtIndex(int index);
    void kill(int cannonIndex);
};