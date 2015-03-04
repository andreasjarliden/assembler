#pragma once

#include <vector>
#include <cassert>

typedef unsigned char Byte;

class MachineCode {
  typedef std::vector<Byte> Code;
public:
  typedef Code::const_iterator CodeIterator;

  void add(Byte b) {
    _code.push_back(b);
  }

  void set(Byte b, size_t offset) {
    assert(offset < _code.size());
    _code[offset] = b;
  }

  void setOrigin(int address);
  bool isEqual(const Byte* expectedBytes, int expectedSizeInBytes) const;
  CodeIterator beginCode() const;
  CodeIterator endCode() const;
  void print() const;
  void printHex() const;

  size_t size() const;

private:
  std::vector<Byte> _code;
};

