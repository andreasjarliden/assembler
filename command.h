#pragma once

#include "Argument.h"

int command0(const char* command);
int command1(const char* command, struct Argument* arg);
int command2(const char* command, struct Argument* arg1, struct Argument* arg2);
void label(const char* label);
void metaCommand3(const char* metaCommand, const char* identifier, struct Argument* arg);
