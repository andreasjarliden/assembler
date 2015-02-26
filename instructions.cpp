#include "MachineCode.hpp"
#include "LabelTable.hpp"
#include "Argument.hpp"
#include "errorChecking.hpp"
#include "InstructionsHost.hpp"
#include "DelayedAddresses.hpp"
#include "Error.hpp"
#include <cassert>

namespace {

Byte registerBits(const Argument& arg1) {
  assert(arg1.type == IDENTIFIER_ARGUMENT);
  if (strlen(arg1.identifier) != 1) {
    throw Error(std::string("Expected 8 bit register, got ") + arg1.identifier);
  }
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
  throw Error(std::string("Expected 8 bit register, got ") + arg1.identifier);
}

} // unnamed namespace

void nopInstruction(InstructionsHost& host) {
  host.addCode(0x00);
}

void eiInstruction(InstructionsHost& host) {
  host.addCode(0xfb);
}

void haltInstruction(InstructionsHost& host) {
  host.addCode(0x76);
}

void negInstruction(InstructionsHost& host) {
  host.addCode(0xed);
  host.addCode(0x44);
}

void retiInstruction(InstructionsHost& host) {
  host.addCode(0xed);
  host.addCode(0x4d);
}

void cplInstruction(InstructionsHost& host) {
  host.addCode(0x2f);
}

bool isAddress(const Argument& arg) {
  return arg.type == ADDRESS_VALUE_ARGUMENT || arg.type == ADDRESS_IDENTIFIER_ARGUMENT;
}

void ldInstruction(InstructionsHost& host, const Argument& arg1, const Argument& arg2) {
  if (isAddress(arg1)) {
    // ld (nn), A
    host.addCode(0x32);
    // TODO much duplication with jp
    int address;
    if (arg1.type == ADDRESS_VALUE_ARGUMENT) {
      address = arg1.value;
    }
    else {
      address = host.addressForLabel(arg1.identifier);
    }
    Byte low = (Byte)address & 0xff;
    Byte high = (Byte)((address >> 8) & 0xff);
    host.addCode(low);
    host.addCode(high);
  }
  else {
    // ld r, n
    host.addCode(0b00000110 | registerBits(arg1));
    verifyIsValueArgument(arg2, 2);
    // TODO: should check size of value
    Byte byte = (Byte)arg2.value;
    host.addCode(byte);
  }
}

void outInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xd3);
  assert(arg.type == ADDRESS_VALUE_ARGUMENT);
  assert(arg.value >= 0);
  assert(arg.value <= 255);
  Byte byte = (Byte)arg.value;
  host.addCode(byte);
}

void inInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xdb);
  assert(arg.type == ADDRESS_VALUE_ARGUMENT);
  assert(arg.value >= 0);
  assert(arg.value <= 255);
  Byte byte = (Byte)arg.value;
  host.addCode(byte);
}

void jpInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xc3);
  int address;
  if (arg.type == VALUE_ARGUMENT) {
    address = arg.value;
  }
  else {
    if (host.containsLabel(arg.identifier)) {
      address = host.addressForLabel(arg.identifier);
    }
    else {
      address = 0;
      host.addDelayed16BitValue(arg.identifier);
    }
  }
  Byte low = (Byte)address & 0xff;
  Byte high = (Byte)((address >> 8) & 0xff);
  host.addCode(low);
  host.addCode(high);
}

void imInstruction(InstructionsHost& host, const Argument& arg) {
  verifyIsValueArgument(arg, 1);
  // TODO: add support for mode 0 and 2
  assert(arg.value == 1);
  host.addCode(0xed);
  host.addCode(0x56);
}



