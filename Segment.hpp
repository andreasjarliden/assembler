#pragma once

#include "MachineCode.hpp"

class Segment : public MachineCode {
public:
  Segment(size_t base)
    : _base(base) {}

  size_t base() const {
    return _base;
  }

  size_t currentOffset() const {
    return base() + size();
  }

  void set(Byte b, size_t offset) {
    assert(offset < size() + base());
    assert(offset >= base());
    _code[offset - base()] = b;
  }

  Byte get(size_t offset) const {
    assert(offset < size() + base());
    assert(offset >= base());
    return _code[offset - base()];
  }

private:
  size_t _base;
};
