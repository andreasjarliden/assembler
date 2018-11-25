#include "MachineCode.hpp"
#include "LabelTable.hpp"
#include "Argument.hpp"
#include "errorChecking.hpp"
#include "InstructionsHost.hpp"
#include "DelayedAddresses.hpp"
#include "Error.hpp"
#include <cassert>
#include <sstream>
#include <cstring>

namespace {

Byte registerBits(const Argument& arg1) {
  assert(arg1.type() == IDENTIFIER_ARGUMENT);
  if (strlen(arg1.identifier()) != 1) {
    throw Error(std::string("Expected 8 bit register, got ") + arg1.identifier());
  }
  switch (arg1.identifier()[0]) {
    case 'a':
    case 'A': return 0b111;
    case 'b':
    case 'B': return 0b000;
    case 'c':
    case 'C': return 0b001;
    case 'd':
    case 'D': return 0b010;
    case 'e':
    case 'E': return 0b011;
    case 'h':
    case 'H': return 0b100;
    case 'l':
    case 'L': return 0b101;
  }
  throw Error(std::string("Expected 8 bit register, got ") + arg1.identifier());
}

Byte register16BitsQQ(const Argument& arg) {
  assert(arg.type() == IDENTIFIER_ARGUMENT);
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
  if (strcmp(arg.identifier(), "af") == 0)
    return 0b11 << 4;
  if (strcmp(arg.identifier(), "AF") == 0)
    return 0b11 << 4;
  throw Error(std::string("Expected 16 bit register bc, de, hl, or af, got ") + arg.identifier());
}


Byte register16Bits(const Argument& arg) {
  assert(arg.type() == IDENTIFIER_ARGUMENT);
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
  throw Error(std::string("Expected 16 bit register bc, de, hl, or sp, got ") + arg.identifier());
}

Byte conditionBits(const Argument& arg) {
  assert(arg.type() == IDENTIFIER_ARGUMENT);
  if (strcmp(arg.identifier(), "nz") == 0)
    return 0b000 << 3;
  if (strcmp(arg.identifier(), "z") == 0)
    return 0b001 << 3;
  if (strcmp(arg.identifier(), "nc") == 0)
    return 0b010 << 3;
  if (strcmp(arg.identifier(), "c") == 0)
    return 0b011 << 3;
  if (strcmp(arg.identifier(), "po") == 0)
    return 0b100 << 3;
  if (strcmp(arg.identifier(), "pe") == 0)
    return 0b101 << 3;
  if (strcmp(arg.identifier(), "p") == 0)
    return 0b110 << 3;
  if (strcmp(arg.identifier(), "m") == 0)
    return 0b111 << 3;
  throw Error(std::string("Expected 16 bit register, got ") + arg.identifier());
}

} // unnamed namespace

void nopInstruction(InstructionsHost& host) {
  host.addCode(0x00);
}

void diInstruction(InstructionsHost& host) {
  host.addCode(0xf3);
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

void retInstruction(InstructionsHost& host) {
  host.addCode(0xc9);
}

void retiInstruction(InstructionsHost& host) {
  host.addCode(0xed);
  host.addCode(0x4d);
}

void cplInstruction(InstructionsHost& host) {
  host.addCode(0x2f);
}

void ldInstruction(InstructionsHost& host, const Argument& arg1, const Argument& arg2) {
  if (arg1.is8BitRegister(NOT_DEREFERENCED) && arg2.is8BitRegister(NOT_DEREFERENCED)) {
    // ld r, r'
    host.addCode(0b01000000 | registerBits(arg1) << 3 | registerBits(arg2));
  }
  else if (arg1.isI(NOT_DEREFERENCED) && arg2.isA(NOT_DEREFERENCED)) {
    // ld I, A
    host.addCode(0xed);
    host.addCode(0x47);
  }
  // TODO this must currently come before ld A, nn as we treat registers as any identifiers (and in this case a value).
  else if (arg1.isA(NOT_DEREFERENCED) && arg2.isI(NOT_DEREFERENCED)) {
    // ld A, I
    host.addCode(0xed);
    host.addCode(0x57);
  }
  else if (arg1.is8BitRegister(NOT_DEREFERENCED) && arg2.isValue()) {
    // ld r, n
    host.addCode(0b00000110 | registerBits(arg1) << 3);
    host.addCode(arg2.byteValue());
  }
  else if (arg1.is8BitRegister(NOT_DEREFERENCED) && arg2.isHL(DEREFERENCED)) {
    // ld r, (HL)
    host.addCode(0b01000110 | registerBits(arg1) << 3);
  }
  else if (arg1.isHL(DEREFERENCED) && arg2.is8BitRegister(NOT_DEREFERENCED)) {
    // ld (hl), r
    host.addCode(0b01110000 | registerBits(arg2));
  }
  else if (arg1.isDE(DEREFERENCED) && arg2.isA(NOT_DEREFERENCED)) {
    // ld (DE), A
    host.addCode(0x12);
  }
  else if (arg1.isDereferenced() && arg2.isA(NOT_DEREFERENCED)) {
    // LD (nn), A
    host.addCode(0x32);
    host.add16BitAddress(arg1);
  }
  // TODO ld a, (DE) must currently come before ld a, (nn)
  else if (arg1.isA(NOT_DEREFERENCED) && arg2.isDE(DEREFERENCED)) {
    // ld a, (DE)
    host.addCode(0x1a);
  }
  else if (arg1.isA(NOT_DEREFERENCED) && arg2.isDereferenced()) {
    // LD A, (nn)
    host.addCode(0x3a);
    host.add16BitAddress(arg2);
  }
  else if (arg1.isDereferenced() && arg2.isHL(NOT_DEREFERENCED)) {
    // LD (nn), hl
    host.addCode(0x22);
    host.add16BitAddress(arg1);
  }
  else if (arg1.isHL(NOT_DEREFERENCED) && arg2.isDereferenced()) {
    // ld hl, (nn)
    host.addCode(0x2a);
    host.add16BitAddress(arg2);
  }
  else if (arg1.is16BitRegister(NOT_DEREFERENCED) && arg2.isValue()) {
    // LD dd, nn
    host.addCode(0b00000001 | register16Bits(arg1));
    host.add16BitAddress(arg2);
  }
  else {
    std::stringstream what;
    what << "Unknown form of LD instruction\n\tArg 1:" << arg1 << "\n\t" << arg2;
    throw Error(what.str());
  }
}

void addInstruction(InstructionsHost& host, const Argument& arg1, const Argument& arg2) {
  if (arg1.isHL()) {
    host.addCode(0b00001001 | register16Bits(arg2));
    return;
  }
  if (arg1.isA()) {
    if (arg2.is8BitRegister()) {
      host.addCode(0b10000000 | registerBits(arg2));
      return;
    }
    else {
      host.addCode(0xc6);
      Byte byte = arg2.byteValue();
      host.addCode(byte);
      return;
    }
  }
  error("Uknown form of add instruction");
}

void orInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0b10110000 | registerBits(arg));
}

