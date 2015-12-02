#include "DelayedAddresses.hpp"
#include "Segment.hpp"
#include "LabelTable.hpp"
#include "Error.hpp"
#include "errorChecking.hpp"
#include <vector>

struct DelayedAddresses::Impl {
  void resolve16(Segment& code, const LabelTable& table) {
    for (const auto& entry : entries16) {
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

  void resolve8(Segment& code, const LabelTable& table) {
    for (const auto& entry : entries8) {
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

  struct Entry {
    int offset;
    const char* identifier;
  };

  std::vector<Entry> entries16;
  std::vector<Entry> entries8;
};

DelayedAddresses::DelayedAddresses()
  : _pimpl(new Impl) {}

DelayedAddresses::~DelayedAddresses() {}

void DelayedAddresses::add16Bit(const char* identifier, int offset) {
  Impl::Entry entry;
  entry.offset = offset;
  entry.identifier = identifier;
  _pimpl->entries16.push_back(entry);
}

void DelayedAddresses::add8BitRelative(const char* identifier, int offset) {
  Impl::Entry entry;
  entry.offset = offset;
  entry.identifier = identifier;
  _pimpl->entries8.push_back(entry);
}

void DelayedAddresses::resolve(Segment& code, const LabelTable& table) {
  _pimpl->resolve16(code, table);
  _pimpl->resolve8(code, table);
}
