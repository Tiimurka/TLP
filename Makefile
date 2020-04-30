CC=c++
CFLAGS=-Wall -c
LDFLAGS=-Wall
TARGET=parse utest_lex utest_parse utest_symtab utest_sem
BUILD_PARSE=build/main.o build/lex.o build/parse.o  build/ast.o build/symtab.o build/sem.o
BUILD_UTEST_LEX=build/utest_lex.o  build/lex.o
BUILD_UTEST_PARSE=build/utest_parse.o build/lex.o build/parse.o  build/ast.o
BUILD_UTEST_SYMTAB=build/utest_symtab.o build/lex.o build/parse.o  build/ast.o build/symtab.o
BUILD_UTEST_SEM=build/utest_sem.o build/lex.o build/parse.o  build/ast.o build/symtab.o build/sem.o

all: $(TARGET)

parse: $(BUILD_PARSE)
	$(CC) $(LDFLAGS) $(BUILD_PARSE) -o parse

utest_lex: $(BUILD_UTEST_LEX)
	$(CC) $(LDFLAGS) $(BUILD_UTEST_LEX) -o utest_lex

utest_parse: $(BUILD_UTEST_PARSE)
	$(CC) $(LDFLAGS) $(BUILD_UTEST_PARSE) -o utest_parse

utest_symtab: $(BUILD_UTEST_SYMTAB)
	$(CC) $(LDFLAGS) $(BUILD_UTEST_SYMTAB) -o utest_symtab

utest_sem: $(BUILD_UTEST_SEM)
	$(CC) $(LDFLAGS) $(BUILD_UTEST_SEM) -o utest_sem

build/%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf build/*.o $(TARGET)
