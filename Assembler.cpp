#include "Assembler.hpp"
#include "instructions.hpp"
#include <cassert>
#include <iostream>

Assembler::Assembler() {
  _nullaryInstructions[std::string("neg")] = negInstruction;
  _nullaryInstructions[std::string("cpl")] = cplInstruction;
  _binaryInstructions[std::string("ld")] = ldInstruction;
  _unaryInstructions[std::string("out")] = outInstruction;
  _unaryInstructions[std::string("jp")] = jpInstruction;
}

Assembler::~Assembler() {}

void Assembler::command0(const char* mnemonic) {
  auto f = _nullaryInstructions[std::string(mnemonic)];
  assert(f);
  f(_machineCode);
}

void Assembler::command1(const char* mnemonic, const Argument& arg) {
  auto f = _unaryInstructions[std::string(mnemonic)];
  assert(f);
  f(arg, _machineCode, _labelTable);
}

void Assembler::command2(const char* mnemonic,
    const Argument& arg1,
    const Argument& arg2) {
  auto f = _binaryInstructions[std::string(mnemonic)];
  assert(f);
  f(arg1, arg2, _machineCode, _labelTable);
}

void Assembler::label(const char* label) {
  int address = machineCode().size();
  _labelTable.addLabel(label, address);
}

const MachineCode& Assembler::machineCode() const {
  return _machineCode;
}
