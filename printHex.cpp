#include "printHex.hpp"
#include "Segments.hpp"
#include "Segment.hpp"
#include <iostream>
#include <iomanip>

void printHex(const Segment& segment) {
  for (int i = 0; i < segment.size(); ) {
    std::cout << std::hex << std::setw(4) << std::setfill('0') << i << ": ";
    for (int j = 0; j < 8 && i < segment.size(); ++j, ++i) {
      std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)segment.get(i) << " ";
    }
    std::cout << std::endl;
  }
}

void printHex(const Segments& segments) {
  for (int i = 0; i < segments.numberOfSegments(); ++i) {
    printHex(segments.index(i));
  }
}