void andInstruction(InstructionsHost& host, const Argument& arg) {
  if (arg.isValue()) {
    host.addCode(0xe6);
    host.addCode(arg.byteValue());
    return;
  }
  error("Unknown form of AND instruction");
}

void srlInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0b11001011);
  host.addCode(0b00111000 | registerBits(arg));
}

void sbcInstruction(InstructionsHost& host, const Argument& arg1, const Argument& arg2) {
  if (!arg1.isHL()) {
    error("First argument to sbc instruction should be hl register");
    return;
  }
  host.addCode(0xed);
  host.addCode(0b01000010 | register16Bits(arg2));
}

void pushInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0b11000101 | register16BitsQQ(arg));
}

void popInstruction(InstructionsHost& host, const Argument& arg) {
  if (arg.isIX()) {
    host.addCode(0xdd);
    host.addCode(0xe1);
  }
  else if (arg.isIY()) {
    host.addCode(0xdd);
    host.addCode(0xfd);
  }
  else {
    host.addCode(0b11000001 | register16BitsQQ(arg));
  }
}

void outInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xd3);
  host.addCode(arg.ioAddress());
}

void inInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xdb);
  host.addCode(arg.ioAddress());
}

void jrUnaryInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0x18);
  host.add8BitRelativeAddress(arg);
}

void jpUnaryInstruction(InstructionsHost& host, const Argument& arg) {
  if (arg.isDereferenced() && arg.isHL()) {
    host.addCode(0xe9);
  }
  else {
    host.addCode(0xc3);
    host.add16BitAddress(arg);
  }
}

void jpBinaryInstruction(InstructionsHost& host, const Argument& arg1, const Argument& arg2) {
  host.addCode(0b11000010 | conditionBits(arg1));
  host.add16BitAddress(arg2);
}

void jrBinaryInstruction(InstructionsHost& host, const Argument& arg1, const Argument& arg2) {
  if (arg1.isFlag("z")) {
    host.addCode(0x28);
  }
  else if (arg1.isFlag("nz")) {
    host.addCode(0x20);
  }
  else {
    throw Error(std::string("Uknown flag to JR instruction: ") + arg1.identifier());
  }
  host.add8BitRelativeAddress(arg2);
}

void callInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0xcd);
  host.add16BitAddress(arg);
}

void imInstruction(InstructionsHost& host, const Argument& arg) {
  // TODO: add support for mode 0
  if (arg.value() == 1) {
    host.addCode(0xed);
    host.addCode(0x56);
  }
  else if (arg.value() == 2) {
    host.addCode(0xed);
    host.addCode(0x5e);
  }
  else {
    throw Error("IM instruction with unhandled value");
  }
}

void cpInstruction(InstructionsHost& host, const Argument& arg) {
  if (arg.is8BitRegister()) {
    host.addCode(0b10111000 | registerBits(arg));
  }
  else if (arg.isValue()) {
    host.addCode(0xfe);
    host.addCode(arg.byteValue());
  }
  else {
    assert(false);
  }
}

void incInstruction(InstructionsHost& host, const Argument& arg) {
  host.addCode(0b00000011 | register16Bits(arg));
}

void exInstruction(InstructionsHost& host, const Argument& arg1, const Argument& arg2) {
  if (arg1.isDE() && arg2.isHL()) {
    host.addCode(0xeb);
  }
  else {
    error("Unknown form of EX instruction");
  }
}


void decInstruction(InstructionsHost& host, const Argument& arg) {
  if (arg.is8BitRegister()) {
    host.addCode(0b00000101 | registerBits(arg) << 3);
  }
  else if (arg.is16BitRegister()) {
    host.addCode(0b00001011 | register16Bits(arg));
  }
  else {
    throw Error("Unknown form of DEC instruction");
  }
}

void cpirInstruction(InstructionsHost& host) {
  host.addCode(0xed);
  host.addCode(0xb1);
}
