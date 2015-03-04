#pragma once

#include "Argument.h"

int command0(const char* command);
int command1(const char* command, struct RawArgument* arg);
int command2(const char* command, struct RawArgument* arg1, struct RawArgument* arg2);
void label(const char* label);
void metaCommand1(const char* metaCommand, struct RawArgument* arg);
void metaCommand2(const char* metaCommand, const char* identifier, struct RawArgument* arg);
