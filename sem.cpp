#include "sem.h"

class symtab *tab;

int sem_rec(struct ast *tree, unsigned int flag){
	if(tree->type == AST_DECL_ARRAY){
		
		for(unsigned int i = 0; i < 2; i++){
			float check = std::atof(tree->nodes[i].ptr_t->Lexeme.c_str());
			std::cout << check << "\n";
			check -= (int)check;
			if(check != 0){
				std::cout << "Line " << tree->nodes[i].ptr_t->row << 
				": Error: array borders must be integer, but it is " << tree->nodes[i].ptr_t->Lexeme <<"\n";
				return -1;
			}
		}
	}//else if(tree->type == AST_TYPE_ID){
		//if()
	//}
	int ret = 0;
	for(unsigned int i = 0; i < tree->nodes.size(); i++){
		if(tree->nodes[i].type == AST_TYPE_NODE)
			ret += sem_rec(tree->nodes[i].ptr_n, SEM_DEFAULT);
	}
	return ret;
}


struct ast * sem_analyzer(struct ast *tree, class symtab *t){
	tab = t;
	int ret = sem_rec(tree, SEM_DEFAULT);
	if(ret == 0){
		std::cout << "Your semantycs is awesome!\n";
		return tree;
	}
	else{
		std::cout << "Your semantycs isn't awesome. Try again\n";
		return tree;
	}
}
