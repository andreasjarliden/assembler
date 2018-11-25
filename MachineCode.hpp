#pragma once

#include "types.hpp"
#include <vector>
#include <cassert>

class MachineCode {
  typedef std::vector<Byte> Code;
public:
  typedef Code::const_iterator CodeIterator;

  void add(Byte b) {
    _code.push_back(b);
  }

  bool isEqual(const Byte* expectedBytes, int expectedSizeInBytes) const;
  CodeIterator beginCode() const;
  CodeIterator endCode() const;
  std::size_t size() const;

protected:
  std::vector<Byte> _code;
};

