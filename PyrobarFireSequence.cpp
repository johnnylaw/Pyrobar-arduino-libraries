#include <Arduino.h>
#include "PyrobarFireSequence.h"

PyrobarFireSequence::PyrobarFireSequence(void) : _noteIndex(0) {
}

void PyrobarFireSequence::reset(void) {
  _noteIndex = 0;
}

void PyrobarFireSequence::addNote(int cannon, unsigned long startTime, unsigned int duration) {
  _cannons[_noteIndex] = cannon;
  _startTimes[_noteIndex] = startTime;
  _durations[_noteIndex] = duration;
  _noteIndex++;
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
  return _durations[index];
}