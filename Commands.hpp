#pragma once

class RawArgument;

class Commands {
public:
  virtual ~Commands() = 0;
  virtual void command0(const char* mnemonic) = 0;
  virtual void command1(const char* mnemonic,
      const RawArgument& arg) = 0;
  virtual void command2(const char* mnemonic,
      const RawArgument& arg1,
      const RawArgument& arg2) = 0;
  virtual void label(const char* label) = 0;
  virtual void metaCommand1(const char* command,
      const RawArgument& arg) = 0;
  virtual void metaCommand2(const char* command,
      const char* identifier,
      const RawArgument& arg) = 0;
};

