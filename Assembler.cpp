#include "Assembler.hpp"
#include "instructions.hpp"
#include <cassert>

Assembler::Assembler() {
  _nullaryInstructions[std::string("neg")] = &myNegInstruction;
  _nullaryInstructions[std::string("cpl")] = &myCplInstruction;
  _binaryInstructions[std::string("ld")] = &myLdInstruction;
}

Assembler::~Assembler() {}

void Assembler::command0(const char* mnemonic) {
  auto f = _nullaryInstructions[std::string(mnemonic)];
  assert(f);
  f(_machineCode);
}

void Assembler::command2(const char* mnemonic,
    const Argument& arg1,
    const Argument& arg2) {
  auto f = _binaryInstructions[std::string(mnemonic)];
  assert(f);
  f(arg1, arg2, _machineCode);
}

const MachineCode& Assembler::machineCode() const {
  return _machineCode;
}
