#pragma once

extern "C" {
#include "Argument.h"
}

class Argument {
public:
  static Argument createWithRawArgument(const RawArgument&);
  Argument();

  bool isAddress() const;
  bool is8BitRegister() const;
  bool is16BitRegister() const;
  const char* identifier() const;
  int value() const;
  ArgumentType type() const;
  Argument asAddressValue() const;

private:
  const char* _identifier;
  int _value;
  ArgumentType _type;
};
