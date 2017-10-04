#include "Commands.hpp"
#include "Assembler.hpp"
#include "MachineCode.hpp"
#include "errorChecking.hpp"
#include "printHex.hpp"
#include "Error.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

extern "C" {

int yyparse();

}

Assembler ASSEMBLER;
Commands& COMMANDS = ASSEMBLER;
const char* filename = "STDIN";
const char* outputFilename = "/dev/stdout";
const char* symbolsFilename = nullptr;
const char* relocationsFilename = nullptr;
std::ofstream outputFileStream;

int main(int argc, const char* argv[]) {
  try {
    int numOptions = 0;
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == 'o') {
          outputFilename = argv[i+1];
          numOptions += 2;
        }
        else if (argv[i][1] == 's') {
          symbolsFilename = argv[i+1];
          numOptions += 2;
        }
        else if (argv[i][1] == 'r') {
          relocationsFilename = argv[i+1];
          numOptions += 2;
        }
        else {
          throw Error(std::string("Unknown command line option ") + argv[i]);
        }
      }
    }
    if (argc > numOptions + 1) {
      extern FILE* yyin;
      filename = argv[numOptions + 1];
      FILE* f = fopen(filename, "r");
      if (!f) {
        fprintf(stderr, "fatal error: could not open input file %s\n", argv[1]);
        exit(-1);
      }
      yyin = f;
    }
    yyparse();
    ASSEMBLER.resolveRemaining();
    if (NUMBER_OF_ERRORS > 0)
      return 1;
    outputFileStream.open(outputFilename);
    printHex(ASSEMBLER.segments(), outputFileStream);
    if (symbolsFilename) {
      std::ofstream symbolsStream(symbolsFilename);
      ASSEMBLER.printSymbolTable(symbolsStream);
    }
    if (relocationsFilename) {
      ASSEMBLER.printRelocations(std::cout);
    }
    return 0;
  }
  catch (const Error& e) {
    std::cerr << e.message() << std::endl;
  }
}

