#pragma once

class Commands {
public:
  virtual ~Commands() = 0;
  virtual void command0(const char* mnemonic) = 0;
};

