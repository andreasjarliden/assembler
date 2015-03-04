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
  _entries16.push_back(entry);
}

void DelayedAddresses::add8BitRelative(const char* identifier, int offset) {
  Entry entry;
  entry.offset = offset;
  entry.identifier = identifier;
  _entries8.push_back(entry);
}

void DelayedAddresses::resolve(MachineCode& code, const LabelTable& table) {
  resolve16(code, table);
  resolve8(code, table);
}

void DelayedAddresses::resolve8(MachineCode& code, const LabelTable& table) {
  for (const auto& entry : _entries8) {
    if (!table.contains(entry.identifier)) {
      error(std::string("Label ") + entry.identifier + " never defined");
      continue;
    }
    int address = table.addressForLabel(entry.identifier);
    int currentPC = entry.offset - 1;
    int delta = address - currentPC - 2;
    assert(delta >= -128);
    assert(delta <= 127);
    code.set(delta, entry.offset);
  }
}

void DelayedAddresses::resolve16(MachineCode& code, const LabelTable& table) {
  for (const auto& entry : _entries16) {
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
