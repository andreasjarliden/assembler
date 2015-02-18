#include "Commands.hpp"
#include "Assembler.hpp"
#include "MachineCode.hpp"
#include <cassert>

extern "C" {

int yyparse();

}

Assembler ASSEMBLER;
Commands& COMMANDS = ASSEMBLER;

int main() {
  yyparse();
  ASSEMBLER.machineCode().print();
  return 0;
}

