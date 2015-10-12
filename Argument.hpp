#pragma once

#include <string>

extern "C" {
#include "Argument.h"
}

class Argument {
public:
  static Argument createWithRawArgument(const RawArgument&);
  Argument();

  bool isAddress() const;
  bool isValue() const;
  bool hasValue() const;
  bool isString() const;
  bool isIdentifier() const;
  bool is8BitRegister() const;
  bool is16BitRegister() const;
  bool isHL() const;
  bool isA() const;
  bool isI() const;
  const char* identifier() const;
  int value() const;
  unsigned char ioAddress() const;
  std::string string() const;
  ArgumentType type() const; // TODO remove?
  Argument asAddressValue() const;

private:
  const char* _identifier;
  int _value;
  ArgumentType _type;
};
