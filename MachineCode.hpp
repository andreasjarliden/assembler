#pragma once

#include <vector>

typedef unsigned char Byte;

class MachineCode {
public:
  void add(Byte b) {
    _code.push_back(b);
  }

  bool isEqual(const Byte* expectedBytes, int expectedSizeInBytes) const;
  void print() const;
  void printHex() const;

  size_t size() const;

private:
  std::vector<Byte> _code;
};

