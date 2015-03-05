#include "testUtilities.hpp"
#include "Assembler.hpp"
#include "MachineCode.hpp"
#include <iostream>

void isEqualToBytes(const Assembler& assembler,
    Byte expectedBytes[],
    int numberOfBytes) {
  if (!assembler.machineCode().isEqual(expectedBytes, numberOfBytes)) {
    std::cout << "Expected \n\t" << std::hex;
    std::copy(expectedBytes, expectedBytes + numberOfBytes, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "but received \n\t" << std::hex;
    std::copy(assembler.machineCode().beginCode(), assembler.machineCode().endCode(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    exit(-1);
  }
}
