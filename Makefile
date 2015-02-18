BISON:=bison

LDLIBS+=-ll -ly

CXXFLAGS+=-g
CFLAGS+=-g
LDFLAGS+=-g

asm: main.o parser.tab.o lexer.o command.o Commands.o
	$(CXX) -o $@ $(LDFLAGS) $+ $(LDLIBS)

%.tab.c: %.y
	$(BISON) $+

parser.tab.h: parser.y
	$(BISON) -d $+

lexer.o: lexer.c parser.tab.h Argument.h

parser.tab.o: parser.tab.h parser.tab.c Argument.h command.h

main.cpp: Commands.h

Commands.cpp: Commands.h

command.cpp: command.hpp command.h Commands.h


