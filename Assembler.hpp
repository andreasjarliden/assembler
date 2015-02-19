#pragma once

#include "Commands.hpp"
#include "MachineCode.hpp"
#include "LabelTable.hpp"
#include <string>

class LabelTable;

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
  std::map<std::string, std::function<void (const Argument&, MachineCode&, const LabelTable&)>> _unaryInstructions;
  std::map<std::string, std::function<void (const Argument&, const Argument&, MachineCode&, const LabelTable&)>> _binaryInstructions;
  LabelTable _labelTable;
  MachineCode _machineCode;
};

