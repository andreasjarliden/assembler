#include "DelayedAddresses.hpp"
#include "Segment.hpp"
#include "LabelTable.hpp"
#include "Error.hpp"
#include "errorChecking.hpp"
#include <vector>
#include <set>
#include <iostream>

struct DelayedAddresses::Impl {
  Impl() {
    // TODO add to the symbol table so we can check based on the pointer
    reserved.insert(std::string("a"));
    reserved.insert(std::string("b"));
    reserved.insert(std::string("c"));
    reserved.insert(std::string("d"));
    reserved.insert(std::string("e"));
    reserved.insert(std::string("h"));
    reserved.insert(std::string("l"));
    reserved.insert(std::string("af"));
    reserved.insert(std::string("be"));
    reserved.insert(std::string("de"));
    reserved.insert(std::string("ix"));
    reserved.insert(std::string("iy"));
    reserved.insert(std::string("hl"));
    reserved.insert(std::string("sp"));
  }

  void resolve16(Segment& code, const LabelTable& table) {
    for (const auto& entry : entries16) {
      if (!table.contains(entry.identifier)) {
        error(std::string("Label ") + entry.identifier + " never defined");
        continue;
      }
      int address = table.addressForLabel(entry.identifier);
      Byte low = (Byte)address & 0xff;
      Byte high = (Byte)((address >> 8) & 0xff);
      code.set(low, entry.address);
      code.set(high, entry.address + 1);
    }
  }

  void resolve8(Segment& code, const LabelTable& table) {
    for (const auto& entry : entries8) {
      if (!table.contains(entry.identifier)) {
        error(std::string("Label ") + entry.identifier + " never defined");
        continue;
      }
      int address = table.addressForLabel(entry.identifier);
      int currentPC = entry.address - 1;
      int delta = address - currentPC - 2;
      assert(delta >= -128);
      assert(delta <= 127);
      code.set(delta, entry.address);
    }
  }

  void verifyNotReserved(const char* identifier) {
    if (reserved.find(std::string(identifier)) != reserved.end()) {
      error(std::string("Identifier ") + identifier + " is reserved");
    }
  }

  struct Entry {
    int address;
    const char* identifier;
  };

  std::vector<Entry> entries16;
  std::vector<Entry> entries8;
  std::set<std::string> reserved;
};

DelayedAddresses::DelayedAddresses()
  : _pimpl(new Impl) {}

DelayedAddresses::~DelayedAddresses() {}

void DelayedAddresses::add16Bit(const char* identifier, int address) {
  _pimpl->verifyNotReserved(identifier);
  Impl::Entry entry;
  entry.address = address;
  entry.identifier = identifier;
  _pimpl->entries16.push_back(entry);
}

void DelayedAddresses::add8BitRelative(const char* identifier, int address) {
  _pimpl->verifyNotReserved(identifier);
  Impl::Entry entry;
  entry.address = address;
  entry.identifier = identifier;
  _pimpl->entries8.push_back(entry);
}

void DelayedAddresses::resolve(Segment& code, const LabelTable& table) {
  _pimpl->resolve16(code, table);
  _pimpl->resolve8(code, table);
}
