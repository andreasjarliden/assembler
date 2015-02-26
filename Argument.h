#pragma once

enum ArgumentType { IDENTIFIER_ARGUMENT, VALUE_ARGUMENT, ADDRESS_VALUE_ARGUMENT, ADDRESS_IDENTIFIER_ARGUMENT };

struct Argument {
  enum ArgumentType type;
  int value;
  const char* identifier;
};
