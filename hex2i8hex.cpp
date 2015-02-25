#include <iostream>
#include <cassert>
#include <ctype.h>
#include <cstdio>

void readLine(char* line, unsigned char bytes[], int* bytesRead, int* offset) {
    int consumed;
    int nFields = sscanf(line, "%x:%n", offset, &consumed);
    assert(nFields == 1);
    line+=consumed;
    unsigned int value;
    for (*bytesRead = 0; sscanf(line, "%x%n", &value, &consumed) == 1; ++*bytesRead) {
      line+=consumed;
      bytes[*bytesRead] = (unsigned char)value;
    }
    for (; *line != 0; ++line) {
      if (!isspace(*line)) {
        fprintf(stderr, "Parsing error");
        exit(-1);
      }
    }
}

void outputI8HexDataRecord(unsigned char bytes[], int numBytes, int offset) {
  unsigned char checksum = 0;
  fputc(':', stdout);
  fprintf(stdout, "%02X", numBytes);
  checksum += numBytes;
  fprintf(stdout, "%04X", offset);
  checksum += offset & 0x0ff;
  checksum += (offset >> 8) & 0x0ff;
  fprintf(stdout, "%02X", 0); // Record type == DATA
  for (int i = 0; i < numBytes; ++i) {
    fprintf(stdout, "%02X", (int)bytes[i]);
    checksum += bytes[i];
  }
  checksum = -checksum;
  int checksumInt = checksum;
  fprintf(stdout, "%02X\n", checksumInt);
}

void outputI8HexEndOfFileRecord() {
  fprintf(stdout, ":00000001FF\n");
}

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    FILE* f = fopen(argv[i], "r");
    char* line = NULL;
    size_t len;
    unsigned char bytes[255];
    int bytesRead;
    int offset;
    while (getline(&line, &len, f) != -1) {
      readLine(line, bytes, &bytesRead, &offset);
      outputI8HexDataRecord(bytes, bytesRead, offset);
      free(line);
      line = NULL;
    }
  }
  outputI8HexEndOfFileRecord();
  return 0;
}
