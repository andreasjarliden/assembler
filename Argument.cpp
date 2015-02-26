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

bool Argument::is8BitRegister() const {
  if (_type != IDENTIFIER_ARGUMENT)
    return false;
  if (strlen(_identifier) != 1)
    return false;
  switch (_identifier[0]) {
    case 'a':
    case 'A':
    case 'b':
    case 'B':
    case 'c':
    case 'C':
    case 'd':
    case 'D':
    case 'e':
    case 'E':
    case 'h':
    case 'H':
    case 'l':
    case 'L':
      return true;
    default:
      return false;
  }
}

bool Argument::is16BitRegister() const {
  if (_type != IDENTIFIER_ARGUMENT)
    return false;
  if (strlen(_identifier) != 2)
    return false;
  if (strcmp(_identifier, "bc") == 0)
    return true;
  if (strcmp(_identifier, "BC") == 0)
    return true;
  if (strcmp(_identifier, "de") == 0)
    return true;
  if (strcmp(_identifier, "DE") == 0)
    return true;
  if (strcmp(_identifier, "hl") == 0)
    return true;
  if (strcmp(_identifier, "HL") == 0)
    return true;
  if (strcmp(_identifier, "sp") == 0)
    return true;
  if (strcmp(_identifier, "SP") == 0)
    return true;
  return false;
}


Argument Argument::asAddressValue() const {
  assert(_type == VALUE_ARGUMENT || _type == ADDRESS_VALUE_ARGUMENT);
  Argument out = *this;
  out._type = ADDRESS_VALUE_ARGUMENT;
  return out;
}
