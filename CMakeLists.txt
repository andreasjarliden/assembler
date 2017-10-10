cmake_minimum_required(VERSION 3.0)

find_package(bison REQUIRED)
find_package(flex REQUIRED)

bison_target(parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
flex_target(lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${PROJECT_SOURCE_DIR})

set(commonSources
  Commands.cpp
  stringTable.cpp
  argumentHelpers.cpp
  MachineCode.cpp
  instructions.cpp
  Assembler.cpp
  LabelTable.cpp
  errorChecking.cpp
  DelayedAddresses.cpp
  Argument.cpp
  Segments.cpp
  printHex.cpp
  )

set(testSources
  test.cpp
  testUtilities.cpp
  testJumpGroup.cpp
  testLoadGroup.cpp
  testArithmeticGroup.cpp
  )

add_executable(asm main.cpp command.cpp ${commonSources} ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS})
add_executable(unitTests ${commonSources} ${testSources})
add_executable(hex2i8hex hex2i8hex.cpp)
add_executable(hex2h hex2h.cpp)
add_executable(hex2bin hex2bin.cpp)

#TODO duplication
set_property(TARGET asm PROPERTY CXX_STANDARD 11)
set_property(TARGET asm PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET unitTests PROPERTY CXX_STANDARD 11)
set_property(TARGET unitTests PROPERTY CXX_STANDARD_REQUIRED ON)

enable_testing()

add_test(unit unitTests)

set(CTEST_OUTPUT_ON_FAILURE ON)

