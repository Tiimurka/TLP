CC=c++
CFLAGS=-Wall -c
LDFLAGS=-Wall
TARGET=parse utest_lex utest_parse
BUILD_PARSE=build/main.o build/lex.o build/parse.o  build/ast.o build/symtab.o
BUILD_UTEST_LEX=build/utest_lex.o  build/lex.o
BUILD_UTEST_PARSE=build/utest_parse.o build/lex.o build/parse.o  build/ast.o

all: $(TARGET)

parse: $(BUILD_PARSE)
	$(CC) $(LDFLAGS) $(BUILD_PARSE) -o parse

utest_lex: $(BUILD_UTEST_LEX)
	$(CC) $(LDFLAGS) $(BUILD_UTEST_LEX) -o utest_lex

utest_parse: $(BUILD_UTEST_PARSE)
	$(CC) $(LDFLAGS) $(BUILD_UTEST_PARSE) -o utest_parse

build/%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf build/*.o $(TARGET)
