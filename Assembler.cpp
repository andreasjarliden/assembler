#include "Assembler.hpp"
#include "Instruction.hpp"

Assembler::Assembler() {
  static NegInstruction negInstruction;
  static CplInstruction cplInstruction;
  static LdInstruction ldInstruction;

  addInstruction("neg", negInstruction);
  addInstruction("cpl", cplInstruction);
  addInstruction("ld", ldInstruction);
}

Assembler::~Assembler() {}

void Assembler::command0(const char* mnemonic) {
  const Instruction* i = _mnemonics[std::string(mnemonic)];
  assert(i);
  i->translate(_machineCode);
}

void Assembler::command2(const char* mnemonic,
    const Argument& arg1,
    const Argument& arg2) {
  const Instruction* i = _mnemonics[std::string(mnemonic)];
  assert(i);
  i->translate(_machineCode, arg1, arg2);
}

const MachineCode& Assembler::machineCode() const {
  return _machineCode;
}

void Assembler::addInstruction(const char* mnemonic, const Instruction& instruction) {
  _mnemonics[std::string(mnemonic)] = &instruction;
}
