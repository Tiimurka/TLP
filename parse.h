#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include "lex.h"
#include "ast.h"

struct ast *parse_test(const char *filename);
int parse_prog(Token *token, struct ast *tree);
int parse_pre(Token *token, struct ast *tree);
int parse_pre_part(Token *token, struct ast *tree);
int parse_var(Token *token, struct ast *tree);
int parse_ids(Token *token, struct ast *tree);
int parse_var_decl(Token *token, struct ast *tree);
int parse_decl_array(Token *token, struct ast *tree);
int parse_types(Token *token, struct ast *tree);
int parse_func_decl(Token *token, struct ast *tree);
int parse_main(Token *token, struct ast *tree);
int parse_m_cont(Token *token, struct ast *tree);
int parse_if(Token *token, struct ast *tree);
int parse_if_part(Token *token, struct ast *tree);
int parse_f_body(Token *token, struct ast *tree);
int parse_sent (Token *token, struct ast *tree);
int parse_f_cont (Token *token, struct ast *tree);
int parse_arg (Token *token, struct ast *tree);
int parse_num_fact(Token *token);
int parse_num_expr(Token *token);
int parse_assign(Token *token, struct ast *tree);

#endif