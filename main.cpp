#include "Commands.hpp"
#include "Argument.hpp"
#include "argumentHelpers.hpp"
#include "MachineCode.hpp"
#include "Assembler.hpp"
#include <cassert>

extern "C" {

int yyparse();

}

Assembler ASSEMBLER;
Commands& COMMANDS = ASSEMBLER;

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

void testNoSuchInstruction() {}

int main() {
  testSingleOneByteInstruction();
  testSingleTwoByteInstruction();
  testTwoInstructions();
  test_ld_a_byte();
  yyparse();
  ASSEMBLER.machineCode().print();
  return 0;
}

