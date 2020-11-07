#ifndef SEM_H
#define SEM_H

#include "ast.h" 
#include "symtab.h"

enum SEM {SEM_DEFAULT = 0, SEM_EXPR};

struct ast * sem_analyzer(struct ast *tree, class symtab *t);
int sem_expr(struct ast *tree, unsigned int curr_level, unsigned int curr_sublevel);
int sem_id(struct ast *tree, unsigned int flag, unsigned int curr_level, unsigned int curr_sublevel);

#endif