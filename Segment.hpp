#pragma once

#include "MachineCode.hpp"

class Segment : public MachineCode {
public:
  Segment(size_t base)
    : _base(base) {}

  size_t base() const {
    return _base;
  }

private:
  size_t _base;
};
