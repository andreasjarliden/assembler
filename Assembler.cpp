#include "Assembler.hpp"
#include "instructions.hpp"
#include "LabelTable.hpp"
#include "MachineCode.hpp"
#include <map>
#include <string>
#include <functional>
#include <cassert>

namespace {

} // unnamed namespace

struct Assembler::Impl {
  typedef std::function<void (MachineCode&)> NullaryInstruction;
  typedef std::function<void (const Argument&, MachineCode&, const LabelTable&)> UnaryInstruction;

  NullaryInstruction findNullaryInstruction(const char* mnemonic) {
    auto f = nullaryInstructions[std::string(mnemonic)];
    if (!f) {
      throw std::logic_error(std::string("No nullary instruction ") + mnemonic);
    }
    return f;
  }

  UnaryInstruction findUnaryInstruction(const char* mnemonic) {
    auto f = unaryInstructions[std::string(mnemonic)];
    if (!f) {
      throw std::logic_error(std::string("No unary instruction ") + mnemonic);
    }
    return f;
  }

  // TODO add binary find version

  LabelTable labelTable;
  MachineCode machineCode;
  std::map<std::string, NullaryInstruction> nullaryInstructions;
  std::map<std::string, UnaryInstruction> unaryInstructions;
  std::map<std::string, std::function<void (const Argument&, const Argument&, MachineCode&, const LabelTable&)>> binaryInstructions;
};

Assembler::Assembler()
  : _pimpl(new Impl) {
  // TODO use macros? Could even declare as extern?
  _pimpl->nullaryInstructions[std::string("ei")] = eiInstruction;
  _pimpl->nullaryInstructions[std::string("halt")] = haltInstruction;
  _pimpl->nullaryInstructions[std::string("neg")] = negInstruction;
  _pimpl->nullaryInstructions[std::string("cpl")] = cplInstruction;
  _pimpl->binaryInstructions[std::string("ld")] = ldInstruction;
  _pimpl->unaryInstructions[std::string("out")] = outInstruction;
  _pimpl->unaryInstructions[std::string("jp")] = jpInstruction;
  _pimpl->unaryInstructions[std::string("im")] = imInstruction;
}

Assembler::~Assembler() {}

void Assembler::command0(const char* mnemonic) {
  auto f = _pimpl->findNullaryInstruction(mnemonic);
  f(_pimpl->machineCode);
}

void Assembler::command1(const char* mnemonic, const Argument& arg) {
  auto f = _pimpl->findUnaryInstruction(mnemonic);
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
