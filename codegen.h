#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h" 
#include "symtab.h" 
#include <fstream>

int codegen(struct ast *tree, class symtab *t);

#endif