#include "Assembler.hpp"
#include "LabelTable.hpp"
#include "MachineCode.hpp"
#include "Argument.hpp"
#include "errorChecking.hpp"
#include "InstructionsHost.hpp"
#include "DelayedAddresses.hpp"
#include <map>
#include <string>
#include <functional>
#include <cassert>

class Argument;

namespace {

} // unnamed namespace

struct Assembler::Impl : public InstructionsHost {
  typedef std::function<void (InstructionsHost&)> NullaryInstruction;
  typedef std::function<void (InstructionsHost&, const Argument&)> UnaryInstruction;
  typedef std::function<void (InstructionsHost&, const Argument&, const Argument&)> BinaryInstruction;

  NullaryInstruction findNullaryInstruction(const char* mnemonic) {
    auto f = nullaryInstructions[std::string(mnemonic)];
    if (!f) {
      error(std::string("No nullary instruction ") + mnemonic);
    }
    return f;
  }

  UnaryInstruction findUnaryInstruction(const char* mnemonic) {
    auto f = unaryInstructions[std::string(mnemonic)];
    if (!f) {
      error(std::string("No unary instruction ") + mnemonic);
    }
    return f;
  }

  BinaryInstruction findBinaryInstruction(const char* mnemonic) {
    auto f = binaryInstructions[std::string(mnemonic)];
    if (!f) {
      error(std::string("No binary instruction ") + mnemonic);
    }
    return f;
  }

  Argument resolveArgument(const Argument& argument) {
    if (argument.type == IDENTIFIER_ARGUMENT) {
      auto t = eqTable.find(std::string(argument.identifier));
      if (t != eqTable.end()) {
        return t->second;
      }
    }
    else if (argument.type == ADDRESS_IDENTIFIER_ARGUMENT) {
      auto t = eqTable.find(std::string(argument.identifier));
      if (t != eqTable.end()) {
        Argument resolved = t->second;
        resolved.type = ADDRESS_VALUE_ARGUMENT;
        return resolved;
      }
    }
    return argument;
  }

  void addEq(const char* identifier, const Argument& argument) {
    eqTable[identifier] = argument;
  }

  void addCode(Byte byte) {
    machineCode.add(byte);
  }

  int addressForLabel(const char* label) {
    return labelTable.addressForLabel(label);
  }

  bool containsLabel(const char* label) {
    return labelTable.contains(label);
  }

  void addDelayed16BitValue(const char* identifier) {
    return delayedAddresses.add16Bit(identifier, machineCode.size());
  }

  LabelTable labelTable;
  MachineCode machineCode;
  DelayedAddresses delayedAddresses;
  std::map<std::string, NullaryInstruction> nullaryInstructions;
  std::map<std::string, UnaryInstruction> unaryInstructions;
  std::map<std::string, BinaryInstruction> binaryInstructions;
  std::map<std::string, Argument> eqTable;
};

#define ASM_NULLARY_INSTRUCTION(X) extern void X ## Instruction(InstructionsHost&); \
  _pimpl->nullaryInstructions[std::string(#X)] = X ## Instruction;

#define ASM_UNARY_INSTRUCTION(X) extern void X ## Instruction(InstructionsHost&, const Argument&); \
  _pimpl->unaryInstructions[std::string(#X)] = X ## Instruction;

#define ASM_BINARY_INSTRUCTION(X) extern void X ## Instruction(InstructionsHost&, const Argument&, const Argument&); \
  _pimpl->binaryInstructions[std::string(#X)] = X ## Instruction;


Assembler::Assembler()
  : _pimpl(new Impl) {
  ASM_NULLARY_INSTRUCTION(nop);
  ASM_NULLARY_INSTRUCTION(ei);
  ASM_NULLARY_INSTRUCTION(halt);
  ASM_NULLARY_INSTRUCTION(neg);
  ASM_NULLARY_INSTRUCTION(cpl);
  ASM_NULLARY_INSTRUCTION(reti);
  ASM_BINARY_INSTRUCTION(ld);
  ASM_UNARY_INSTRUCTION(out);
  ASM_UNARY_INSTRUCTION(in);
  ASM_UNARY_INSTRUCTION(jp);
  ASM_UNARY_INSTRUCTION(im);
}

Assembler::~Assembler() {}

void Assembler::command0(const char* mnemonic) {
  auto f = _pimpl->findNullaryInstruction(mnemonic);
  f(*_pimpl);
}

void Assembler::command1(const char* mnemonic, const Argument& arg) {
  auto f = _pimpl->findUnaryInstruction(mnemonic);
  Argument resolvedArg = _pimpl->resolveArgument(arg);
  f(*_pimpl, resolvedArg);
}

void Assembler::command2(const char* mnemonic,
    const Argument& arg1,
    const Argument& arg2) {
  auto f = _pimpl->findBinaryInstruction(mnemonic);
  Argument resolvedArg1 = _pimpl->resolveArgument(arg1);
  Argument resolvedArg2 = _pimpl->resolveArgument(arg2);
  f(*_pimpl, resolvedArg1, resolvedArg2);
}

void Assembler::label(const char* label) {
  int address = machineCode().size();
  _pimpl->labelTable.addLabel(label, address);
}

void Assembler::metaCommand2(const char* command,
      const char* identifier,
      const Argument& argument) {
  if (strcmp(command, "eq") == 0) {
    _pimpl->addEq(identifier, argument);
  }
  else
    assert(false);
}

const MachineCode& Assembler::machineCode() const {
  return _pimpl->machineCode;
}

void Assembler::resolveRemaining() {
  _pimpl->delayedAddresses.resolve(_pimpl->machineCode, _pimpl->labelTable);
}

