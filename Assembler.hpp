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
  void command1(const char* mnemonic, const Argument&);
  void command2(const char* mnemonic, const Argument&, const Argument&);
  void label(const char* label);

  const MachineCode& machineCode() const;

private:
  std::map<std::string, std::function<void (MachineCode&)>> _nullaryInstructions;
  std::map<std::string, std::function<void (const Argument&, MachineCode&)>> _unaryInstructions;
  std::map<std::string, std::function<void (const Argument&, const Argument&, MachineCode&)>> _binaryInstructions;
  std::map<const char*, size_t> _labels;
  MachineCode _machineCode;
};

