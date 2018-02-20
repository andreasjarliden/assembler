#include "testArithmeticGroup.hpp"
#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "argumentHelpers.hpp"
#include "testUtilities.hpp"

namespace {

void testAddHlBc() {
  Assembler assembler;
  assembler.command2("add", identifierArg("hl"), identifierArg("bc"));
  Byte expectedBytes[] = { 0b00001001 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void testAdd_A_nn() {
  Assembler assembler;
  assembler.command2("add", identifierArg("a"), numberArg(123));
  Byte expectedBytes[] = { 0xc6, 123 };
  isEqualToBytes(assembler, expectedBytes, 2);
}


void testOr() {
  Assembler assembler;
  assembler.command1("or", identifierArg("a"));
  Byte expectedBytes[] = { 0b10110111 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void testAnd_nn() {
  Assembler assembler;
  assembler.command1("and", numberArg(0x13));
  Byte expectedBytes[] = { 0xe6, 0x13 };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void testSrl() {
  Assembler assembler;
  assembler.command1("srl", identifierArg("c"));
  Byte expectedBytes[] = { 0xcb, 0b00111001 };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void testDec_ss() {
  Assembler assembler;
  assembler.command1("dec", identifierArg("sp"));
  Byte expectedBytes[] = { 0b00111011 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void testDec_r() {
  Assembler assembler;
  assembler.command1("dec", identifierArg("a"));
  Byte expectedBytes[] = { 0b00111101 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

} // unnamed namespace

void testArithmeticGroup() {
  testAdd_A_nn();
  testAddHlBc();
  testOr();
  testAnd_nn();
  testSrl();
  testDec_r();
  testDec_ss();
}

