#include <iostream>
#include <cassert>
#include <ctype.h>
#include <cstdio>
#include <cstdlib>

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

void outputLine(unsigned char bytes[], int numBytes, int offset) {
  for (int i = 0; i < numBytes; ++i) {
    fprintf(stdout, "0x%02x, ", (int)bytes[i]);
  }
  fprintf(stdout, "\n");
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
      outputLine(bytes, bytesRead, offset);
      free(line);
      line = NULL;
    }
    fclose(f);
  }
  return 0;
}
