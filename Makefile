all: parse utest_lex utest_parse

parse: build/main.o build/lex.o build/parse.o  build/ast.o
	c++ -Wall build/main.o build/lex.o build/parse.o  build/ast.o -o parse

build/main.o:
	c++ -Wall -c main.cpp -o build/main.o

build/lex.o:
	c++ -Wall -c lex.cpp -o build/lex.o

build/parse.o:
	c++ -Wall -c parse.cpp -o build/parse.o

build/ast.o:
	c++ -Wall -c ast.cpp -o build/ast.o

utest_lex: build/utest_lex.o  build/lex.o
	c++ -Wall build/utest_lex.o build/lex.o -o utest_lex

build/utest_lex.o:
	c++ -Wall -c utest_lex.cpp -o build/utest_lex.o

utest_parse: build/utest_parse.o build/lex.o build/parse.o  build/ast.o
	c++ -Wall build/utest_parse.o build/lex.o build/parse.o  build/ast.o -o parse

build/utest_parse.o:
	c++ -Wall -c utest_parse.cpp -o build/utest_parse.o

clean:
	rm -rf build/*.o parse utest_lex utest_parse
