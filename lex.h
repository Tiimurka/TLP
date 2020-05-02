#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>

enum TC {TC_UNKNOWN = 0, TC_VAR, TC_T_INT, TC_T_REAL, TC_T_STRING, TC_T_ARRAY, TC_FUNC, TC_DO, TC_FOR, TC_WHILE, TC_DOWNTO, TC_BEGIN, TC_UNTIL, TC_REPEAT, TC_IF, 
TC_THEN, TC_ELSE, TC_OR, TC_AND, TC_OF, TC_END_FUNC, TC_END_PROG, TC_COMMA, TC_COLON, TC_SEMICOLON, TC_LPAREN, TC_RPAREN, TC_DOUBLEDOT, 
TC_TO, TC_LBRACKET, TC_RBRACKET, TC_ISEQUAL, TC_ISLESS, TC_ISMORE, TC_ISNOTEQUAL, TC_ASSIGN, TC_DIV, TC_MOD, TC_PLUS, 
TC_MINUS, TC_ASTER, TC_SLASH, TC_USES, TC_ID, TC_NUM, TC_STRING, TC_COMMENT, TC_ENDLINE, TC_FINAL,
TC_ID_WM, TC_ARR_WM, TC_ARR, CAST_REAL};


#define MODE_OTHER 0
#define MODE_SYM1 1
#define MODE_SYM2 2
#define MODE_STRING 3
#define MODE_COMMENT 4

//const unsigned int AMOUNT = 42;

const char* const TC_NAMES[] = {"TC_UNKNOWN", "TC_VAR", "TC_T_INT", "TC_T_REAL", "TC_T_STRING", "TC_T_ARRAY", "TC_FUNC", "TC_DO", "TC_FOR", "TC_WHILE", "TC_DOWNTO", "TC_BEGIN", "TC_UNTIL", "TC_REPEAT", "TC_IF", 
"TC_THEN", "TC_ELSE","TC_OR", "TC_AND", "TC_OF", "TC_END_FUNC", "TC_END_PROG", "TC_COMMA", "TC_COLON", "TC_SEMICOLON", "TC_LPAREN", "TC_RPAREN", "TC_DOUBLEDOT", 
"TC_TO", "TC_LBRACKET", "TC_RBRACKET", "TC_ISEQUAL", "TC_ISLESS", "TC_ISMORE", "TC_ISNOTEQUAL", "TC_ASSIGN", "TC_DIV", "TC_MOD", "TC_PLUS", 
"TC_MINUS", "TC_ASTER","TC_SLASH", "TC_USES", "TC_ID", "TC_NUM", "TC_STRING", "TC_COMMENT", "TC_ENDLINE", "TC_FINAL",
"TC_ID_WM", "TC_ARR_WM", "TC_ARR", "CAST_REAL"};

struct Token {
	int TokenClass;
	unsigned int row;
	std::string Lexeme;
};

unsigned int get_curr_row();
unsigned int get_curr_col();
char ModSetter(char *pos, char mode);
bool IdCheck(char* lex);
bool NumCheck(char* lex);
int LexAdd(char* Lexeme);
bool Lex(const char* filename);
char ModSetter(char *pos, char mode);
bool LexInit(std::ifstream *fin);
void move_lex_pos(int value);
char* get_current_buf();
Token *GetNextToken(std::ifstream *fin);

#endif