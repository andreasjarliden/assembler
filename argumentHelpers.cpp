#include "argumentHelpers.hpp"

Argument identifierArg(const char* identifier) {
  Argument argument;
  argument.type = IDENTIFIER_ARGUMENT;
  argument.identifier = identifier;
  return argument;
}

Argument byteArg(unsigned char byte) {
  Argument argument;
  argument.type = VALUE_ARGUMENT;
  argument.value = byte;
  return argument;
}
