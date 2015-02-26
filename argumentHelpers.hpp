#pragma once

#include "Argument.hpp"

Argument identifierArg(const char* identifier);
Argument numberArg(int byte);
Argument addressArg(int address);
Argument addressIdentifierArg(const char* identifier);
