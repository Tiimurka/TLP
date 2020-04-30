#ifndef SEM_H
#define SEM_H

#include "ast.h" 
#include "symtab.h"

enum SEM {SEM_DEFAULT = 0, SEM_EXPR};

//const char* const SEM_NAMES[] = {"SEM_DEFAULT"};

//bool isBinop(struct ast *node);
//int TypeHandler(Token *t);
//int RetHandler(int ret1, int ret2);
//bool is_b_str(int ret1, int ret2);
//bool checkTypeForBinop(unsigned int type, int ret1, int ret2);
//bool AssignCheck(unsigned int ret1, unsigned int ret2);

struct ast * sem_analyzer(struct ast *tree, class symtab *t);
int sem_expr(struct ast *tree, unsigned int curr_level, unsigned int curr_sublevel);
int sem_id(struct ast *tree, unsigned int flag, unsigned int curr_level, unsigned int curr_sublevel);

#endif