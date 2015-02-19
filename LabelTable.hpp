#pragma once

#include <map>

class LabelTable {
public:
  void addLabel(const char* label, int address);
  int addressForLabel(const char* label) const;

private:
  std::map<const char*, int> _labels;
};
