#pragma once

#include <string>

class Assembler;
class Segment;

extern int NUM_FAILURES;

void printTestSummary();

void _isEqualToBytes(const Segment& segment,
    unsigned char expectedBytes[],
    int numberOfBytes,
    const char* testFunction);

void _isEqualToBytes(const Assembler& assembler,
    unsigned char expectedBytes[],
    int numberOfBytes,
    const char* testFunction);

void _equalStrings(const std::string& actual,
    const std::string& expected,
    const char* testFunction);

#define isEqualToBytes(segmentOrAssembler, expectedBytes, numberOfBytes) \
  _isEqualToBytes(segmentOrAssembler, expectedBytes, numberOfBytes, __func__)

#define equalStrings(actual, expected) \
  _equalStrings(actual, expected, __func__)
