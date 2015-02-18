#include "Assembler.hpp"
#include "Instruction.hpp"

Assembler::Assembler() {
  static NegInstruction negInstruction;
  static CplInstruction cplInstruction;
  static LdInstruction ldInstruction;

  _nullaryInstructions[std::string("neg")] = &negInstruction;
  _nullaryInstructions[std::string("cpl")] = &cplInstruction;
  _binaryInstructions[std::string("ld")] = &ldInstruction;
}

Assembler::~Assembler() {}

void Assembler::command0(const char* mnemonic) {
  const Instruction* i = _nullaryInstructions[std::string(mnemonic)];
  assert(i);
  i->translate(_machineCode);
}

void Assembler::command2(const char* mnemonic,
    const Argument& arg1,
    const Argument& arg2) {
  const Instruction* i = _binaryInstructions[std::string(mnemonic)];
  assert(i);
  i->translate(_machineCode, arg1, arg2);
}

const MachineCode& Assembler::machineCode() const {
  return _machineCode;
}
