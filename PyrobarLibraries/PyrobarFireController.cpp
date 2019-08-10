#include <Arduino.h>
#include "PyrobarFireController.h"
#include "PyrobarConstants.h"

PyrobarFireController::PyrobarFireController(int numberOfCannons, uint8_t *fireCannonPins, PyrobarFireSequence *sequence) : _sequence(sequence), _numberOfCannons(numberOfCannons), _fireCannonPins(fireCannonPins) {
}

void PyrobarFireController::begin(void) {
  for (int i = 0; i < _numberOfCannons; i++) {
    pinMode(_fireCannonPins[i], OUTPUT);
    if (DEBUG_FIRE_SEQUENCE) {
      Serial.print("Setting pinMode to OUTPUT on pin: ");
      Serial.println(_fireCannonPins[i]);
    }
  }
  reset();
}

bool PyrobarFireController::play() {
  unsigned int numberOfNotes = _sequence->numberOfNotes();
  if (numberOfNotes < 1) return false;
  if (_startTime == NULL) {
    dumpSequence();
    _startTime = millis();
  }

  _currentSequenceTime = millis() - _startTime;

  while (_nextNoteIndex < numberOfNotes && shouldStartNoteAtIndex(_nextNoteIndex)) {
    turnOnNoteAtIndex(_nextNoteIndex);
    _nextNoteIndex++;
  }

  for (int i = 0; i < _nextNoteIndex; i++) {
    if(shouldStopNoteAtIndex(i)) {
      turnOffNoteAtIndex(i);
    }
  }

  if (_nextNoteIndex < numberOfNotes) {
    return true;
  }

  if (_noteStates[numberOfNotes - 1] != PYROBAR_FIRE_NOTE_FINISHED_STATE || !allCannonsAreOff()) {
    return true;
  } else {
    reset();
    return false;
  }
}

void PyrobarFireController::reset() {
  _nextNoteIndex = 0;
  _startTime = NULL;
  _sequence->reset();
  killAll();
}

void PyrobarFireController::killAll(void) {
  for (int i = 0; i < _numberOfCannons; i++) {
    digitalWrite(_fireCannonPins[i], LOW);
  }
}

bool PyrobarFireController::shouldStartNoteAtIndex(int index) {
  return _sequence->startTimeAtIndex(index) <= _currentSequenceTime;
}

bool PyrobarFireController::shouldStopNoteAtIndex(int index) {
   return noteAtIndexStillGoing(index) && noteAtIndexExpired(index);
}

bool PyrobarFireController::noteAtIndexStillGoing(int index) {
  return _noteStates[index] != PYROBAR_FIRE_NOTE_FINISHED_STATE;
}

bool PyrobarFireController::noteAtIndexExpired(int index) {
  unsigned long elapsedTime = _currentSequenceTime - _actualNoteStartTimes[index];
  return elapsedTime >= _sequence->durationAtIndex(index);
}

void PyrobarFireController::turnOnNoteAtIndex(int index) {
  int cannon = _sequence->cannonAtIndex(index);
  int pin = _fireCannonPins[cannon];
  digitalWrite(pin, HIGH);
  _actualNoteStartTimes[index] = _currentSequenceTime;
  _noteStates[index] = PYROBAR_FIRE_NOTE_UNFINISHED_STATE;
  _cannonsOn[cannon] = true;

  if (DEBUG_FIRE_SEQUENCE) {
    Serial.print("STARTing cannon on pin ");
    Serial.print(pin);
    Serial.print(" after ");
    Serial.println(_sequence->startTimeAtIndex(index));
  }
}

bool PyrobarFireController::allCannonsAreOff(void) {
  for (int i = 0; i < CANNON_COUNT; i++) {
    if (_cannonsOn[i]) return false;
  }
  return true;
}

void PyrobarFireController::turnOffNoteAtIndex(int index) {
  int cannon = _sequence->cannonAtIndex(index);
  int pin = _fireCannonPins[cannon];
  digitalWrite(pin, LOW);
  _noteStates[index] = PYROBAR_FIRE_NOTE_FINISHED_STATE;
  _cannonsOn[cannon] = false;

  if (DEBUG_FIRE_SEQUENCE) {
    Serial.print("STOPping cannon on pin ");
    Serial.print(pin);
    Serial.print(" after ");
    Serial.println(_sequence->startTimeAtIndex(index));
  }
}

void PyrobarFireController::dumpSequence() {
  if (DEBUG_FIRE_SEQUENCE) {
    Serial.print("NUMBER OF NOTES: ");
    Serial.println(_sequence->numberOfNotes());
    for (int i = 0; i < _sequence->numberOfNotes(); i++) {
      unsigned int startTime = _sequence->startTimeAtIndex(i);
      Serial.print("NOTE "); Serial.print(i);
      Serial.print(" on pin "); Serial.print(_fireCannonPins[_sequence->cannonAtIndex(i)]);
      Serial.print(" from "); Serial.print(startTime);
      Serial.print(" for "); Serial.print(_sequence->durationAtIndex(i));
      Serial.println(" ms");
    }
  }
}
