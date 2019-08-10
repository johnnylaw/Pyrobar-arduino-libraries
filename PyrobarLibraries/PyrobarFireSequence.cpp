#include <Arduino.h>
#include "PyrobarFireSequence.h"

PyrobarFireSequence::PyrobarFireSequence(void) : _noteIndex(0) {
}

void PyrobarFireSequence::reset(void) {
  _noteIndex = 0;
  for (int i = 0; i < CANNON_COUNT; i++) {
    _cannonsAlive[i] = true;
  }
}

void PyrobarFireSequence::addNote(int cannon, unsigned long startTime, unsigned int duration) {
  if (cannon < CANNON_COUNT) {
    _cannonsAlive[cannon] = true;
    _cannons[_noteIndex] = cannon;
    _startTimes[_noteIndex] = startTime;
    _durations[_noteIndex] = min(MAX_FIRE_DURATION, duration);
    _noteIndex++;
  }
}

unsigned int PyrobarFireSequence::numberOfNotes(void) {
  return _noteIndex;
}

int PyrobarFireSequence::cannonAtIndex(int index) {
  return _cannons[index];
}

unsigned int PyrobarFireSequence::startTimeAtIndex(int index) {
  return _startTimes[index];
}

unsigned int PyrobarFireSequence::durationAtIndex(int index) {
  if (_cannonsAlive[_cannons[index]]) {
    return _durations[index];
  }
  return 0;
}

void PyrobarFireSequence::kill(int cannonIndex) {
  _cannonsAlive[cannonIndex] = false;
}
