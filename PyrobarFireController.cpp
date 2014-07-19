#include <Arduino.h>
#include "PyrobarFireController.h"

PyrobarFireController::PyrobarFireController(int numberOfCannons, uint8_t *fireCannonPins, PyrobarFireSequence sequence) : _numberOfCannons(numberOfCannons), _fireCannonPins(fireCannonPins), _sequence(sequence) {
}

void PyrobarFireController::begin(void) {
  for (int i = 0; i < _numberOfCannons; i++) {
    pinMode(_fireCannonPins[i], OUTPUT);
  }
  reset();
}

bool PyrobarFireController::play() {
  unsigned int numberOfNotes = _sequence.numberOfNotes();
  if (numberOfNotes < 1) return false;
  if (_startTime == NULL) {
    dumpSequence();
    _startTime = millis();
  }

  unsigned long currentSequenceTime = millis() - _startTime;

  while (_nextNoteIndex < numberOfNotes && _sequence.startTimeAtIndex(_nextNoteIndex) <= currentSequenceTime) {
    digitalWrite(_fireCannonPins[_sequence.cannonAtIndex(_nextNoteIndex)], HIGH);
    _actualNoteStartTimes[_nextNoteIndex] = currentSequenceTime;
    _noteStates[_nextNoteIndex] = PYROBAR_FIRE_NOTE_UNFINISHED_STATE;
    _nextNoteIndex++;
  }

  int i = 0;
  while (i < _nextNoteIndex) {
    if(_noteStates[i] != PYROBAR_FIRE_NOTE_FINISHED_STATE && _sequence.durationAtIndex(i) + _actualNoteStartTimes[i] <= currentSequenceTime) {
      digitalWrite(_fireCannonPins[_sequence.cannonAtIndex(i)], LOW);
      _noteStates[i] = PYROBAR_FIRE_NOTE_FINISHED_STATE;
    }
    i++;
  }
  return _nextNoteIndex < numberOfNotes || _noteStates[numberOfNotes - 1] != PYROBAR_FIRE_NOTE_FINISHED_STATE;
}

void PyrobarFireController::reset() {
  _nextNoteIndex = 0;
  _startTime = NULL;
}

void PyrobarFireController::dumpSequence() {
  for (int i = 0; i < _sequence.numberOfNotes(); i++) {
    unsigned int startTime = _sequence.startTimeAtIndex(i);
    Serial.print("NOTE "); Serial.print(i);
    Serial.print(" on pin "); Serial.print(_sequence.cannonAtIndex(i));
    Serial.print(" from "); Serial.print(startTime);
    Serial.print(" for "); Serial.print(_sequence.durationAtIndex(i));
    Serial.println(" ms");
  }
}
