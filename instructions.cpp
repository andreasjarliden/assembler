#include "instructions.hpp"
#include "MachineCode.hpp"
#include "Argument.hpp"
#include <cassert>

namespace {

Byte registerBits(const Argument& arg1) {
  assert(arg1.type == IDENTIFIER_ARGUMENT);
  assert(strlen(arg1.identifier) == 1);
  switch (arg1.identifier[0]) {
    case 'a':
    case 'A': return 0b111 << 3;
    case 'b':
    case 'B': return 0b000 << 3;
    case 'c':
    case 'C': return 0b001 << 3;
    case 'd':
    case 'D': return 0b010 << 3;
    case 'e':
    case 'E': return 0b011 << 3;
    case 'h':
    case 'H': return 0b100 << 3;
    case 'l':
    case 'L': return 0b101 << 3;
  }
   assert(false);
}

} // unnamed namespace

void myNegInstruction(MachineCode& code) {
  code.add(0xed);
  code.add(0x44);
}

void myCplInstruction(MachineCode& code) {
  code.add(0x2f);
}

void myLdInstruction(const Argument& arg1, const Argument& arg2, MachineCode& code) {
  code.add(0b00000110 | registerBits(arg1));
  assert(arg2.type == VALUE_ARGUMENT);
  // TODO: should check size of value
  Byte byte = (Byte)arg2.value;
  code.add(byte);
}

void outInstruction(const Argument& arg, MachineCode& code) {
  code.add(0xd3);
  assert(arg.type == ADDRESS_ARGUMENT);
  assert(arg.value >= 0);
  assert(arg.value <= 255);
  Byte byte = (Byte)arg.value;
  code.add(byte);
}


