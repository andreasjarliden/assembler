#pragma once

#include <string>

class Assembler;
class Segment;

void isEqualToBytes(const Segment& segment,
    unsigned char expectedBytes[],
    int numberOfBytes);

void isEqualToBytes(const Assembler& assembler,
    unsigned char expectedBytes[],
    int numberOfBytes);

void equalStrings(const std::string& actual,
    const std::string& expected);
