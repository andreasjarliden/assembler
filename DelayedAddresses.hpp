#pragma once

#include <memory>

class LabelTable;
class MachineCode;

class DelayedAddresses {
public:
  DelayedAddresses();
  ~DelayedAddresses();

  void add16Bit(const char* identifier, int offset);
  void add8BitRelative(const char* identifier, int offset);
  void resolve(MachineCode& code, const LabelTable& table);

private:
  class Impl;
  std::unique_ptr<Impl> _pimpl;
};

