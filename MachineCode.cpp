#include "MachineCode.hpp"
#include "Error.hpp"
#include <algorithm>

bool MachineCode::isEqual(const Byte* expectedBytes, int expectedSizeInBytes) const {
  if (_code.size() != expectedSizeInBytes)
    return false;
  return std::equal(_code.begin(), _code.end(), expectedBytes);
}

MachineCode::CodeIterator MachineCode::beginCode() const {
  return _code.begin();
}

MachineCode::CodeIterator MachineCode::endCode() const {
  return _code.end();
}

size_t MachineCode::size() const {
  return _code.size();
}
