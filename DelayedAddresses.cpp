#include "DelayedAddresses.hpp"

#include "MachineCode.hpp"
#include "LabelTable.hpp"
#include <vector>

void DelayedAddresses::add16Bit(const char* identifier, int offset) {
  Entry entry;
  entry.offset = offset;
  entry.identifier = identifier;
  _entries.push_back(entry);
}

void DelayedAddresses::resolve(MachineCode& code, const LabelTable& table) {
  for (const auto& entry : _entries) {
    int address = table.addressForLabel(entry.identifier);
    // TODO error if still not resolved
    Byte low = (Byte)address & 0xff;
    Byte high = (Byte)((address >> 8) & 0xff);
    code.set(low, entry.offset);
    code.set(high, entry.offset + 1);
  }
}
