#ifndef AST_H
#define AST_H

#include "lex.h"

#define AST_TYPE_TOKEN 0
#define AST_TYPE_NODE 1
#define AST_TYPE_UNKNOWN 9999
#define TC_ARR 99999

enum AST_ID {AST_PROG = 0, AST_PRE, AST_VAR, AST_VAR_IDS, AST_VAR_DECL, AST_DECL_ARRAY, AST_FUNC_DECL, AST_ARR,
 AST_MAIN, AST_FUNC, AST_F_BODY, AST_F_CONT, AST_ARG, AST_ASSIGN, AST_CHOICE, AST_IF, AST_ELSE, AST_IF_COND,
 AST_TYPE_NUM, AST_TYPE_ID, AST_BINOP_PLUS, AST_BINOP_MINUS, AST_BINOP_SLASH, AST_BINOP_ASTER, AST_BINOP_DIV, AST_BINOP_MOD,
 AST_LOGIC_ISEQUAL, AST_LOGIC_ISLESS, AST_LOGIC_ISMORE, AST_LOGIC_OR, AST_LOGIC_AND,
 AST_REPUN, AST_WHILE, AST_FOR};

const char* const AST_NAMES[] = {"AST_PROG", "AST_PRE", "AST_VAR", "AST_VAR_IDS", "AST_VAR_DECL", "AST_DECL_ARRAY", "AST_FUNC_DECL", "AST_ARR",
	"AST_MAIN", "AST_FUNC", "AST_F_BODY", "AST_F_CONT", "AST_ARG", "AST_ASSIGN", "AST_CHOICE", "AST_IF", "AST_ELSE", "AST_IF_COND",
	"AST_TYPE_NUM", "AST_TYPE_ID", "AST_BINOP_PLUS", "AST_BINOP_MINUS", "AST_BINOP_SLASH", "AST_BINOP_ASTER", "AST_BINOP_DIV", "AST_BINOP_MOD",
	"AST_LOGIC_ISEQUAL", "AST_LOGIC_ISLESS", "AST_LOGIC_ISMORE", "AST_LOGIC_OR", "AST_LOGIC_AND",
	"AST_REPUN", "AST_WHILE", "AST_FOR"};

struct ast{
	unsigned int type;
	std::vector<struct ast_node> nodes;
};

struct ast_node{
	unsigned int type;
	struct Token *ptr_t;
	struct ast *ptr_n;
};

struct ast *ast_create(unsigned int type);
struct ast_node *node_create(struct Token *t, struct ast *n);
void node_insert(struct ast *tree, struct Token *t, unsigned int type);

int get_prior(int type, int par_pr);
unsigned int get_type(std::string Lexeme);
void expr_lex (char *buf, std::vector<Token> *Tokens);
void expr_insert(struct ast *tree, char *expr);
void ex_ins_rec(struct ast *tree, std::vector<Token> vec);

bool show_node (struct ast *node, unsigned int tablevel);
void ast_print (struct ast *tree, unsigned int tablevel);
#endif