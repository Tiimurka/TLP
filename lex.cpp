#include "lex.h"

static unsigned int current_col;
static unsigned int current_row;
static char buf[150];
static char str1[150];

bool MinusHandler(char *pos){
	if(*pos != '-')
		return false;
	if(*pos == '-' && current_col == 0)
		return false;
	unsigned int b = 1;
	while(*(pos-b) == ' ')
		b++;
	unsigned int f = 1;
	while(*(pos+f) == ' ')
		f++;
	if((*(pos+f) >= 'A' && *(pos+f) <= 'Z') || (*(pos+f) >= 'a' && *(pos+f) <= 'z') || *(pos+f) == '-')
		return true;
	if(*pos == '-' && (((*(pos-b) >= 'A' && *(pos-b) <= 'Z') || (*(pos-b) >= 'a' && *(pos-b) <= 'z') || *(pos-b) == '_' || *(pos-b) == ']'
		|| (*(pos-b) >= '0' && *(pos-b) <= '9') || *(pos-b) == '=' ) || *(pos-b) == ')'))
		return true;
	return false;
}

char ModSetter(char *pos, char mode){
	char ret = MODE_OTHER;
	if (mode == MODE_OTHER){
		if ((*pos == '/' && *(pos+1) == '/') || (*pos == '{'))
			ret = MODE_COMMENT;
		else if ((*pos == ':' && *(pos+1) == '=') || (*pos == '<' && *(pos+1) == '>') || (*pos == '.' && *(pos+1) == '.'))
			ret = MODE_SYM2;
		else if(*pos == '(' || *pos == ')' || *pos == '[' || *pos == ']' || *pos == ':' || *pos == ';' || *pos == ',' 
		|| *pos == '+' || (MinusHandler(pos) == true) || *pos == '*' || *pos == '/' || *pos == '<' || *pos == '>' || *pos == '=' )
			ret = MODE_SYM1;
		else if (*pos == '\'')
			ret = MODE_STRING;
		else
			ret = MODE_OTHER;
	}
	return ret;
}

bool IdCheck(char* lex){
	char* check = lex;
	bool isFirst = true;
	while(*check != '\0'){
		if((*check >= 'A' && *check <= 'Z') || (*check >= 'a' && *check <= 'z') || *check == '_' 
		|| (isFirst == false && (*check >= '0' && *check <= '9'))){
			++check;
			if(isFirst)
				isFirst = false;
		}else
			return false;	
	}
	return true;
}

bool NumCheck(char* lex){
	char* check = lex;
	if(*check == '-'){
		++check;
	}
	char first = *check;
	switch(first){
		case '$': //16x
			++check;
			while(*check != '\0'){
				if((*check >= '0' && *check <= '9') || (*check >= 'A' && *check <= 'F'))
					++check;
				else
					return false;
			}
			break;
		case '%': //2x
			++check;
			while(*check != '\0'){
				if(*check == '0' || *check == '1')
						++check;
				else
					return false;
			}
			break;
		case '&': //8x
			++check;
			while(*check != '\0'){
				if(*check >= '0' && *check <= '7')
					++check;
				else
					return false;
			}
			break;
		default: //10x
			bool isDotAllowed = true;
			while(*check != '\0'){
				if((*check >= '0' && *check <= '9') || (*check == '.' && isDotAllowed)){
					if(*check == '.')
						isDotAllowed = false;
					++check;
				}
				else
					return false;
			}
	}
	return true;
}

