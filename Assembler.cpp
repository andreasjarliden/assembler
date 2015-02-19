#include "Assembler.hpp"
#include "instructions.hpp"
#include "LabelTable.hpp"
#include "MachineCode.hpp"
#include <map>
#include <string>
#include <functional>
#include <cassert>

struct Assembler::Impl {
  LabelTable labelTable;
  MachineCode machineCode;
  std::map<std::string, std::function<void (MachineCode&)>> nullaryInstructions;
  std::map<std::string, std::function<void (const Argument&, MachineCode&, const LabelTable&)>> unaryInstructions;
  std::map<std::string, std::function<void (const Argument&, const Argument&, MachineCode&, const LabelTable&)>> binaryInstructions;
};

Assembler::Assembler()
  : _pimpl(new Impl) {
  _pimpl->nullaryInstructions[std::string("neg")] = negInstruction;
  _pimpl->nullaryInstructions[std::string("cpl")] = cplInstruction;
  _pimpl->binaryInstructions[std::string("ld")] = ldInstruction;
  _pimpl->unaryInstructions[std::string("out")] = outInstruction;
  _pimpl->unaryInstructions[std::string("jp")] = jpInstruction; }

Assembler::~Assembler() {}

void Assembler::command0(const char* mnemonic) {
  auto f = _pimpl->nullaryInstructions[std::string(mnemonic)];
  assert(f);
  f(_pimpl->machineCode);
}

void Assembler::command1(const char* mnemonic, const Argument& arg) {
  auto f = _pimpl->unaryInstructions[std::string(mnemonic)];
  assert(f);
  f(arg, _pimpl->machineCode, _pimpl->labelTable);
}

void Assembler::command2(const char* mnemonic,
    const Argument& arg1,
    const Argument& arg2) {
  auto f = _pimpl->binaryInstructions[std::string(mnemonic)];
  assert(f);
  f(arg1, arg2, _pimpl->machineCode, _pimpl->labelTable);
}

void Assembler::label(const char* label) {
  int address = machineCode().size();
  _pimpl->labelTable.addLabel(label, address);
}

const MachineCode& Assembler::machineCode() const {
  return _pimpl->machineCode;
}
