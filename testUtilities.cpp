#include "testUtilities.hpp"
#include "Assembler.hpp"
#include "Segments.hpp"
#include "Segment.hpp"
#include "printHex.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <iterator>

extern "C" {

int yyparse();
extern FILE* yyin;

}

int NUM_FAILURES = 0;

void _isEqualToBytes(const Segment& segment,
    Byte expectedBytes[],
    int numberOfBytes,
    const char* testFunction) {
  if (!segment.isEqual(expectedBytes, numberOfBytes)) {
    std::cout << "\n" << testFunction << ":\n";
    std::cout << "Expected \n\t" << std::hex;
    std::copy(expectedBytes, expectedBytes + numberOfBytes, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "but received \n\t" << std::hex;
    std::copy(segment.beginCode(), segment.endCode(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    NUM_FAILURES++;
  }
  else {
    std::cout << ".";
    std::cout.flush();
  }
}

void _isEqualToBytes(const Assembler& assembler,
    Byte expectedBytes[],
    int numberOfBytes,
    const char* testFunction) {
  const Segment& segment = assembler.segments().index(0);
  if (!segment.isEqual(expectedBytes, numberOfBytes)) {
    std::cout << "\n" << testFunction << ":\n";
    std::cout << "Expected \n\t" << std::hex;
    std::copy(expectedBytes, expectedBytes + numberOfBytes, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "but received \n\t" << std::hex;
    std::copy(segment.beginCode(), segment.endCode(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    NUM_FAILURES++;
  }
  else {
    std::cout << ".";
    std::cout.flush();
  }
}

void _equalStrings(const std::string& actual,
    const std::string& expected,
    const char* testFunction) {
  if (actual != expected) {
    std::cout << "\n" << testFunction << ":\n";
    std::cout << "Expected \n\t<" << expected << ">\nbut received \n\t<" << actual << ">" << std::endl;
    NUM_FAILURES++;
  }
  else {
    std::cout << ".";
    std::cout.flush();
  }
}

void printTestSummary() {
  if (NUM_FAILURES == 0) {
    std::cout << "\nOK" << std::endl;
  }
  else {
    std::cout << "\nFAIL " << NUM_FAILURES << " failed asserts\n" << std::endl;
  }
}

std::string compileProgram(Assembler& assembler, const char* p) {
  extern Commands* COMMANDS;
  COMMANDS = &assembler;
  yyin = fmemopen((void*)p, strlen(p), "r");
  yyparse();
  fclose(yyin);
  assembler.resolveRemaining();
  std::stringstream outputStream;
  printHex(assembler.segments(), outputStream);
  return outputStream.str();
}

