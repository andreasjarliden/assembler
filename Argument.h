#pragma once

enum ArgumentType { IDENTIFIER_ARGUMENT, VALUE_ARGUMENT, DEREFERENCED_VALUE_ARGUMENT, DEREFERENCED_IDENTIFIER_ARGUMENT, STRING_ARGUMENT };

struct RawArgument {
  enum ArgumentType type;
  int value;
  const char* identifier;
};

