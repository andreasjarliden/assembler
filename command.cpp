#include "command.hpp"
#include "Commands.h"
#include <iostream>
#include <cassert>

extern Commands& COMMANDS;

int command0(char* command) {
  COMMANDS.command0(command);
  return 1;
}

void printArgument(Argument* arg) {
  if (arg->type == 1) {
    std::cout << "\tidentifier argument " << arg->identifier << std::endl;
  }
  else if (arg->type == 2) {
    std::cout << "\tnumeric argument " << arg->value << std::endl;
  }
  else {
    assert(0 && "unknown argument type");
  }
}

int command2(char* command, Argument* arg1, Argument* arg2) {
  std::cout << "Command with 2 args:\n";
  printArgument(arg1);
  printArgument(arg2);
  return 0;
}


