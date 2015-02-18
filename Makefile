BISON:=bison

LDLIBS+=-ll -ly

CXXFLAGS+=-g
CFLAGS+=-g
LDFLAGS+=-g

asm: main.o parser.tab.o lexer.o command.o Commands.o symbolTable.o argumentHelpers.o MachineCode.o
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

%.tab.c: %.y
	$(BISON) $+

parser.tab.h: parser.y
	$(BISON) -d $+

lexer.o: lexer.c parser.tab.h Argument.h symbolTable.h

parser.tab.o: parser.tab.h parser.tab.c Argument.h command.h

main.o: main.cpp Commands.hpp Argument.h Argument.hpp argumentHelpers.hpp MachineCode.hpp

Commands.o: Commands.cpp Commands.hpp

command.o: command.cpp command.hpp command.h Commands.hpp

symbolTable.o: symbolTable.cpp symbolTable.hpp symbolTable.h

argumentHelpers.o: argumentHelpers.cpp argumentHelpers.hpp Argument.hpp Argument.h

MachineCode.o: MachineCode.cpp MachineCode.hpp

