#include "Assembler.hpp"
#include "LabelTable.hpp"
#include "Segment.hpp"
#include "Argument.hpp"
#include "errorChecking.hpp"
#include "InstructionsHost.hpp"
#include "DelayedAddresses.hpp"
#include "Error.hpp"
#include "Segments.hpp"
#include <map>
#include <string>
#include <functional>
#include <cassert>

struct Assembler::Impl : public InstructionsHost {
  typedef std::function<void (InstructionsHost&)> NullaryInstruction;
  typedef std::function<void (InstructionsHost&, const Argument&)> UnaryInstruction;
  typedef std::function<void (InstructionsHost&, const Argument&, const Argument&)> BinaryInstruction;

  NullaryInstruction findNullaryInstruction(const char* mnemonic) {
    auto f = nullaryInstructions[std::string(mnemonic)];
    if (!f) {
      throw Error(std::string("No nullary instruction ") + mnemonic);
    }
    return f;
  }

  UnaryInstruction findUnaryInstruction(const char* mnemonic) {
    auto f = unaryInstructions[std::string(mnemonic)];
    if (!f) {
      throw Error(std::string("No unary instruction ") + mnemonic);
    }
    return f;
  }

  BinaryInstruction findBinaryInstruction(const char* mnemonic) {
    auto f = binaryInstructions[std::string(mnemonic)];
    if (!f) {
      throw Error(std::string("No binary instruction ") + mnemonic);
    }
    return f;
  }

  Argument resolveArgument(const RawArgument& argument) {
    if (argument.type == IDENTIFIER_ARGUMENT) {
      auto t = eqTable.find(std::string(argument.identifier));
      if (t != eqTable.end()) {
        return t->second;
      }
    }
    else if (argument.type == ADDRESS_IDENTIFIER_ARGUMENT) {
      auto t = eqTable.find(std::string(argument.identifier));
      if (t != eqTable.end()) {
        return t->second.asAddressValue();
      }
    }
    return Argument::createWithRawArgument(argument);
  }

  void addEq(const char* identifier, const Argument& argument) {
    eqTable[identifier] = argument;
  }

  void addCode(Byte byte) {
    currentSegment().add(byte);
  }

  void add16BitValue(int value) {
    assert(value >= 0);
    assert(value <= 0xffff);
    Byte low = (Byte)value & 0xff;
    Byte high = (Byte)((value >> 8) & 0xff);
    addCode(low);
    addCode(high);
  }

  void add16BitAddress(const Argument& arg) {
    int address;
    if (arg.isValue()) {
      address = arg.value();
    }
    else {
      if (labelTable.contains(arg.identifier())) {
        address = labelTable.addressForLabel(arg.identifier());
      }
      else {
        address = 0; // Write zero for now
        delayedAddresses.add16Bit(arg.identifier(), currentSegment().size());
      }
    }
    add16BitValue(address);
  }

  void add8BitRelativeAddress(const Argument& arg) {
    int address;
    if (arg.isValue()) {
      address = arg.value();
    }
    else {
      if (labelTable.contains(arg.identifier())) {
        address = labelTable.addressForLabel(arg.identifier());
      }
      else {
        address = 0; // Write zero for now
        delayedAddresses.add8BitRelative(arg.identifier(), currentSegment().size());
      }
    }
    int currentPC = currentSegment().size() - 1;
    int delta = address - currentPC - 2;
    assert(delta >= -128);
    assert(delta <= 127);
    addCode(delta);
  }

  MachineCode& currentSegment() {
    return segments.index(segments.numberOfSegments()-1);
  }

  LabelTable labelTable;
  Segments segments;
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
  ASM_NULLARY_INSTRUCTION(ret);
  ASM_NULLARY_INSTRUCTION(reti);
  ASM_BINARY_INSTRUCTION(ld);
  ASM_BINARY_INSTRUCTION(add);
  ASM_UNARY_INSTRUCTION(out);
  ASM_UNARY_INSTRUCTION(in);
  ASM_UNARY_INSTRUCTION(im);
  ASM_UNARY_INSTRUCTION(cp);
  ASM_UNARY_INSTRUCTION(inc);
  ASM_UNARY_INSTRUCTION(dec);
  ASM_UNARY_INSTRUCTION(jr);
  ASM_UNARY_INSTRUCTION(call);
  // Special handling of instructions which can have different arity
  extern void jpUnaryInstruction(InstructionsHost&, const Argument&);
  extern void jpBinaryInstruction(InstructionsHost&, const Argument&, const Argument&);
  _pimpl->unaryInstructions[std::string("jp")] = jpUnaryInstruction;
  _pimpl->binaryInstructions[std::string("jp")] = jpBinaryInstruction;
}

Assembler::~Assembler() {}

void Assembler::command0(const char* mnemonic) {
  try {
    auto f = _pimpl->findNullaryInstruction(mnemonic);
    f(*_pimpl);
  }
  catch (const Error& e) {
    error(e.message());
  }
}

void Assembler::command1(const char* mnemonic, const RawArgument& arg) {
  try {
    auto f = _pimpl->findUnaryInstruction(mnemonic);
    Argument resolvedArg = _pimpl->resolveArgument(arg);
    f(*_pimpl, resolvedArg);
  }
  catch (const Error& e) {
    error(e.message());
  }
}

void Assembler::command2(const char* mnemonic,
    const RawArgument& arg1,
    const RawArgument& arg2) {
  try {
    auto f = _pimpl->findBinaryInstruction(mnemonic);
    Argument resolvedArg1 = _pimpl->resolveArgument(arg1);
    Argument resolvedArg2 = _pimpl->resolveArgument(arg2);
    f(*_pimpl, resolvedArg1, resolvedArg2);
  }
  catch (const Error& e) {
    error(e.message());
  }
}

void Assembler::label(const char* label) {
  int address = _pimpl->currentSegment().size();
  _pimpl->labelTable.addLabel(label, address);
}

void Assembler::metaCommand1(const char* command,
    const RawArgument& argument) {
  if (strcmp(command, "org") == 0) {
    size_t origin = _pimpl->resolveArgument(argument).value();
    _pimpl->segments.addSegment(origin);
  }
  else
    throw Error(std::string("Unknown single argument .command ") + command);
}

void Assembler::metaCommand2(const char* command,
      const char* identifier,
      const RawArgument& argument) {
  if (strcmp(command, "eq") == 0) {
    _pimpl->addEq(identifier, _pimpl->resolveArgument(argument));
  }
  else
    throw Error(std::string("Unknown double argument .command ") + command);
}

const Segments& Assembler::segments() const {
  return _pimpl->segments;
}

void Assembler::resolveRemaining() {
  _pimpl->delayedAddresses.resolve(_pimpl->currentSegment(), _pimpl->labelTable);
}

