#include "testLoadGroup.hpp"
#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "argumentHelpers.hpp"
#include "testUtilities.hpp"

namespace {

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

void test_ld_hl_word_following_label() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("HL"), identifierArg("aLabel"));
  assembler.label("aLabel");
  assembler.resolveRemaining();
  Byte expectedBytes[] = { 0x21, 0x03, 0x00 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void test_ld_addr_a() {
  Assembler assembler;
  assembler.command2("ld", addressArg(0x1234), identifierArg("a"));
  Byte expectedBytes[] = { 0x32, 0x34, 0x12 };
  isEqualToBytes(assembler, expectedBytes, 3);
}

void test_ld_addr_hl() {
  Assembler assembler;
  assembler.command2("ld", addressArg(0x1234), identifierArg("hl"));
  Byte expectedBytes[] = { 0x22, 0x34, 0x12 };
  isEqualToBytes(assembler, expectedBytes, 3);
}


void test_ld_a_using_eq() {
  Assembler assembler;
  assembler.metaCommand2("eq", "FOUR_TWO", numberArg(42));
  assembler.command2("ld", identifierArg("a"), identifierArg("FOUR_TWO"));
  Byte expectedBytes[] = { 0x3e, 42 };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_ld_c_d() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("c"), identifierArg("d"));
  Byte expectedBytes[] = { 0b01001010 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

void test_ld_i_a() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("i"), identifierArg("a"));
  Byte expectedBytes[] = { 0xed, 0x47 };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_ld_a_i() {
  Assembler assembler;
  assembler.command2("ld", identifierArg("a"), identifierArg("i"));
  Byte expectedBytes[] = { 0xed, 0x57 };
  isEqualToBytes(assembler, expectedBytes, 2);
}

void test_ld_adr_hl_a() {
  Assembler assembler;
  assembler.command2("ld", addressIdentifierArg("hl"), identifierArg("a"));
  Byte expectedBytes[] = { 0b01110111 };
  isEqualToBytes(assembler, expectedBytes, 1);
}

}

void testLoadGroup() {
  test_ld_a_byte();
  test_ld_d_byte();
  test_ld_hl_word();
  test_ld_hl_word_following_label();
  test_ld_addr_a();
  test_ld_addr_hl();
  test_ld_c_d();
  test_ld_i_a();
  test_ld_a_i();
  test_ld_adr_hl_a();
  test_ld_a_using_eq();
}
