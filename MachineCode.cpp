#include "MachineCode.hpp"
#include "Error.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

void MachineCode::setOrigin(int address) {
  if (address < size())
    throw Error(std::string("Setting origin would cause memory to be overwritten."));
  int padding = address - size();
  _code.insert(_code.end(), padding, 0);
}

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

// TODO: shouldn't be part of MachineCode
void MachineCode::printHex() const {
  for (int i = 0; i < _code.size(); ) {
    std::cout << std::hex << std::setw(4) << std::setfill('0') << i << ": ";
    for (int j = 0; j < 8 && i < _code.size(); ++j, ++i) {
      std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)_code[i] << " ";
    }
    std::cout << std::endl;
  }
}


size_t MachineCode::size() const {
  return _code.size();
}
