#include "symtab.h"

/*class symtab
{
	private:
		std::unordered_map<std::string, unsigned int*> table;
	public:
		symtab(struct ast *tree){
			bool check = ast_handle(tree);
			if (check)
				std::cout << "handling complete!\n";
		}
		bool ast_handle(struct ast *tree){
			if (tree == NULL){
				std::cout << "Error: tree is not exist\n";
				return false;
			}
			struct ast *ptr = tree->nodes[0].ptr_n;
			if(ptr->type == AST_PRE){
				unsigned int func_count = 0;
				for(unsigned int i = 0; i < ptr->nodes.size(); i++){
					struct ast *node = ptr->nodes[0].ptr_n;
					if(node->type == AST_VAR){
						struct ast *ids = node->nodes[0].ptr_n;
						if(ids->type == AST_VAR_IDS){
							for(unsigned int j = 0; j < ids->nodes.size(); j++){
								unsigned int ins[2] = {0, 0};
								table[ids->nodes[j].ptr_t->Lexeme] = ins;
							}
						}else{
							std::cout << "Error: no ids in AST_VAR\n";
							return false;
						}
					}else if(node->type == AST_FUNC_DECL){
						struct ast *ids = node->nodes[0].ptr_n->nodes[0].ptr_n;
						for(unsigned int j = 0; j < ids->nodes.size(); j++){
								unsigned int ins[2] = {1, func_count};
								table[ids->nodes[j].ptr_t->Lexeme] = ins;
						}
						++func_count;
					}
				}
			}
			return true;
		}
		void show(){
			for (auto x : table) 
				std::cout << x.first << " " << x.second[0] << ", " << x.second[1] << "\n";
		}
};*/

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
				unsigned int *ins1;
				if(ids->type == AST_VAR_IDS){
					for(unsigned int j = 0; j < ids->nodes.size(); j++){
						ins1 = new unsigned int[2];
						ins1[0] = 0; ins1[1] = 0;
						//std::cout << "var case, ins1 = " << ins1[0] << " " << ins1[1] << " " << ids->nodes[j].ptr_t->Lexeme << "\n";
						table.insert(make_pair(ids->nodes[j].ptr_t->Lexeme, ins1)); 
						//table[ids->nodes[j].ptr_t->Lexeme] = ins1;
					}
				}else{
					std::cout << "Error: no ids in AST_VAR\n";
					return false;
				}
			}else if(node->type == AST_FUNC_DECL){
				struct ast *decl = node->nodes[0].ptr_n->nodes[0].ptr_n;
				unsigned int *ins2;
				for(unsigned int j = 0; j < decl->nodes.size(); j++){
					//struct ast *ids->nodes[j].ptr_n;
					ins2 = new unsigned int[2];
					ins2[0] = 1; ins2[1] = func_count;
					//std::cout << "func case, ins2 = " << ins2[0] << " " << ins2[1] << " " << decl->nodes[j].ptr_t->Lexeme << "\n";
					table[decl->nodes[j].ptr_t->Lexeme] = ins2;
				}
				++func_count;
			}
		}
	}
	return true;
}

void symtab::show(){
	/*for (auto x : table) 
		std::cout << x.first << " " << x.second[0] << ", " << x.second[1] << "\n";*/
	std::unordered_map<std::string, unsigned int*>:: iterator itr; 
	std::cout << "\nAll Elements : \n"; 
	for (itr = table.begin(); itr != table.end(); itr++) 
	{ 
		std::cout << itr->first << " " << itr->second[0] << " " << itr->second[1] << "\n"; 
	}
}