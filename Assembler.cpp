#include "Assembler.hpp"
#include "instructions.hpp"
#include <cassert>
#include <iostream>

Assembler::Assembler() {
  _nullaryInstructions[std::string("neg")] = &myNegInstruction;
  _nullaryInstructions[std::string("cpl")] = &myCplInstruction;
  _binaryInstructions[std::string("ld")] = &myLdInstruction;
  _unaryInstructions[std::string("out")] = &outInstruction;
  _unaryInstructions[std::string("jp")] = &jpInstruction;
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
  f(arg, _machineCode);
}

void Assembler::command2(const char* mnemonic,
    const Argument& arg1,
    const Argument& arg2) {
  auto f = _binaryInstructions[std::string(mnemonic)];
  assert(f);
  f(arg1, arg2, _machineCode);
}

void Assembler::label(const char* label) {
  size_t offset = machineCode().size();
  _labels[label] = offset;
  std::cout << "Storing label " << label << " to " << offset << std::endl;
}

const MachineCode& Assembler::machineCode() const {
  return _machineCode;
}
