#include "argumentHelpers.hpp"

RawArgument identifierArg(const char* identifier) {
  RawArgument argument;
  argument.type = IDENTIFIER_ARGUMENT;
  argument.identifier = identifier;
  return argument;
}

RawArgument numberArg(int byte) {
  RawArgument argument;
  argument.type = VALUE_ARGUMENT;
  argument.value = byte;
  return argument;
}

RawArgument dereferencedValueArg(int address) {
  RawArgument argument;
  argument.type = DEREFERENCED_VALUE_ARGUMENT;
  argument.value = address;
  return argument;
}

RawArgument dereferencedIdentifierArg(const char* identifier) {
  RawArgument argument;
  argument.type = DEREFERENCED_IDENTIFIER_ARGUMENT;
  argument.identifier = identifier;
  return argument;
}
