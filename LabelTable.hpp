#pragma once

#include <map>
#include <iosfwd>

class LabelTable {
public:
  void addLabel(const char* label, int address);
  int addressForLabel(const char* label) const;
  bool contains(const char* label) const;
  void print(std::ostream& stream) const;

private:
  std::map<const char*, int> _labels;
};
