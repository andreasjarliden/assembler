#pragma once

#include "MachineCode.hpp"
#include "Argument.hpp"
#include <cassert>

class MachineCode;
class Argument;

class Instruction {
  public:
    virtual ~Instruction() = 0;
    virtual void translate(MachineCode& code) const = 0;
    virtual void translate(MachineCode& code, const Argument& arg1, const Argument& arg2) const {}
};

class NegInstruction : public Instruction {
  void translate(MachineCode& code) const {
    code.add(0xed);
    code.add(0x44);
  }
};

class CplInstruction : public Instruction {
  void translate(MachineCode& code) const {
    code.add(0x2f);
  }
};

class LdInstruction : public Instruction {
  void translate(MachineCode& code) const {
    assert(false); // TODO
  }

  void translate(MachineCode& code, const Argument& arg1, const Argument& arg2) const {
    // TODO ignore arg1 for know and hardcode regA
    code.add(0b00111110);
    assert(arg2.type == VALUE_ARGUMENT);
    // TODO: should check size of value
    Byte byte = (Byte)arg2.value;
    code.add(byte);
  }
};

