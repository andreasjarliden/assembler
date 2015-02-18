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
  std::map<std::string, const Instruction*> _nullaryInstructions;
  std::map<std::string, const Instruction*> _unaryInstructions;
  std::map<std::string, const Instruction*> _binaryInstructions;
  MachineCode _machineCode;
};

