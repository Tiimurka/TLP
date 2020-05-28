#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h" 
#include "symtab.h" 
#include <fstream>

int codegen_init(class symtab *t);
int clear_asm_line(unsigned int part);
std::string get_asm_line(unsigned int part, unsigned int linenum);
int gen_var(struct ast *tree);
int gen_decl(struct ast *tree);
int gen_pre(struct ast *tree);
std::string gen_pn(struct ast *tree, bool pn_num);
int gen_func(struct ast *tree);
std::string gen_assign(struct ast *tree);
int gen_if(struct ast *tree, unsigned int curr_bnum, unsigned int cb, bool is_print_next);
int gen_choice(struct ast *tree);
int gen_loop(struct ast *tree);
int gen_main(struct ast *tree);
std::string codegen(std::string filename, struct ast *tree, class symtab *t, bool is_print);

#endif