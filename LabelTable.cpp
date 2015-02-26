#include "LabelTable.hpp"

void LabelTable::addLabel(const char* label, int address) {
  _labels[label] = address;
}

int LabelTable::addressForLabel(const char* label) const {
  return _labels.at(label);
}

bool LabelTable::contains(const char* label) const {
  return _labels.find(label) != _labels.end();
}
