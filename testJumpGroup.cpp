#include "testJumpGroup.hpp"
#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "argumentHelpers.hpp"
#include "testUtilities.hpp"

namespace {

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

void test_jp_z_nn() {
  Assembler assembler;
  assembler.command2("jp", identifierArg("z"), numberArg(0x1234));
  Byte expectedBytes[] = { 0b11001010, 0x34, 0x12 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void test_jr_number_address() {
  Assembler assembler;
  assembler.command1("jr", numberArg(0));
  Byte expectedBytes[] = { 0x18, 0xfe };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_jr_backwards_label() {
  Assembler assembler;
  assembler.label("label");
  assembler.command1("jr", identifierArg("label"));
  assembler.resolveRemaining();
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

void test_jr_z_number_address() {
  Assembler assembler;
  assembler.command2("jr", identifierArg("Z"), numberArg(0));
  Byte expectedBytes[] = { 0x28, 0xfe };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_jr_nz_number_address() {
  Assembler assembler;
  assembler.command2("jr", identifierArg("nz"), numberArg(0));
  Byte expectedBytes[] = { 0x20, 0xfe };
  isEqualToBytes(assembler, expectedBytes, 2);
}

} // anonymous namespace

void testJumpGroup() {
  testJpToNumericAddress();
  testJpBackwardsToLabel();
  testJpForwardsToLabel();
  test_jp_z_nn();
  test_jr_number_address();
  test_jr_backwards_label();
  test_jr_forward_label();
  test_jr_z_number_address();
  test_jr_nz_number_address();
}
