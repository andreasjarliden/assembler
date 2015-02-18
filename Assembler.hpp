#pragma once

#include "Commands.hpp"
#include "MachineCode.hpp"
#include <map>
#include <string>

class Instruction;

class Assembler : public Commands {
public:
  Assembler();

  ~Assembler();

  void command0(const char* mnemonic);

  virtual void command2(const char* mnemonic,
      const Argument& arg1,
      const Argument& arg2);

  const MachineCode& machineCode() const;

private:
  void addInstruction(const char* mnemonic, const Instruction& instruction);

  std::map<std::string, const Instruction*> _mnemonics;
  MachineCode _machineCode;
};

