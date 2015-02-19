#include "Commands.hpp"
#include "Assembler.hpp"
#include "MachineCode.hpp"

extern "C" {

int yyparse();

}

Assembler ASSEMBLER;
Commands& COMMANDS = ASSEMBLER;

int main() {
  yyparse();
  ASSEMBLER.machineCode().printHex();
  return 0;
}

