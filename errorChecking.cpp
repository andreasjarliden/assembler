#include "errorChecking.hpp"
#include "Argument.hpp"
#include <cstdio>
#include <cstdlib>

extern int yylineno;
extern const char* filename;

int NUMBER_OF_ERRORS;

void verifyIsValueArgument(const Argument& arg, int argumentNumber) {
  if (arg.type() == VALUE_ARGUMENT)
    return;
  // TODO check if address
  fprintf(stderr, "%s:%d: error: Expected argument %d to be a number, but received identifier %s\n", filename, yylineno, argumentNumber, arg.identifier());
  ++NUMBER_OF_ERRORS;
}

void error(const std::string& message) {
  fprintf(stderr, "%s:%d: error: %s\n", filename, yylineno, message.c_str());
  ++NUMBER_OF_ERRORS;
}

