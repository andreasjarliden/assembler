#include "Argument.hpp"
#include <cassert>

Argument::Argument()
  : _identifier(nullptr) {}

Argument Argument::createWithRawArgument(const RawArgument& in) {
  Argument out;
  out._type = in.type;
  out._value = in.value;
  out._identifier = in.identifier;
  return out;
}

const char* Argument::identifier() const {
  return _identifier;
}

int Argument::value() const {
  return _value;
}

ArgumentType Argument::type() const {
  return _type;
}

bool Argument::isAddress() const {
  return _type == ADDRESS_VALUE_ARGUMENT || _type == ADDRESS_IDENTIFIER_ARGUMENT;
}

Argument Argument::asAddressValue() const {
  assert(_type == VALUE_ARGUMENT || _type == ADDRESS_VALUE_ARGUMENT);
  Argument out = *this;
  out._type = ADDRESS_VALUE_ARGUMENT;
  return out;
}
