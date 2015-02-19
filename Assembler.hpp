#pragma once

#include "Commands.hpp"
#include <memory>

class MachineCode;
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
  class Impl;
  std::unique_ptr<Impl> _pimpl;
};

