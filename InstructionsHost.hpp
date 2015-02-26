#pragma once

class InstructionsHost {
  public:
    virtual void addCode(Byte) = 0;
    virtual int addressForLabel(const char* label) = 0;
};
