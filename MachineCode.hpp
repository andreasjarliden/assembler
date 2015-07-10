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

  Byte get(size_t offset) const {
    return _code[offset];
  }

  void setOrigin(int address);
  bool isEqual(const Byte* expectedBytes, int expectedSizeInBytes) const;
  CodeIterator beginCode() const;
  CodeIterator endCode() const;
  size_t size() const;

private:
  std::vector<Byte> _code;
};

