#ifndef SEM_H
#define SEM_H

#include "ast.h" 
#include "symtab.h"

enum SEM {SEM_DEFAULT = 0, SEM_EXPR};

//const char* const SEM_NAMES[] = {"SEM_DEFAULT"};

struct ast * sem_analyzer(struct ast *tree, class symtab *t);

#endif