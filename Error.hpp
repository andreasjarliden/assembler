#pragma once

class Error {
public:
  Error(std::string message) : _message(message) {}

  const std::string& message() const { return _message; }

private:
  std::string _message;
};

