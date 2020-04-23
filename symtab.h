#ifndef SYMTAB_H
#define SYMTAB_H

#include <iostream> 
#include <unordered_map> 
#include "ast.h"

class symtab
{
	private:
		std::unordered_map<std::string, unsigned int*> table;
	public:
		symtab(struct ast *tree);
		bool ast_handle(struct ast *tree);
		void show();
		unsigned int* get(std::string key);
};

#endif