#ifndef SYMTAB_H
#define SYMTAB_H

#include <iostream> 
#include <unordered_map> 
#include "ast.h"

struct tabnode {
	unsigned int level;
	unsigned int sublevel;
	unsigned int type;
	bool is_arr;
	unsigned int arr_ub;
	unsigned int arr_lb;
};

class symtab
{
	private:
		std::unordered_map<std::string,struct tabnode*> table;
	public:
		symtab(struct ast *tree);
		bool ast_handle(struct ast *tree);
		void show();
		struct tabnode* get(std::string key);
};

#endif