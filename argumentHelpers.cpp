#include "argumentHelpers.hpp"

Argument identifierArg(const char* identifier) {
  Argument argument;
  argument.type = 1;
  argument.identifier = identifier;
  return argument;
}

Argument byteArg(unsigned char byte) {
  Argument argument;
  argument.type = 2;
  argument.value = byte;
  return argument;
}
