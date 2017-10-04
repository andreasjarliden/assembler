#pragma once

#include "types.hpp"
#include <string>
#include <ostream>

extern "C" {
#include "Argument.h"
}

class Argument {
public:
  static Argument createWithRawArgument(const RawArgument&);
  Argument();

  // (nnnn) or (identifier)
  bool isDereferenced() const;
  bool isValue() const;
  bool hasValue() const;
  bool isString() const;
  bool isIdentifier() const;
  // 8 bit register, possibly dereference (e.g. C or (C))
  bool is8BitRegister() const;
  // 16 bit register, possibly dereference (e.g. BC or (BC))
  bool is16BitRegister() const;
  bool isHL() const;
  bool isDE() const;
  bool isIX() const;
  bool isIY() const;
  bool isA() const;
  bool isI() const;
  bool isFlag(const char* flag) const;
  const char* identifier() const;
  int value() const;
  Byte byteValue() const;
  unsigned char ioAddress() const;
  std::string string() const;
  ArgumentType type() const; // TODO remove?
  Argument asAddressValue() const;

private:
  const char* _identifier;
  int _value;
  ArgumentType _type;
};

std::ostream& operator<<(std::ostream& s, const Argument& arg);
