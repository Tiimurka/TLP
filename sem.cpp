#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h" 

enum SEM {SEM_DEFAULT = 0};

//const char* const SEM_NAMES[] = {"SEM_DEFAULT"};

bool sem_analyzer(struct ast *tree){
	return sem_rec(tree, SEM_DEFAULT);
}

bool sem_rec(){
	if(tree->type== AST_TYPE_ASSIGN)
}

#endif