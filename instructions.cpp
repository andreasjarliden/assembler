#include "instructions.hpp"
#include "MachineCode.hpp"
#include "Argument.hpp"
#include <cassert>

void myNegInstruction(MachineCode& code) {
  code.add(0xed);
  code.add(0x44);
}

void myCplInstruction(MachineCode& code) {
  code.add(0x2f);
}

void myLdInstruction(const Argument& arg1, const Argument& arg2, MachineCode& code) {
  // TODO ignore arg1 for know and hardcode regA
  code.add(0b00111110);
  assert(arg2.type == VALUE_ARGUMENT);
  // TODO: should check size of value
  Byte byte = (Byte)arg2.value;
  code.add(byte);
}

