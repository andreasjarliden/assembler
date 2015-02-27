#include "DelayedAddresses.hpp"

#include "MachineCode.hpp"
#include "LabelTable.hpp"
#include "Error.hpp"
#include "errorChecking.hpp"
#include <vector>

void DelayedAddresses::add16Bit(const char* identifier, int offset) {
  Entry entry;
  entry.offset = offset;
  entry.identifier = identifier;
  _entries.push_back(entry);
}

void DelayedAddresses::resolve(MachineCode& code, const LabelTable& table) {
  for (const auto& entry : _entries) {
    if (!table.contains(entry.identifier)) {
      error(std::string("Label ") + entry.identifier + " never defined");
      continue;
    }
    int address = table.addressForLabel(entry.identifier);
    Byte low = (Byte)address & 0xff;
    Byte high = (Byte)((address >> 8) & 0xff);
    code.set(low, entry.offset);
    code.set(high, entry.offset + 1);
  }
}
