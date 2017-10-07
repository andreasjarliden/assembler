#include "Argument.hpp"
#include "Error.hpp"
#include <cassert>
#include <cstring>
#include <string>

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
  if (!hasValue())
    throw Error(std::string("Expected a value"));
  return _value;
}

Byte Argument::byteValue() const {
  int v = value();
  if (v < -128) {
    throw Error(std::string("8 bit value <-128: ") + std::to_string(v));
  }
  if (v > 255) {
    throw Error(std::string("8 bit value >-128: ") + std::to_string(v));
  }
  return static_cast<unsigned char>(v);
}

std::string Argument::string() const {
  if (!isString())
    throw Error(std::string("Expected a string"));
  return _identifier;
}

unsigned char Argument::ioAddress() const {
  if (type() == IDENTIFIER_ARGUMENT || type() == DEREFERENCED_IDENTIFIER_ARGUMENT) {
    throw Error(std::string("Expected numeric IO address, but got ") + identifier());
  }
  assert(value() >= 0);
  assert(value() <= 255);
  return value();
}

ArgumentType Argument::type() const {
  return _type;
}

bool Argument::isDereferenced() const {
  return _type == DEREFERENCED_VALUE_ARGUMENT || _type == DEREFERENCED_IDENTIFIER_ARGUMENT;
}

bool Argument::isValue() const {
  return _type == VALUE_ARGUMENT || _type == IDENTIFIER_ARGUMENT;
}

bool Argument::hasValue() const {
  return _type == VALUE_ARGUMENT || _type == DEREFERENCED_VALUE_ARGUMENT;
}

bool Argument::isString() const {
  return _type == STRING_ARGUMENT;
}

bool Argument::isIdentifier() const {
  return _type == IDENTIFIER_ARGUMENT || _type == DEREFERENCED_IDENTIFIER_ARGUMENT;
}

bool Argument::isIdentifier(Dereferenced deref) const {
  return (deref == DEREFERENCED && _type == DEREFERENCED_IDENTIFIER_ARGUMENT) || 
    (deref == NOT_DEREFERENCED && _type == IDENTIFIER_ARGUMENT);
}

bool Argument::is8BitRegister(Dereferenced deref) const {
  if (!isIdentifier(deref))
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

bool Argument::is16BitRegister(Dereferenced deref) const {
  if (!isIdentifier(deref))
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

bool Argument::isHL(Dereferenced deref) const {
  return isIdentifier(deref) && strcasecmp(identifier(), "hl") == 0;
}

bool Argument::isDE(Dereferenced deref) const {
  return isIdentifier(deref) && strcasecmp(identifier(), "de") == 0;
}

bool Argument::isIX(Dereferenced deref) const {
  return isIdentifier(deref) && strcasecmp(identifier(), "ix") == 0;
}

bool Argument::isIY(Dereferenced deref) const {
  return isIdentifier(deref) && strcasecmp(identifier(), "iy") == 0;
}

bool Argument::isA(Dereferenced deref) const {
  return isIdentifier(deref) && strcasecmp(identifier(), "a") == 0;
}

bool Argument::isI(Dereferenced deref) const {
  return isIdentifier(deref) && strcasecmp(identifier(), "i") == 0;
}

bool Argument::isFlag(const char* flag) const {
  return _type == IDENTIFIER_ARGUMENT && strcasecmp(identifier(), flag) == 0;
}

Argument Argument::asAddressValue() const {
  if (_type != VALUE_ARGUMENT && _type != DEREFERENCED_VALUE_ARGUMENT) {
    throw Error(std::string("Expected VALUE_ARGUMENT or DEREFERENCED_VALUE_ARGUMENT"));
  }
  Argument out = *this;
  out._type = DEREFERENCED_VALUE_ARGUMENT;
  return out;
}

std::string argumentTypeAsString(ArgumentType t) {
  switch (t) {
    case IDENTIFIER_ARGUMENT: return "IDENTIFIER_ARGUMENT";
    case VALUE_ARGUMENT: return "VALUE_ARGUMENT";
    case DEREFERENCED_VALUE_ARGUMENT: return "DEREFERENCED_VALUE_ARGUMENT";
    case DEREFERENCED_IDENTIFIER_ARGUMENT: return "DEREFERENCED_IDENTIFIER_ARGUMENT";
    case STRING_ARGUMENT: return "STRING_ARGUMENT";
    default: return "Unknown argument type";
  }
}

std::ostream& operator<<(std::ostream& s, const Argument& arg) {
  s << argumentTypeAsString(arg.type()) << " ";
  if (arg.isDereferenced()) {
    s << "dereferenced ";
  }
  if (arg.isValue()) {
    s << "value ";
  }
  if (arg.hasValue()) {
    s << "has value " << arg.value() << " ";
  }
  if (arg.isIdentifier()) {
    s << "identifier " << arg.identifier() << " ";
  }
  return s;
}

