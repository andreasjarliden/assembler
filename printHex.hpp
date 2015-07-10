#pragma once

#include <string>
#include <iostream>

class Segments;

void printHex(const Segments&, std::ostream& = std::cout);
std::string asHexString(const Segments&);
