BISON:=bison

LDLIBS+=-ll -ly

CXXFLAGS+=-g
CFLAGS+=-g
LDFLAGS+=-g

asm: main.o parser.tab.o lexer.o command.o Commands.o symbolTable.o argumentHelpers.o
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

%.tab.c: %.y
	$(BISON) $+

parser.tab.h: parser.y
	$(BISON) -d $+

lexer.o: lexer.c parser.tab.h Argument.h symbolTable.h

parser.tab.o: parser.tab.h parser.tab.c Argument.h command.h

main.o: main.cpp Commands.h Argument.h argumentHelpers.hpp

Commands.o: Commands.cpp Commands.h

command.o: command.cpp command.hpp command.h Commands.h

symbolTable.o: symbolTable.cpp symbolTable.hpp symbolTable.h

argumentHelpers.o: argumentHelpers.cpp argumentHelpers.hpp Argument.hpp Argument.h

