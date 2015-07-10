BISON:=bison

LDLIBS+=-ll -ly

CXXFLAGS+=-g -std=c++11
CFLAGS+=-g
LDFLAGS+=-g

default: test asm
	./test

hex2i8hex: hex2i8hex.cpp
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

hex2h: hex2h.cpp
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

hex2bin: hex2bin.cpp
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

asm: main.o parser.tab.o lexer.o command.o Commands.o stringTable.o argumentHelpers.o MachineCode.o instructions.o Assembler.o LabelTable.o errorChecking.o DelayedAddresses.o Argument.o Segments.o printHex.o
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

test: test.o Commands.o argumentHelpers.o MachineCode.o instructions.o Assembler.o LabelTable.o errorChecking.o DelayedAddresses.o Argument.o testUtilities.cpp testJumpGroup.o testLoadGroup.o testArithmeticGroup.o Segments.o
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

%.tab.c: %.y
	$(BISON) $+

parser.tab.h: parser.y
	$(BISON) -d $+

lexer.o: lexer.c parser.tab.h Argument.h stringTable.h

parser.tab.o: parser.tab.h parser.tab.c Argument.h command.h

main.o: main.cpp Commands.hpp MachineCode.hpp Assembler.hpp errorChecking.hpp printHex.hpp

printHex.o: Segments.hpp Segment.hpp printHex.cpp

Commands.o: Commands.cpp Commands.hpp

command.o: command.cpp command.hpp command.h Commands.hpp

stringTable.o: stringTable.cpp stringTable.hpp stringTable.h

argumentHelpers.o: argumentHelpers.cpp argumentHelpers.hpp Argument.hpp Argument.h

MachineCode.o: MachineCode.cpp MachineCode.hpp Error.hpp

instructions.o: instructions.cpp LabelTable.hpp InstructionsHost.hpp Error.hpp Argument.hpp

Assembler.o: Assembler.cpp Assembler.hpp MachineCode.hpp Commands.hpp LabelTable.hpp Argument.hpp Argument.h InstructionsHost.hpp Error.hpp Segments.hpp

LabelTable.o: LabelTable.cpp LabelTable.hpp

errorChecking.o: errorChecking.cpp errorChecking.hpp Argument.hpp Argument.h

DelayedAddresses.o: DelayedAddresses.cpp DelayedAddresses.hpp MachineCode.hpp LabelTable.hpp

Argument.o: Argument.hpp Argument.h Argument.cpp

test.o: test.cpp argumentHelpers.hpp Assembler.hpp MachineCode.hpp testUtilities.hpp testJumpGroup.hpp

testUtilities.o: testUtilities.hpp testUtilities.cpp Assembler.hpp Segments.hpp Segment.hpp

testJumpGroup.o: testJumpGroup.hpp testJumpGroup.cpp testUtilities.hpp Assembler.hpp MachineCode.hpp

testLoadGroup.o: testJumpGroup.hpp testJumpGroup.cpp testUtilities.hpp Assembler.hpp MachineCode.hpp

testArithmeticGroup.o: testArithmeticGroup.hpp testArithmeticGroup.cpp testUtilities.hpp Assembler.hpp MachineCode.hpp

