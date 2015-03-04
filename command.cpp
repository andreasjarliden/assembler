#include "command.hpp"
#include "Commands.hpp"
#include <iostream>
#include <cassert>

extern Commands& COMMANDS;

int command0(const char* command) {
  COMMANDS.command0(command);
  return 1;
}

void printArgument(RawArgument* arg) {
  if (arg->type == IDENTIFIER_ARGUMENT) {
    std::cout << "\tidentifier argument " << arg->identifier << std::endl;
  }
  else if (arg->type == VALUE_ARGUMENT) {
    std::cout << "\tnumeric argument " << arg->value << std::endl;
  }
  else {
    assert(0 && "unknown argument type");
  }
}

int command1(const char* command, RawArgument* arg) {
  COMMANDS.command1(command, *arg);
  return 0;
}

int command2(const char* command, RawArgument* arg1, RawArgument* arg2) {
  COMMANDS.command2(command, *arg1, *arg2);
  return 0;
}

void label(const char* label) {
  COMMANDS.label(label);
}

void metaCommand1(const char* metaCommand, RawArgument* arg) {
  COMMANDS.metaCommand1(metaCommand, *arg);
}

void metaCommand2(const char* metaCommand, const char* identifier, RawArgument* arg) {
  COMMANDS.metaCommand2(metaCommand, identifier, *arg);
}


