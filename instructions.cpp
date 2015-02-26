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
  assert(arg1.type() == IDENTIFIER_ARGUMENT);
  if (strlen(arg1.identifier()) != 1) {
    throw Error(std::string("Expected 8 bit register, got ") + arg1.identifier());
  }
  switch (arg1.identifier()[0]) {
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
  throw Error(std::string("Expected 8 bit register, got ") + arg1.identifier());
}

Byte register16Bits(const Argument& arg) {
  assert(arg.type() == IDENTIFIER_ARGUMENT);
  if (strlen(arg.identifier()) != 2) {
    throw Error(std::string("Expected 16 bit register, got ") + arg.identifier());
  }
  if (strcmp(arg.identifier(), "bc") == 0)
    return 0b00 << 4;
  if (strcmp(arg.identifier(), "BC") == 0)
    return 0b00 << 4;
  if (strcmp(arg.identifier(), "de") == 0)
    return 0b01 << 4;
  if (strcmp(arg.identifier(), "DE") == 0)
    return 0b01 << 4;
  if (strcmp(arg.identifier(), "hl") == 0)
    return 0b10 << 4;
  if (strcmp(arg.identifier(), "HL") == 0)
    return 0b10 << 4;
  if (strcmp(arg.identifier(), "sp") == 0)
    return 0b11 << 4;
  if (strcmp(arg.identifier(), "SP") == 0)
    return 0b11 << 4;
  throw Error(std::string("Expected 16 bit register, got ") + arg.identifier());
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

void ldInstruction(InstructionsHost& host, const Argument& arg1, const Argument& arg2) {
  if (arg1.isAddress()) {
    // ld (nn), A
    host.addCode(0x32);
    // TODO much duplication with jp
    int address;
    if (arg1.type() == ADDRESS_VALUE_ARGUMENT) {
      address = arg1.value();
    }
    else {
      address = host.addressForLabel(arg1.identifier());
    }
    Byte low = (Byte)address & 0xff;
    Byte high = (Byte)((address >> 8) & 0xff);
    host.addCode(low);
    host.addCode(high);
  }
  else {
    if (arg1.is8BitRegister()) {
      // ld r, n
      host.addCode(0b00000110 | registerBits(arg1));
      verifyIsValueArgument(arg2, 2);
      // TODO: should check size of value
      Byte byte = (Byte)arg2.value();
      host.addCode(byte);
    }
    else if (arg1.is16BitRegister()) {
      host.addCode(0b00000001 | register16Bits(arg1));
      int value = arg2.value();
      Byte low = (Byte)value & 0xff;
      Byte high = (Byte)((value >> 8) & 0xff);
      host.addCode(low);
      host.addCode(high);
    }
    else
      throw Error("Unknown form of LD instruction");
  }
}

void outInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xd3);
  assert(arg.type() == ADDRESS_VALUE_ARGUMENT);
  assert(arg.value() >= 0);
  assert(arg.value() <= 255);
  Byte byte = (Byte)arg.value();
  host.addCode(byte);
}

void inInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xdb);
  assert(arg.type() == ADDRESS_VALUE_ARGUMENT);
  assert(arg.value() >= 0);
  assert(arg.value() <= 255);
  Byte byte = (Byte)arg.value();
  host.addCode(byte);
}

void jpInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xc3);
  int address;
  if (arg.type() == VALUE_ARGUMENT) {
    address = arg.value();
  }
  else {
    if (host.containsLabel(arg.identifier())) {
      address = host.addressForLabel(arg.identifier());
    }
    else {
      address = 0;
      host.addDelayed16BitValue(arg.identifier());
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
  assert(arg.value() == 1);
  host.addCode(0xed);
  host.addCode(0x56);
}



