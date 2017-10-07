#include "testUtilities.hpp"
#include "Assembler.hpp"
#include "Segments.hpp"
#include "Segment.hpp"
#include <iostream>

void _isEqualToBytes(const Segment& segment,
    Byte expectedBytes[],
    int numberOfBytes,
    const char* testFunction) {
  if (!segment.isEqual(expectedBytes, numberOfBytes)) {
    std::cout << testFunction << ":\n";
    std::cout << "Expected \n\t" << std::hex;
    std::copy(expectedBytes, expectedBytes + numberOfBytes, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "but received \n\t" << std::hex;
    std::copy(segment.beginCode(), segment.endCode(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
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
    std::cout << testFunction << ":\n";
    std::cout << "Expected \n\t" << std::hex;
    std::copy(expectedBytes, expectedBytes + numberOfBytes, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "but received \n\t" << std::hex;
    std::copy(segment.beginCode(), segment.endCode(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
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
    std::cout << testFunction << ":\n";
    std::cout << "Expected \n\t<" << expected << ">\nbut received \n\t<" << actual << ">" << std::endl;
  }
  else {
    std::cout << ".";
    std::cout.flush();
  }
}
