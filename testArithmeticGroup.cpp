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

void testOr() {
  Assembler assembler;
  assembler.command1("or", identifierArg("a"));
  Byte expectedBytes[] = { 0b10110111 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

}

void testArithmeticGroup() {
  testAddHlBc();
  testOr();
}
