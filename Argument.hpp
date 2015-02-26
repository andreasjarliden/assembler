#pragma once

extern "C" {
#include "Argument.h"
}

struct Argument {
  static Argument createWithRawArgument(const RawArgument&);
  bool isAddress() const;

  enum ArgumentType type;
  int value;
  const char* identifier;
};
