#pragma once

class InstructionsHost {
  public:
    virtual void addCode(Byte) = 0;
    virtual void add16BitValue(int value) = 0;
    virtual void add16BitAddress(const Argument&) = 0;
    virtual void add8BitRelativeAddress(const Argument& arg) = 0;
};
