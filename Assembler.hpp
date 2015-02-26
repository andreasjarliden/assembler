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
  void command1(const char* mnemonic, const RawArgument&);
  void command2(const char* mnemonic, const RawArgument&, const RawArgument&);
  void label(const char* label);
  void metaCommand2(const char* command,
      const char* identifier,
      const RawArgument& argument);

  void resolveRemaining();

  const MachineCode& machineCode() const;

private:
  class Impl;
  std::unique_ptr<Impl> _pimpl;
};

