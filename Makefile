BISON:=bison

LDLIBS+=-ll -ly

CXXFLAGS+=-g -std=c++11
CFLAGS+=-g
LDFLAGS+=-g

default: test asm
	./test

asm: main.o parser.tab.o lexer.o command.o Commands.o symbolTable.o argumentHelpers.o MachineCode.o instructions.o Assembler.o LabelTable.o
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

test: test.o Commands.o argumentHelpers.o MachineCode.o instructions.o Assembler.o LabelTable.o
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

test.o: test.cpp argumentHelpers.hpp Assembler.hpp MachineCode.hpp Commands.hpp LabelTable.hpp

%.tab.c: %.y
	$(BISON) $+

parser.tab.h: parser.y
	$(BISON) -d $+

lexer.o: lexer.c parser.tab.h Argument.h symbolTable.h

parser.tab.o: parser.tab.h parser.tab.c Argument.h command.h

main.o: main.cpp Commands.hpp MachineCode.hpp instructions.hpp Assembler.hpp LabelTable.hpp

Commands.o: Commands.cpp Commands.hpp

command.o: command.cpp command.hpp command.h Commands.hpp

symbolTable.o: symbolTable.cpp symbolTable.hpp symbolTable.h

argumentHelpers.o: argumentHelpers.cpp argumentHelpers.hpp Argument.hpp Argument.h

MachineCode.o: MachineCode.cpp MachineCode.hpp

instructions.o: instructions.cpp instructions.hpp LabelTable.hpp

Assembler.o: Assembler.cpp Assembler.hpp MachineCode.hpp Commands.hpp instructions.hpp LabelTable.hpp

LabelTable.o: LabelTable.cpp LabelTable.hpp
