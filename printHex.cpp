#include "printHex.hpp"
#include "Segments.hpp"
#include "Segment.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

void printHex(const Segment& segment, std::ostream& stream) {
  for (int i = 0; i < segment.size(); ) {
    stream << std::hex << std::setw(4) << std::setfill('0') << i + segment.base() << ": ";
    for (int j = 0; j < 8 && i < segment.size(); ++j, ++i) {
      stream << std::hex << std::setw(2) << std::setfill('0') << (int)segment.get(i) << " ";
    }
    stream << std::endl;
  }
}

void printHex(const Segments& segments, std::ostream& stream) {
  for (int i = 0; i < segments.numberOfSegments(); ++i) {
    printHex(segments.index(i), stream);
  }
}

std::string asHexString(const Segments& segments) {
  std::stringstream stream;
  printHex(segments, stream);
  return stream.str();
}

