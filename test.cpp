#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "argumentHelpers.hpp"
#include "DelayedAddresses.hpp"
#include <cassert>
#include <iostream>

int yylineno; // Usually defined by lexer
const char* filename = "test.txt";

namespace {

void isEqualToBytes(const Assembler& assembler,
    Byte expectedBytes[],
    int numberOfBytes) {
  if (!assembler.machineCode().isEqual(expectedBytes, numberOfBytes)) {
    std::cout << "Expected \n\t" << std::hex;
    std::copy(expectedBytes, expectedBytes + numberOfBytes, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "but received \n\t" << std::hex;
    std::copy(assembler.machineCode().beginCode(), assembler.machineCode().endCode(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    exit(-1);
  }
}

} // unnamed namespace

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

void test_ld_a_byte() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("a"), numberArg(255));
  Byte expectedBytes[] = { 0x3e, 0xff };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_ld_d_byte() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("L"), numberArg(255));
  Byte expectedBytes[] = { 0x2e, 0xff };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_ld_hl_word() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("HL"), numberArg(0x1234));
  Byte expectedBytes[] = { 0x21, 0x34, 0x12 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void test_ld_addr_a() {
  Assembler assembler;
  assembler.command2("ld", addressArg(0x1234), identifierArg("a"));
  Byte expectedBytes[] = { 0x32, 0x34, 0x12 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void test_out_20() {
  Assembler assembler;
  assembler.command1("out", addressArg(255));
  Byte expectedBytes[] = { 0xd3, 0xff };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void testJpToNumericAddress() {
  Assembler assembler;
  assembler.command1("jp", numberArg(511));
  Byte expectedBytes[] = { 0xc3, 0xff, 0x01 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void testJpBackwardsToLabel() {
  Assembler assembler;
  assembler.label("aLabel");
  assembler.command1("jp", identifierArg("aLabel"));
  Byte expectedBytes[] = { 0xc3, 0x00, 0x00 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void testJpForwardsToLabel() {
  Assembler assembler;
  assembler.command1("jp", identifierArg("aLabel"));
  assembler.label("aLabel");
  assembler.resolveRemaining();
  Byte expectedBytes[] = { 0xc3, 0x03, 0x00 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void test_ld_a_using_eq() {
  Assembler assembler;
  assembler.metaCommand2("eq", "FOUR_TWO", numberArg(42));
  assembler.command2("ld", identifierArg("a"), identifierArg("FOUR_TWO"));
  Byte expectedBytes[] = { 0x3e, 42 };
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

void test_jp_z_nn() {
  Assembler assembler;
  assembler.command2("jp", identifierArg("z"), numberArg(0x1234));
  Byte expectedBytes[] = { 0b11001010, 0x34, 0x12 };
  isEqualToBytes(assembler, expectedBytes, 3);
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

void test_ld_c_d() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("c"), identifierArg("d"));
  Byte expectedBytes[] = { 0b01001010 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void test_ld_adr_hl_a() {
  Assembler assembler;
  assembler.command2("ld", addressIdentifierArg("hl"), identifierArg("a"));
  Byte expectedBytes[] = { 0b01110111 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void test_jr_number_address() {
  Assembler assembler;
  assembler.command1("jr", numberArg(0));
  Byte expectedBytes[] = { 0x18, 0xfe };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_jr_forward_label() {
  Assembler assembler;
  assembler.command1("jr", identifierArg("label"));
  assembler.command0("nop");
  assembler.label("label");
  assembler.resolveRemaining();
  Byte expectedBytes[] = { 0x18, 0x01, 0x00 };
  isEqualToBytes(assembler, expectedBytes, 3);
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
  test_ld_c_d();
  test_ld_adr_hl_a();
  test_out_20();
  testJpToNumericAddress();
  testJpBackwardsToLabel();
  testJpForwardsToLabel();
  test_ld_a_using_eq();
  test_out_using_eq();
  test_cp_c();
  test_jp_z_nn();
  test_inc_hl();
  test_dec_b();
  test_jr_number_address();
  test_jr_forward_label();
  std::cout << "Test passed" << std::endl;
}
