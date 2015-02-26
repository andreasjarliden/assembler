#pragma once

class InstructionsHost {
  public:
    virtual void addCode(Byte) = 0;
    virtual void add16BitValue(int value) = 0;
    virtual bool containsLabel(const char* label) = 0;
    virtual int addressForLabel(const char* label) = 0;
    virtual void addDelayed16BitValue(const char* identifier) = 0;
};
