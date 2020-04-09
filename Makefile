all:
	c++ -Wall utest_lex.cpp lex.cpp -o utest_lex
	c++ -Wall main.cpp lex.cpp parse.cpp ast.cpp  -o parse 
	c++ -Wall utest_parse.cpp lex.cpp parse.cpp ast.cpp -o utest_parse