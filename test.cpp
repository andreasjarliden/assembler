#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "Segments.hpp"
#include "Segment.hpp"
#include "argumentHelpers.hpp"
#include "printHex.hpp"
#include "testUtilities.hpp"
#include "testJumpGroup.hpp"
#include "testLoadGroup.hpp"
#include "testArithmeticGroup.hpp"
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
  assembler.command1("out", dereferencedValueArg(255));
  Byte expectedBytes[] = { 0xd3, 0xff };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_out_using_eq() {
  Assembler assembler;
  assembler.metaCommand2("eq", "FOUR_TWO", numberArg(42));
  assembler.command1("out", dereferencedIdentifierArg("FOUR_TWO"));
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

void testOneSegment() {
  Assembler assembler;
  assembler.command0("cpl");
  assert(assembler.segments().numberOfSegments() == 1);
}

void testTwoSegments() {
  Assembler assembler;
  assembler.command0("cpl");
  assembler.metaCommand1("org", dereferencedValueArg(10));
  assembler.command0("cpl");

  assert(assembler.segments().numberOfSegments() == 2);
  Byte expectedBytes[] = { 0x2f };
  isEqualToBytes(assembler.segments().index(0), expectedBytes, 1);
  // isEqualToBytes(assembler.segments().index(1), expectedBytes, 1);
}

void testPrintTwoSegments() {
  Segments segments;
  segments.index(0).add(0x42);
  segments.addSegment(0x0010);
  segments.index(1).add(0x42);
  segments.index(1).add(0x06);

  equalStrings(asHexString(segments),
      "0000: 42 \n"
      "0010: 42 06 \n");
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
  testArithmeticGroup();
  testOneSegment();
  testTwoSegments();
  testPrintTwoSegments();
  std::cout << "Test passed" << std::endl;
}
