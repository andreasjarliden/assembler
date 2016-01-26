#pragma once

#include <string>

class Argument;

void verifyIsValueArgument(const Argument& arg, int argumentNumber);
void error(const std::string& message);
void warning(const std::string& message);

extern int NUMBER_OF_ERRORS;
