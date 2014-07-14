#pragma once

#define MAX_NOTES 70

class PyrobarFireSequence {
  public:
    PyrobarFireSequence(void);

  private:
    unsigned int _startTimes[MAX_NOTES];
    unsigned int _durations[MAX_NOTES];
    int _cannons[MAX_NOTES];
    unsigned int _noteIndex;
    unsigned int _numberOfNotes;

  public:
    void addNote(int cannon, unsigned int startTime, unsigned int duration);
    void setNumberOfNotes(unsigned int numberOfNotes);
    unsigned int numberOfNotes();
    int cannonAtIndex(int index);
    unsigned int durationAtIndex(int index);
    unsigned int startTimeAtIndex(int index);
};