int LexAdd(char* Lexeme){
	int check;
	if(*Lexeme == '\0')
		check = TC_FINAL;
	else if (std::strcmp(Lexeme, "var") == 0)
		check = TC_VAR;
	else if(std::strcmp(Lexeme, "integer") == 0)
		check = TC_T_INT;
	else if(std::strcmp(Lexeme, "real") == 0)
		check = TC_T_REAL;
	else if(std::strcmp(Lexeme, "string") == 0)
		check = TC_T_STRING;
	else if(std::strcmp(Lexeme, "array") == 0)
		check = TC_T_ARRAY;
	else if(std::strcmp(Lexeme, "function") == 0)
		check = TC_FUNC;
	else if(std::strcmp(Lexeme, "do") == 0)
		check = TC_DO;
	else if(std::strcmp(Lexeme, "for") == 0)
		check = TC_FOR;
	else if(std::strcmp(Lexeme, "do") == 0)
		check = TC_DO;
	else if(std::strcmp(Lexeme, "downto") == 0)
		check = TC_DOWNTO;
	else if(std::strcmp(Lexeme, "while") == 0)
		check = TC_WHILE;
	else if(std::strcmp(Lexeme, "begin") == 0)
		check = TC_BEGIN;
	else if(std::strcmp(Lexeme, "until") == 0)
		check = TC_UNTIL;
	else if(std::strcmp(Lexeme, "repeat") == 0)
		check = TC_REPEAT;
	else if(std::strcmp(Lexeme, "if") == 0)
		check = TC_IF;
	else if(std::strcmp(Lexeme, "then") == 0)
		check = TC_THEN;
	else if(std::strcmp(Lexeme, "else") == 0)
		check = TC_ELSE;
	else if(std::strcmp(Lexeme, "or") == 0)
		check = TC_OR;
	else if(std::strcmp(Lexeme, "of") == 0)
		check = TC_OF;
	else if(std::strcmp(Lexeme, "and") == 0)
		check = TC_AND;
	else if(std::strcmp(Lexeme, "end") == 0)
		check = TC_END_FUNC;
	else if(std::strcmp(Lexeme, "end.") == 0)
		check = TC_END_PROG;
	else if(std::strcmp(Lexeme, ",") == 0)
		check = TC_COMMA;
	else if(std::strcmp(Lexeme, ":") == 0)
		check = TC_COLON;
	else if(std::strcmp(Lexeme, ";") == 0)
		check = TC_SEMICOLON;
	else if(std::strcmp(Lexeme, "(") == 0)
		check = TC_LPAREN;
	else if(std::strcmp(Lexeme, ")") == 0)
		check = TC_RPAREN;
	else if(std::strcmp(Lexeme, "..") == 0)
		check = TC_DOUBLEDOT;
	else if(std::strcmp(Lexeme, "to") == 0)
		check = TC_TO;
	else if(std::strcmp(Lexeme, "[") == 0)
		check = TC_LBRACKET;
	else if(std::strcmp(Lexeme, "]") == 0)
		check = TC_RBRACKET;
	else if(std::strcmp(Lexeme, "=") == 0)
		check = TC_ISEQUAL;
	else if(std::strcmp(Lexeme, "<") == 0)
		check = TC_ISLESS;
	else if(std::strcmp(Lexeme, ">") == 0)
		check = TC_ISMORE;
	else if(std::strcmp(Lexeme, "<>") == 0)
		check = TC_ISNOTEQUAL;
	else if(std::strcmp(Lexeme, ":=") == 0)
		check = TC_ASSIGN;
	else if(std::strcmp(Lexeme, "div") == 0)
		check = TC_DIV;
	else if(std::strcmp(Lexeme, "mod") == 0)
		check = TC_MOD;
	else if(std::strcmp(Lexeme, "+") == 0)
		check = TC_PLUS;
	else if(std::strcmp(Lexeme, "-") == 0)
		check = TC_MINUS;
	else if(std::strcmp(Lexeme, "*") == 0)
		check = TC_ASTER;
	else if(std::strcmp(Lexeme, "/") == 0)
		check = TC_SLASH;
	else if(std::strcmp(Lexeme, "uses") == 0)
		check = TC_USES;
	else if(IdCheck(Lexeme))
		check = TC_ID;
	else if(NumCheck(Lexeme))
		check = TC_NUM;
	else{
		check = TC_UNKNOWN;
	}
	return check;
}

bool LexInit(std::ifstream *fin){
	current_col = 0;
	current_row = 1;
	if(!(fin->getline(buf, 150)))
				return false;
	return true;
}

void move_lex_pos(int value){
	current_col += value;
}

