#include <Arduino.h>
#include "PyrobarFireSequence.h"

PyrobarFireSequence::PyrobarFireSequence(void) : _noteIndex(0), _numberOfNotes(0) {
}

void PyrobarFireSequence::setNumberOfNotes(unsigned int numberOfNotes) {
  _numberOfNotes = numberOfNotes;
  _noteIndex = 0;
}

void PyrobarFireSequence::addNote(int cannon, unsigned int startTime, unsigned int duration) {
  if (_noteIndex <= _numberOfNotes) {
    _cannons[_noteIndex] = cannon;
    _startTimes[_noteIndex] = startTime;
    _durations[_noteIndex] = duration;
    _noteIndex++;
  }
}

unsigned int PyrobarFireSequence::numberOfNotes(void) {
  return _numberOfNotes;
}

int PyrobarFireSequence::cannonAtIndex(int index) {
  return _cannons[index];
}

unsigned int PyrobarFireSequence::startTimeAtIndex(int index) {
  return _startTimes[index];
}

unsigned int PyrobarFireSequence::durationAtIndex(int index) {
  return _durations[index];
}