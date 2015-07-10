#pragma once

#include "MachineCode.hpp"

class Segment : public MachineCode {
public:
  Segment(size_t offset)
    : _offset(offset) {}

  size_t offset() const {
    return _offset;
  }

private:
  size_t _offset;
};