Token *GetNextToken(std::ifstream *fin){
	Token *t = new Token;
	t->TokenClass = TC_UNKNOWN;
	char sym;
	int j;
	int mode = MODE_OTHER;
	bool isFirst = true;
	while (1){
		if(buf[current_col] == '\0'){
			unsigned int prev_col = current_col;
			current_col = 0;
			++current_row;
			if(!(fin->getline(buf, 150))){
				t->TokenClass = TC_FINAL;
				t->row = current_row-1;
				t->col = prev_col;
				t->Lexeme = "\0";
			}else{
				t->TokenClass = TC_ENDLINE;
				t->row = current_row-1;
				t->col = prev_col;
				t->Lexeme = "\0";
			}
			return t;
		}
			char* parse = buf+current_col;
			while(parse != NULL && *parse != '\0'){
				if(mode == MODE_OTHER)
					mode = ModSetter(parse, mode);
				if(mode == MODE_COMMENT && ((*parse == '/' && *(parse+1) == '/') || (*parse == '{'))){
					j = 0;
					sym = *parse;
					str1[j] = *parse;
					if(sym == '/'){
						++parse;
						++j;
						current_col++;
						str1[j] = *parse;
					}
					++j;
				}else if (mode == MODE_COMMENT){
					if((*(parse+1) == '\0' && sym == '/') || *parse == '}'){
						str1[j] = *parse;
						str1[j+1] = '\0';
						current_col++;
						t->TokenClass = TC_COMMENT;
						t->row = current_row;
						t->col = current_col;
						t->Lexeme = str1;
						return t;
					}else if (*(parse+1) == '\0'){
						//++j;
						str1[j] = ' ';
					}else{
						str1[j] = *parse;
						++j;
					}
				}else if ((*parse == ' ' || *parse == 13 || *parse == 9) && mode == MODE_OTHER){
					parse++;
					current_col++;
					continue;
				}else if(mode == MODE_SYM2) {
					str1[0] = *parse;
					parse++;
					current_col++;
					str1[1] = *parse;
					str1[2] = '\0';
					current_col++;
					t->TokenClass = LexAdd(str1);
					t->row = current_row;
					t->col = current_col;
					t->Lexeme = str1;
					return t;
				}else if(mode == MODE_SYM1){
					str1[0] = *parse;
					str1[1] = '\0';
					current_col++;
					t->TokenClass = LexAdd(str1);
					t->row = current_row;
					t->col = current_col;
					t->Lexeme = str1;
					return t;
				}else if(*parse == '\'' && mode == MODE_STRING && isFirst){
					j = 0;
					str1[j] = *parse;
					++j;
					isFirst = false;
				}else if(*parse == '\'' && mode == MODE_STRING && !isFirst){
					str1[j] = *parse;
					str1[j+1] = '\0';
					current_col++;
					t->TokenClass = TC_STRING;
					t->row = current_row;
					t->col = current_col;
					t->Lexeme = str1;
					return t;
				}else if(mode == MODE_STRING){
					str1[j] = *parse;
					if(*(parse+1) == '\0'){
						str1[j+1] = '\0';
						current_col++;
						t->TokenClass = TC_UNKNOWN;
						t->row = current_row;
						t->col = current_col;
						t->Lexeme = str1;
						return t;
					}
					++j;
				}else if(mode == MODE_OTHER){
					if(isFirst){
						j = 0;
						isFirst = false;
					}
					str1[j] = *parse;
					if((ModSetter(parse+1, MODE_OTHER) != MODE_OTHER) || *(parse+1) == '\0' || *(parse+1) == ' ' || *(parse+1) == 9 || *(parse+1) == 13){
						current_col++;
						str1[j+1] = '\0';
						t->TokenClass = LexAdd(str1);
						t->row = current_row;
						t->col = current_col;
						t->Lexeme = str1;
						return t;
					}
					++j;
				}
				parse++;
				current_col++;
			}

	}
}

char* get_current_buf(){
	char *p = buf+current_col;
	return p;
}

unsigned int get_curr_row(){
	return current_row;
}

unsigned int get_curr_col(){
	return current_col;
}

bool Lex(const char *filename){
	std::ifstream fin;
	fin.open(filename);
	Token *t;
	LexInit(&fin);
	while(1){
		t = GetNextToken(&fin);
		std::cout << t->row << " " << TC_NAMES[t->TokenClass] << " " << t->Lexeme << std::endl;
		if(t->TokenClass == TC_FINAL)
			break;
	}
	fin.close();
	return true;
}

