all:
	@test -f ./rars1_6.jar || (echo "Missing RARS" && exit 1)
	bison -d parser.y
	flex scanner.l
	gcc lex.yy.c parser.tab.c intermediate.c -o cparser
	./cparser test_function.c
	java -jar rars1_6.jar nc program.s

parser:
	bison -d parser.y
	flex scanner.l
	gcc lex.yy.c parser.tab.c intermediate.c -o cparser
	rm -f lex.yy.c parser.tab.c parser.tab.h

debug:
	bison -d -t parser.y
	flex -d scanner.l
	gcc -g -Wall -Wextra lex.yy.c parser.tab.c intermediate.c -o cparser

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h program.s cparser 

clean_all:
	rm -f lex.yy.c parser.tab.c parser.tab.h program.s cparser *.lis 
