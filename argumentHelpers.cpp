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

RawArgument addressArg(int address) {
  RawArgument argument;
  argument.type = ADDRESS_VALUE_ARGUMENT;
  argument.value = address;
  return argument;
}

RawArgument addressIdentifierArg(const char* identifier) {
  RawArgument argument;
  argument.type = ADDRESS_IDENTIFIER_ARGUMENT;
  argument.identifier = identifier;
  return argument;
}
