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
  assembler.command2("ld", identifierArg("a"), byteArg(255));
  Byte expectedBytes[] = { 0x3e, 0xff };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void test_ld_d_byte() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("L"), byteArg(255));
  Byte expectedBytes[] = { 0x2e, 0xff };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void test_out_20() {
  Assembler assembler;
  assembler.command1("out", addressArg(255));
  Byte expectedBytes[] = { 0xd3, 0xff };
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
  std::cout << "Test passed" << std::endl;
}
