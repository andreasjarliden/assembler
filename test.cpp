#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "argumentHelpers.hpp"
#include "testUtilities.hpp"
#include "testJumpGroup.hpp"
#include "testLoadGroup.hpp"
#include <iostream>

int yylineno; // Usually defined by lexer
const char* filename = "test.txt";

void testSingleOneByteInstruction() {
  Assembler assembler;
  assembler.command0("cpl");
  Byte expectedBytes[] = { 0x2f };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void testSingleTwoByteInstruction() {
  Assembler assembler;
  assembler.command0("neg");
  Byte expectedBytes[] = { 0xed, 0x44 };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void testTwoInstructions() {
  Assembler assembler;
  assembler.command0("cpl");
  assembler.command0("neg");
  Byte expectedBytes[] = { 0x2f, 0xed, 0x44 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void test_out_20() {
  Assembler assembler;
  assembler.command1("out", addressArg(255));
  Byte expectedBytes[] = { 0xd3, 0xff };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_out_using_eq() {
  Assembler assembler;
  assembler.metaCommand2("eq", "FOUR_TWO", numberArg(42));
  assembler.command1("out", addressIdentifierArg("FOUR_TWO"));
  Byte expectedBytes[] = { 0xd3, 42 };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_cp_c() {
  Assembler assembler;
  assembler.command1("cp", identifierArg("c"));
  Byte expectedBytes[] = { 0b10111001 };
  isEqualToBytes(assembler, expectedBytes, 1);
}


void test_inc_hl() {
  Assembler assembler;
  assembler.command1("inc", identifierArg("hl"));
  Byte expectedBytes[] = { 0b00100011 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void test_dec_b() {
  Assembler assembler;
  assembler.command1("dec", identifierArg("b"));
  Byte expectedBytes[] = { 0b00000101 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void testNoSuchInstruction() {}

int main() {
  testSingleOneByteInstruction();
  testSingleTwoByteInstruction();
  testTwoInstructions();
  test_out_20();
  test_out_using_eq();
  test_cp_c();
  test_inc_hl();
  test_dec_b();
  testLoadGroup();
  testJumpGroup();
  std::cout << "Test passed" << std::endl;
}
