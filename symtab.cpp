#include "symtab.h"

symtab::symtab(struct ast *tree){
			bool check = ast_handle(tree);
			if (check)
				std::cout << "handling complete!\n";
}

bool symtab::ast_handle(struct ast *tree){
	if (tree == NULL){
		std::cout << "Error: tree is not exist\n";
		return false;
	}
	struct ast *ptr = tree->nodes[0].ptr_n;
	if(ptr->type == AST_PRE){
		unsigned int func_count = 0;
		for(unsigned int i = 0; i < ptr->nodes.size(); i++){
			struct ast *node = ptr->nodes[i].ptr_n;
			if(node->type == AST_VAR){
				struct ast *ids = node->nodes[0].ptr_n;
				struct tabnode *ins1;
				if(ids->type == AST_VAR_IDS){
					for(unsigned int j = 0; j < ids->nodes.size(); j++){
						if(table.find(ids->nodes[j].ptr_t->Lexeme) == table.end()){
							ins1 = new struct tabnode;
							ins1->level = 0; ins1->sublevel = 0; 
							if(node->nodes[1].type == AST_TYPE_NODE && node->nodes[1].ptr_n->type == AST_DECL_ARRAY){
								ins1->is_arr = true;
								ins1->arr_ub = std::atof(node->nodes[1].ptr_n->nodes[0].ptr_t->Lexeme.c_str());
								ins1->arr_lb = std::atof(node->nodes[1].ptr_n->nodes[1].ptr_t->Lexeme.c_str());
								ins1->type = node->nodes[1].ptr_n->nodes[2].ptr_t->TokenClass;
							}else{
								ins1->is_arr = false;
								ins1->type = node->nodes[1].ptr_t->TokenClass;
							}
							//std::cout << "var case, ins1 = " << ins1[0] << " " << ins1[1] << " " << ids->nodes[j].ptr_t->Lexeme << "\n";
							table.insert(make_pair(ids->nodes[j].ptr_t->Lexeme, ins1)); 
						}else{
							std::cout << "Error:" << ids->nodes[j].ptr_t->Lexeme << " is already declared\n";
							return false;
						}
					}
				}else{
					std::cout << "Error: no ids in AST_VAR\n";
					return false;
				}
			}else if(node->type == AST_FUNC_DECL){
				if(node->nodes[1].ptr_n != NULL && node->nodes[1].ptr_n->type == AST_VAR){
					struct ast *decl1 = node->nodes[1].ptr_n;
					struct ast *decl2 = decl1->nodes[0].ptr_n;
					struct tabnode *ins2;
					for(unsigned int j = 0; j < decl2->nodes.size(); j++){
						if(table.find(decl2->nodes[j].ptr_t->Lexeme) == table.end()){
							ins2 = new struct tabnode;
							ins2->level = 1; ins2->sublevel = func_count;
							ins2->type = decl1->nodes[1].ptr_t->TokenClass;
							ins2->is_arr = false;
							table[decl2->nodes[j].ptr_t->Lexeme] = ins2;
						}else{
							std::cout << "Error:" << decl2->nodes[j].ptr_t->Lexeme << " is already declared\n";
							return false;
						}
					}
					++func_count;
				}
			}
		}
	}
	return true;
}

void symtab::show(){
	std::unordered_map<std::string, struct tabnode*>:: iterator itr;  
	for (itr = table.begin(); itr != table.end(); itr++) 
	{ 
		std::cout << itr->first << " " << itr->second->level << " " << itr->second->sublevel << " " << 
		TC_NAMES[itr->second->type]; 
		if(itr->second->is_arr){
			std::cout << " " << itr->second->arr_ub << " " << itr->second->arr_lb;
		}
		std::cout << "\n";
	}
}

struct tabnode* symtab::get(std::string key){
	if(table.find(key) == table.end())
		return NULL;
	return table.find(key)->second;
}