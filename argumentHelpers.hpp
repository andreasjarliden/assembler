#pragma once

#include "Argument.hpp"

RawArgument identifierArg(const char* identifier);
RawArgument numberArg(int byte);
RawArgument addressArg(int address);
RawArgument addressIdentifierArg(const char* identifier);
