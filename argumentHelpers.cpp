#include "argumentHelpers.hpp"

Argument identifierArg(const char* identifier) {
  Argument argument;
  argument.type = IDENTIFIER_ARGUMENT;
  argument.identifier = identifier;
  return argument;
}

Argument numberArg(int byte) {
  Argument argument;
  argument.type = VALUE_ARGUMENT;
  argument.value = byte;
  return argument;
}

Argument addressArg(int address) {
  Argument argument;
  argument.type = ADDRESS_ARGUMENT;
  argument.value = address;
  return argument;
}
