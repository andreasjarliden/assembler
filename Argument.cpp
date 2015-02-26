#include "Argument.hpp"

Argument Argument::createWithRawArgument(const RawArgument& in) {
  Argument out;
  out.type = in.type;
  out.value = in.value;
  out.identifier = in.identifier;
  return out;
}

bool Argument::isAddress() const {
  return type == ADDRESS_VALUE_ARGUMENT || type == ADDRESS_IDENTIFIER_ARGUMENT;
}
