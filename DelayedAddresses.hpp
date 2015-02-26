#pragma once

#include <vector>

class LabelTable;
class MachineCode;

class DelayedAddresses {
public:
  void add16Bit(const char* identifier, int offset);
  void resolve(MachineCode& code, const LabelTable& table);

private:
  struct Entry {
    int offset;
    const char* identifier;
  };
  std::vector<Entry> _entries;
};

