#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "argumentHelpers.hpp"
#include <cassert>
#include <iostream>

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

void test_ld_a_using_eq() {
  Assembler assembler;
  assembler.metaCommand2("eq", "FOUR_TWO", numberArg(42));
  assembler.command2("ld", identifierArg("a"), identifierArg("FOUR_TWO"));
  Byte expectedBytes[] = { 0x3e, 42 };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void testNoSuchInstruction() {}

int main() {
  testSingleOneByteInstruction();
  testSingleTwoByteInstruction();
  testTwoInstructions();
  test_ld_a_byte();
  test_ld_d_byte();
  test_out_20();
  testJpToNumericAddress();
  testJpBackwardsToLabel();
  test_ld_a_using_eq();
  std::cout << "Test passed" << std::endl;
}
