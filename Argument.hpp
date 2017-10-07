#pragma once

#include "types.hpp"
#include <string>
#include <ostream>

extern "C" {
#include "Argument.h"
}

enum Dereferenced { NOT_DEREFERENCED, DEREFERENCED };

class Argument {
public:
  static Argument createWithRawArgument(const RawArgument&);
  Argument();

  // (nnnn) or (identifier)
  bool isDereferenced() const;
  // A value or address
  bool isValue() const;
  // A numeric value or dereferenced address
  bool hasValue() const;
  bool isString() const;
  // identifier or dereferenced identifier
  bool isIdentifier() const;
  bool isIdentifier(Dereferenced deref) const;
  // 8 bit register, possibly dereference (e.g. C or (C))
  bool is8BitRegister(Dereferenced deref = NOT_DEREFERENCED) const;
  // 16 bit register, possibly dereference (e.g. BC or (BC))
  bool is16BitRegister(Dereferenced deref = NOT_DEREFERENCED) const;
  bool isHL(Dereferenced deref = NOT_DEREFERENCED) const;
  bool isDE(Dereferenced deref = NOT_DEREFERENCED) const;
  bool isIX(Dereferenced deref = NOT_DEREFERENCED) const;
  bool isIY(Dereferenced deref = NOT_DEREFERENCED) const;
  bool isA(Dereferenced deref = NOT_DEREFERENCED) const;
  bool isI(Dereferenced deref = NOT_DEREFERENCED) const;
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
