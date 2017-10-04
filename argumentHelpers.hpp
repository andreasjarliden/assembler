#pragma once

#include "Argument.hpp"

RawArgument identifierArg(const char* identifier);
RawArgument numberArg(int byte);
RawArgument dereferencedValueArg(int address);
RawArgument dereferencedIdentifierArg(const char* identifier);
