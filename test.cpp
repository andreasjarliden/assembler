#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "argumentHelpers.hpp"
#include "DelayedAddresses.hpp"
#include <cassert>
#include <iostream>

int yylineno; // Usually defined by lexer
const char* filename = "test.txt";

void testSingleOneByteInstruction() {
  Assembler assembler;
  assembler.command0("cpl");
  Byte expectedBytes[] = { 0x2f };
  assert(assembler.machineCode().isEqual(expectedBytes, 1));
}

void testSingleTwoByteInstruction() {
  Assembler assembler;
  assembler.command0("neg");
  Byte expectedBytes[] = { 0xed, 0x44 };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void testTwoInstructions() {
  Assembler assembler;
  assembler.command0("cpl");
  assembler.command0("neg");
  Byte expectedBytes[] = { 0x2f, 0xed, 0x44 };
  assert(assembler.machineCode().isEqual(expectedBytes, 3));
}

void test_ld_a_byte() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("a"), numberArg(255));
  Byte expectedBytes[] = { 0x3e, 0xff };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void test_ld_d_byte() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("L"), numberArg(255));
  Byte expectedBytes[] = { 0x2e, 0xff };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void test_ld_hl_word() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("HL"), numberArg(0x1234));
  Byte expectedBytes[] = { 0x21, 0x34, 0x12 };
  assert(assembler.machineCode().isEqual(expectedBytes, 3));
}

void test_ld_addr_a() {
  Assembler assembler;
  assembler.command2("ld", addressArg(0x1234), identifierArg("a"));
  Byte expectedBytes[] = { 0x32, 0x34, 0x12 };
  assert(assembler.machineCode().isEqual(expectedBytes, 3));
}

void test_out_20() {
  Assembler assembler;
  assembler.command1("out", addressArg(255));
  Byte expectedBytes[] = { 0xd3, 0xff };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void testJpToNumericAddress() {
  Assembler assembler;
  assembler.command1("jp", numberArg(511));
  Byte expectedBytes[] = { 0xc3, 0xff, 0x01 };
  assert(assembler.machineCode().isEqual(expectedBytes, 3));
}

void testJpBackwardsToLabel() {
  Assembler assembler;
  assembler.label("aLabel");
  assembler.command1("jp", identifierArg("aLabel"));
  Byte expectedBytes[] = { 0xc3, 0x00, 0x00 };
  assert(assembler.machineCode().isEqual(expectedBytes, 3));
}

void testJpForwardsToLabel() {
  Assembler assembler;
  assembler.command1("jp", identifierArg("aLabel"));
  assembler.label("aLabel");
  assembler.resolveRemaining();
  Byte expectedBytes[] = { 0xc3, 0x03, 0x00 };
  assert(assembler.machineCode().isEqual(expectedBytes, 3));
}

void test_ld_a_using_eq() {
  Assembler assembler;
  assembler.metaCommand2("eq", "FOUR_TWO", numberArg(42));
  assembler.command2("ld", identifierArg("a"), identifierArg("FOUR_TWO"));
  Byte expectedBytes[] = { 0x3e, 42 };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void test_out_using_eq() {
  Assembler assembler;
  assembler.metaCommand2("eq", "FOUR_TWO", numberArg(42));
  assembler.command1("out", addressIdentifierArg("FOUR_TWO"));
  Byte expectedBytes[] = { 0xd3, 42 };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void test_cp_c() {
  Assembler assembler;
  assembler.command1("cp", identifierArg("c"));
  Byte expectedBytes[] = { 0b10111001 };
  assert(assembler.machineCode().isEqual(expectedBytes, 1));
}


void testNoSuchInstruction() {}

int main() {
  testSingleOneByteInstruction();
  testSingleTwoByteInstruction();
  testTwoInstructions();
  test_ld_a_byte();
  test_ld_d_byte();
  test_ld_hl_word();
  test_ld_addr_a();
  test_out_20();
  testJpToNumericAddress();
  testJpBackwardsToLabel();
  testJpForwardsToLabel();
  test_ld_a_using_eq();
  test_out_using_eq();
  test_cp_c();
  std::cout << "Test passed" << std::endl;
}
