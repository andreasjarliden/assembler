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
#include <iostream>

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
    if (eqTable.count(identifier)) {
      warning(std::string("Redefining ") + identifier);
    }
    eqTable[identifier] = argument;
  }

  void addString(const Argument& argument) {
    for (char c : argument.string()) {
      addCode(c);
    }
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
    if (arg.hasValue()) {
      address = arg.value();
    }
    else {
      if (labelTable.contains(arg.identifier())) {
        address = labelTable.addressForLabel(arg.identifier());
      }
      else {
        address = 0; // Write zero for now
        delayedAddresses.add16Bit(arg.identifier(), currentSegment().currentOffset());
      }
    }
    add16BitValue(address);
  }

  void add8BitRelativeAddress(const Argument& arg) {
    const int currentPC = currentSegment().currentOffset() - 1;
    int address;
    int delta;
    if (arg.hasValue()) {
      address = arg.value();
      delta = address - currentPC - 2;
    }
    else {
      if (labelTable.contains(arg.identifier())) {
        const int address = labelTable.addressForLabel(arg.identifier());
        delta = address - currentPC - 2;
      }
      else {
        delta = 0; // Write zero for now
        delayedAddresses.add8BitRelative(arg.identifier(), currentSegment().currentOffset());
      }
    }
    if (delta < -128)
      error("Delta <-128");
    if (delta > 127)
      error("Delta >127");
    addCode(delta);
  }

  Segment& currentSegment() {
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
  ASM_NULLARY_INSTRUCTION(cpir);
  ASM_NULLARY_INSTRUCTION(di);
  ASM_NULLARY_INSTRUCTION(ei);
  ASM_NULLARY_INSTRUCTION(halt);
  ASM_NULLARY_INSTRUCTION(neg);
  ASM_NULLARY_INSTRUCTION(cpl);
  ASM_NULLARY_INSTRUCTION(ret);
  ASM_NULLARY_INSTRUCTION(reti);
  ASM_BINARY_INSTRUCTION(ld);
  ASM_BINARY_INSTRUCTION(add);
  ASM_BINARY_INSTRUCTION(sbc);
  ASM_BINARY_INSTRUCTION(ex);
  ASM_UNARY_INSTRUCTION(push);
  ASM_UNARY_INSTRUCTION(pop);
  ASM_UNARY_INSTRUCTION(or);
  ASM_UNARY_INSTRUCTION(and);
  ASM_UNARY_INSTRUCTION(out);
  ASM_UNARY_INSTRUCTION(in);
  ASM_UNARY_INSTRUCTION(im);
  ASM_UNARY_INSTRUCTION(cp);
  ASM_UNARY_INSTRUCTION(inc);
  ASM_UNARY_INSTRUCTION(dec);
  ASM_UNARY_INSTRUCTION(call);
  ASM_UNARY_INSTRUCTION(srl);
  // Special handling of instructions which can have different arity
  extern void jpUnaryInstruction(InstructionsHost&, const Argument&);
  extern void jpBinaryInstruction(InstructionsHost&, const Argument&, const Argument&);
  extern void jrUnaryInstruction(InstructionsHost&, const Argument&);
  extern void jrBinaryInstruction(InstructionsHost&, const Argument&, const Argument&);
  _pimpl->unaryInstructions[std::string("jp")] = jpUnaryInstruction;
  _pimpl->binaryInstructions[std::string("jp")] = jpBinaryInstruction;
  _pimpl->unaryInstructions[std::string("jr")] = jrUnaryInstruction;
  _pimpl->binaryInstructions[std::string("jr")] = jrBinaryInstruction;
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
  int address = _pimpl->currentSegment().currentOffset();
  _pimpl->labelTable.addLabel(label, address);
}

void Assembler::metaCommand1(const char* command,
    const RawArgument& argument) {
  if (strcmp(command, "org") == 0) {
    size_t origin = _pimpl->resolveArgument(argument).value();
    _pimpl->segments.addSegment(origin);
  }
  else if (strcmp(command, "string") == 0) {
    _pimpl->addString(_pimpl->resolveArgument(argument));
  }
  else if (strcmp(command, "int16") == 0) {
    _pimpl->add16BitAddress(_pimpl->resolveArgument(argument));
  }
  else if (strcmp(command, "int8") == 0) {
    _pimpl->addCode(_pimpl->resolveArgument(argument).value());
  }
  else
    error(std::string("Unknown single argument .command ") + command);
}

void Assembler::metaCommand2(const char* command,
      const char* identifier,
      const RawArgument& argument) {
  if (strcmp(command, "eq") == 0) {
    _pimpl->addEq(identifier, _pimpl->resolveArgument(argument));
  }
  else
    error(std::string("Unknown double argument .command ") + command);
}

const Segments& Assembler::segments() const {
  return _pimpl->segments;
}

void Assembler::resolveRemaining() {
  _pimpl->delayedAddresses.resolve(_pimpl->currentSegment(), _pimpl->labelTable);
  _pimpl->labelTable.print();
}

