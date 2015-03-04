#pragma once

#include <vector>

class LabelTable;
class MachineCode;

class DelayedAddresses {
public:
  void add16Bit(const char* identifier, int offset);
  void add8BitRelative(const char* identifier, int offset);
  void resolve(MachineCode& code, const LabelTable& table);

private:
  void resolve16(MachineCode& code, const LabelTable& table);
  void resolve8(MachineCode& code, const LabelTable& table);

  struct Entry {
    int offset;
    const char* identifier;
  };
  std::vector<Entry> _entries16;
  std::vector<Entry> _entries8;
};

