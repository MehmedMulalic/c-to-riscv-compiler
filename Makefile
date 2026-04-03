all:
	bison -d parser.y
	flex scanner.l
	gcc lex.yy.c parser.tab.c intermediate.c -o cparser

parser:
	bison -d parser.y
	flex scanner.l
	gcc lex.yy.c parser.tab.c intermediate.c -o cparser
	rm -f lex.yy.c parser.tab.c parser.tab.h

debug:
	bison -d -t parser.y
	flex -d scanner.l
	gcc -g lex.yy.c parser.tab.c intermediate.c -o cparser

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h intermediate.c intermediate.h cparser 

clean_all:
	rm -f lex.yy.c parser.tab.c parser.tab.h intermediate.c intermediate.h cparser *.lis 
