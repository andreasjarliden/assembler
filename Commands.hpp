#pragma once

class Argument;

class Commands {
public:
  virtual ~Commands() = 0;
  virtual void command0(const char* mnemonic) = 0;
  virtual void command1(const char* mnemonic,
      const Argument& arg) = 0;
  virtual void command2(const char* mnemonic,
      const Argument& arg1,
      const Argument& arg2) = 0;
  virtual void label(const char* label) = 0;
};

