#include "LabelTable.hpp"
#include <ostream>

void LabelTable::addLabel(const char* label, int address) {
  _labels[label] = address;
}

int LabelTable::addressForLabel(const char* label) const {
  return _labels.at(label);
}

bool LabelTable::contains(const char* label) const {
  return _labels.find(label) != _labels.end();
}

void LabelTable::print(std::ostream& stream) const {
  for (const auto& entry : _labels) {
    stream << entry.first << " " << std::hex << entry.second << std::endl;
  }
}
