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
  std::map<std::string, std::function<void (MachineCode&)>> _nullaryInstructions;
  std::map<std::string, std::function<void (const Argument&, MachineCode&)>> _unaryInstructions;
  std::map<std::string, std::function<void (const Argument&, const Argument&, MachineCode&)>> _binaryInstructions;
  MachineCode _machineCode;
};

