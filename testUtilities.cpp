#include "testUtilities.hpp"
#include "Assembler.hpp"
#include "Segments.hpp"
#include "Segment.hpp"
#include <iostream>

void isEqualToBytes(const Segment& segment,
    Byte expectedBytes[],
    int numberOfBytes) {
  if (!segment.isEqual(expectedBytes, numberOfBytes)) {
    std::cout << "Expected \n\t" << std::hex;
    std::copy(expectedBytes, expectedBytes + numberOfBytes, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "but received \n\t" << std::hex;
    std::copy(segment.beginCode(), segment.endCode(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    exit(-1);
  }
}

void isEqualToBytes(const Assembler& assembler,
    Byte expectedBytes[],
    int numberOfBytes) {
  const Segment& segment = assembler.segments().index(0);
  if (!segment.isEqual(expectedBytes, numberOfBytes)) {
    std::cout << "Expected \n\t" << std::hex;
    std::copy(expectedBytes, expectedBytes + numberOfBytes, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "but received \n\t" << std::hex;
    std::copy(segment.beginCode(), segment.endCode(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    exit(-1);
  }
}

void equalStrings(const std::string& actual,
    const std::string& expected) {
  if (actual != expected) {
    std::cout << "Expected \n\t<" << expected << ">\nbut received \n\t<" << actual << ">" << std::endl;
    exit(-1);
  }
}
