#include "MachineCode.hpp"
#include <algorithm>
#include <iostream>

bool MachineCode::isEqual(const Byte* expectedBytes, int expectedSizeInBytes) const {
  if (_code.size() != expectedSizeInBytes)
    return false;
  return std::equal(_code.begin(), _code.end(), expectedBytes);
}

void MachineCode::print() const {
  for (int i = 0; i < _code.size(); ++i) {
    std::cout << i << ": " << std::hex << (int)_code[i] << std::endl;
  }
}
