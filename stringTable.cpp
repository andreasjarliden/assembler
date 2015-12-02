#include "stringTable.hpp"
#include <set>
#include <string>

namespace {

class SymbolTable {
public:
  const char* add(const std::string symbol) {
    return _symbols.insert(symbol).first->c_str();
  }

private:
  typedef std::set<std::string> Symbols;
  Symbols _symbols;
};

SymbolTable SYMBOL_TABLE;

std::string replaced(std::string s, const std::string& from, const std::string& to) {
  size_t startIndex = s.find(from);
  while (startIndex != std::string::npos) {
    s = s.replace(startIndex, from.length(), to);
    startIndex = s.find(from);
  }
  return s;
}

std::string replacedEscapes(std::string s) {
  s = replaced(s, "\\n", "\n");
  s = replaced(s, "\\t", "\t");
  return s;
}

} // anonymous namespace

const char* addSymbol(const char* symbol) {
  return SYMBOL_TABLE.add(symbol);
}

const char* addString(const char* s) {
  const std::string strippedQuotes(s + 1, strlen(s) - 2);
  const std::string processedString(replacedEscapes(strippedQuotes));
  return SYMBOL_TABLE.add(processedString);
}
