#include "errorChecking.hpp"
#include "Argument.h"
#include <cstdio>
#include <cstdlib>

extern int yylineno;
extern const char* filename;

void verifyIsValueArgument(const Argument& arg, int argumentNumber) {
  if (arg.type == VALUE_ARGUMENT)
    return;
  // TODO check if address
  fprintf(stderr, "%s:%d: error: Expected argument %d to be a number, but received identifier %s\n", filename, yylineno, argumentNumber, arg.identifier);
  exit(-1);
}

