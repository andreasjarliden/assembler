#include "Commands.h"
#include <algorithm>
#include <vector>
#include <cassert>
#include <map>
#include <string>
#include <iostream>

extern "C" {

int yyparse();

}

typedef unsigned char Byte;

class MachineCode {
public:
  void add(Byte b) {
    _code.push_back(b);
  }

  bool isEqual(const Byte* expectedBytes, int expectedSizeInBytes) const {
    if (_code.size() != expectedSizeInBytes)
      return false;
    return std::equal(_code.begin(), _code.end(), expectedBytes);
  }

  void print() const {
    for (int i = 0; i < _code.size(); ++i) {
      std::cout << i << ": " << std::hex << (int)_code[i] << std::endl;
    }
  }

private:
  std::vector<Byte> _code;
};


class Instruction {
  public:
    virtual ~Instruction() = 0;
    virtual void translate(MachineCode& code) const = 0;
};

Instruction::~Instruction() {}

class NegInstruction : public Instruction {
  void translate(MachineCode& code) const {
    code.add(0xed);
    code.add(0x44);
  }
};

class CplInstruction : public Instruction {
  void translate(MachineCode& code) const {
    code.add(0x2f);
  }
};

class Assembler : public Commands {
public:
  Assembler() {
    addInstruction("neg", _negInstruction);
    addInstruction("cpl", _cplInstruction);
  }

  ~Assembler() {}

  void command0(const char* mnemonic) {
    const Instruction* i = _mnemonics[std::string(mnemonic)];
    assert(i);
    i->translate(_machineCode);
  }

  const MachineCode& machineCode() const {
    return _machineCode;
  }

private:
  void addInstruction(const char* mnemonic, const Instruction& instruction) {
    _mnemonics[std::string(mnemonic)] = &instruction;
  }

  NegInstruction _negInstruction;
  CplInstruction _cplInstruction;
  std::map<std::string, const Instruction*> _mnemonics;
  MachineCode _machineCode;
};

Assembler ASSEMBLER;
Commands& COMMANDS = ASSEMBLER;

void testSingleOneByteInstruction() {
  Assembler assembler;
  assembler.command0("cpl");
  Byte expectedBytes[] = { 0x2f };
  assert(assembler.machineCode().isEqual(expectedBytes, 1));
}

void testSingleTwoByteInstruction() {
  Assembler assembler;
  assembler.command0("neg");
  Byte expectedBytes[] = { 0xed, 0x44 };
  assert(assembler.machineCode().isEqual(expectedBytes, 2));
}

void testTwoInstructions() {
  Assembler assembler;
  assembler.command0("cpl");
  assembler.command0("neg");
  Byte expectedBytes[] = { 0x2f, 0xed, 0x44 };
  assert(assembler.machineCode().isEqual(expectedBytes, 3));
}

void testNoSuchInstruction() {}

int main() {
  testSingleOneByteInstruction();
  testSingleTwoByteInstruction();
  testTwoInstructions();
  yyparse();
  ASSEMBLER.machineCode().print();
  return 0;
}

