#include "MachineCode.hpp"
#include "LabelTable.hpp"
#include "Argument.hpp"
#include "errorChecking.hpp"
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

void nopInstruction(MachineCode& code) {
  code.add(0x00);
}

void eiInstruction(MachineCode& code) {
  code.add(0xfb);
}

void haltInstruction(MachineCode& code) {
  code.add(0x76);
}

void negInstruction(MachineCode& code) {
  code.add(0xed);
  code.add(0x44);
}

void retiInstruction(MachineCode& code) {
  code.add(0xed);
  code.add(0x4d);
}

void cplInstruction(MachineCode& code) {
  code.add(0x2f);
}

void ldInstruction(const Argument& arg1, const Argument& arg2, MachineCode& code, const LabelTable&) {
  code.add(0b00000110 | registerBits(arg1));
  verifyIsValueArgument(arg2, 2);
  // TODO: should check size of value
  Byte byte = (Byte)arg2.value;
  code.add(byte);
}

void outInstruction(const Argument& arg, MachineCode& code, const LabelTable&) {
  code.add(0xd3);
  assert(arg.type == ADDRESS_VALUE_ARGUMENT);
  assert(arg.value >= 0);
  assert(arg.value <= 255);
  Byte byte = (Byte)arg.value;
  code.add(byte);
}

void inInstruction(const Argument& arg, MachineCode& code, const LabelTable&) {
  code.add(0xdb);
  assert(arg.type == ADDRESS_VALUE_ARGUMENT);
  assert(arg.value >= 0);
  assert(arg.value <= 255);
  Byte byte = (Byte)arg.value;
  code.add(byte);
}

void jpInstruction(const Argument& arg, MachineCode& code, const LabelTable& table) {
  code.add(0xc3);
  int address;
  if (arg.type == VALUE_ARGUMENT) {
    address = arg.value;
  }
  else {
    address = table.addressForLabel(arg.identifier);
  }
  Byte low = (Byte)address & 0xff;
  Byte high = (Byte)((address >> 8) & 0xff);
  code.add(low);
  code.add(high);
}

void imInstruction(const Argument& arg, MachineCode& code, const LabelTable&) {
  verifyIsValueArgument(arg, 1);
  // TODO: add support for mode 0 and 2
  assert(arg.value == 1);
  code.add(0xed);
  code.add(0x56);
}



