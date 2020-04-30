#include "sem.h"

class symtab *tab;

//unsigned int curr_level;
//unsigned int curr_sublevel;

int sem_id(struct ast *tree, unsigned int flag, unsigned int curr_level, unsigned int curr_sublevel){
	//std::cout << "Entering sem_id, "<< AST_NAMES[tree->type] <<"\n";
	struct tabnode* n;
	//int ret = 0;
	Token *t;
		if(tree->nodes[tree->nodes.size()-1].type == AST_TYPE_NODE){
		struct ast *ptr1 = tree->nodes[tree->nodes.size()-1].ptr_n;
			if(ptr1->type == AST_ARR){
				float check = std::atof(ptr1->nodes[1].ptr_t->Lexeme.c_str());
				//std::cout << "getting "<<ptr1->nodes[0].ptr_t->Lexeme<<"\n";
				n = tab->get(ptr1->nodes[0].ptr_t->Lexeme);
				if(n == NULL){
					std::cout << 
					"Error: variable "  << ptr1->nodes[0].ptr_t->Lexeme << " isn't declared\n";
					return -1;
				}
				if(n->is_arr == false){
					std::cout << 
					"Error: variable"<< ptr1->nodes[0].ptr_t->Lexeme  << "isn't declared as an array\n";
					return -1;
				}
				if(check < n->arr_ub || check > n->arr_lb){
					std::cout << 
					"Error: index out of array borders (" << ptr1->nodes[1].ptr_t->Lexeme 
					<<", but should be from " << n->arr_ub << " to " << n->arr_lb <<")\n";
					return -1;
				}
				check -= (int)check;
				if(check != 0){
					std::cout << 
					"Error: array borders must be integer, but it is " << ptr1->nodes[1].ptr_t->Lexeme <<"\n";
					return -1;
				}
				t = ptr1->nodes[0].ptr_t;
			}
		}else{
			//std::cout << "getting "<<tree->nodes[0].ptr_t->Lexeme<<"\n";
			n = tab->get(tree->nodes[0].ptr_t->Lexeme);
			//if(tree->nodes[0].ptr_t == NULL)
				//std::cout << "azazaz\n";
			t = tree->nodes[0].ptr_t;
			if(n == NULL){
				std::cout << 
				"Error: variable "<< tree->nodes[0].ptr_t->row << " " << tree->nodes[0].ptr_t->Lexeme << " isn't declared\n";
				return -1;
			}
			if(n->is_arr == true){
				std::cout << "Line " << 
				"Error: variable " << tree->nodes[0].ptr_t->Lexeme << " declared as an array, but there is no index\n";
				return -1;
			}
		}
		if(n == NULL)
			std::cout << "azazaz\n";
		if(n->level > curr_level){
			std::cout << t->Lexeme <<
			": Error: wrong declaration level (" << curr_level <<", but should be "<< n->level <<")\n";
			return -1;
		}
		if(n->level == curr_level && n->sublevel != curr_sublevel){
			std::cout << t->Lexeme <<  
			": Error: wrong declaration sublevel(" << curr_sublevel <<", but should be "<< n->sublevel <<")\n";
			return -1;
		}
		return n->type;
}

bool isBinop(struct ast *node){
	if(node == NULL)
		return false;
	//std::cout << "entering isBinop, type = "<< AST_NAMES[node->type] <<"\n";
	unsigned int type = node->type;
	if(type == AST_BINOP_PLUS || type == AST_BINOP_MINUS || type ==  AST_BINOP_SLASH ||
	type ==  AST_BINOP_ASTER || type ==  AST_BINOP_DIV || type ==  AST_BINOP_MOD ||
	type == AST_LOGIC_ISEQUAL || type ==  AST_LOGIC_ISLESS || type ==  AST_LOGIC_ISMORE || 
	type ==  AST_LOGIC_OR || type ==  AST_LOGIC_AND)
		return true;
	return false;
}

int TypeHandler(Token *t){
	//std::cout << "entering type handler with " << t->Lexeme << "\n"; 
	if(t->TokenClass == TC_NUM){
		float check = std::atof(t->Lexeme.c_str());
		check -= (int)check;
			if(check != 0)
				return TC_T_REAL;
			else
				return TC_T_INT;
	}else if(t->TokenClass == TC_STRING)
		return TC_T_STRING;
	else{
		//std::cout << "ekekek1\n";
		return -1;
	}
}

int RetHandler(int ret1, int ret2){
	if(ret1 == TC_T_STRING && ret2 == TC_T_STRING)
		return TC_T_STRING;
	if(ret1 == TC_T_REAL || ret2 == TC_T_REAL)
		return TC_T_REAL;
	if(ret1 == TC_T_INT && ret2 == TC_T_INT)
		return TC_T_INT;
	//std::cout << "ekekek2\n";
	return -1;
}

bool is_b_str(int ret1, int ret2){
	if(ret1 == TC_T_STRING)
		if(ret2 != TC_T_STRING)
			return false;
	if(ret2 == TC_T_STRING)
		if(ret1 != TC_T_STRING)
			return false;
	return true;
}

bool checkTypeForBinop(unsigned int type, int ret1, int ret2){
	//int ret = 0;
	if(ret1 == -1 || ret2 == -1)
		return false;
	switch (type){
		case AST_BINOP_PLUS:
			if(!is_b_str(ret1, ret2))
				return false;
			return true;
		case AST_BINOP_MINUS:
		case AST_BINOP_ASTER:
		case AST_BINOP_SLASH:
		case AST_LOGIC_ISEQUAL:
		case AST_LOGIC_ISLESS:
		case AST_LOGIC_ISMORE:
		case AST_LOGIC_OR:
		case AST_LOGIC_AND:
			if(ret1 == TC_T_STRING || ret2 == TC_T_STRING)
				return false;
			return true;
		case AST_BINOP_MOD:
		case AST_BINOP_DIV:
			if(ret1 != TC_T_INT || ret2 != TC_T_INT)
				return false;
			return true;
		
	}
	return false;
}

int sem_expr(struct ast *tree, unsigned int curr_level, unsigned int curr_sublevel){
	//std::cout << "Entering sem_expr, type = "<< AST_NAMES[tree->type] <<"\n";
	int ret1, ret2;
	if(isBinop(tree->nodes[0].ptr_n))
		ret1 = sem_expr(tree->nodes[0].ptr_n, curr_level, curr_sublevel);
	else if(tree->nodes[0].ptr_n != NULL && tree->nodes[0].ptr_n->type == AST_TYPE_ID)
		ret1 = sem_id(tree->nodes[0].ptr_n, SEM_DEFAULT, curr_level, curr_sublevel);
	else
		ret1 = TypeHandler(tree->nodes[0].ptr_t);
	
	if(isBinop(tree->nodes[1].ptr_n))
		ret2 = sem_expr(tree->nodes[1].ptr_n, curr_level, curr_sublevel);
	else if(tree->nodes[1].ptr_n != NULL && tree->nodes[1].ptr_n->type == AST_TYPE_ID)
		ret2 = sem_id(tree->nodes[1].ptr_n, SEM_DEFAULT, curr_level, curr_sublevel);
	else
		ret2 = TypeHandler(tree->nodes[1].ptr_t);
	bool check = checkTypeForBinop(tree->type, ret1, ret2);
	if(!check){
		std::cout << "Error unsuppoted operand type(s) for " << AST_NAMES[tree->type]
		<< "\n " << TC_NAMES[ret1] << " and " << TC_NAMES[ret2] << "\n";
		return -1;
	}
	return RetHandler(ret1, ret2);	
}

bool AssignCheck(unsigned int ret1, unsigned int ret2){
	if(ret1 == TC_T_STRING && ret2 != TC_T_STRING)
		return false;
	else if(ret1 == TC_T_INT && ret2 != TC_T_INT)
		return false;
	else if(ret1 == TC_T_REAL && ret2 == TC_T_STRING)
		return false;
	return true;
	
}

int sem_rec(struct ast *tree, unsigned int flag, unsigned int curr_level, unsigned int curr_sublevel){
	//struct tabnode* n;
	//std::cout << "Entering sem_rec, type = "<< AST_NAMES[tree->type]<< "\n";
	unsigned int flg = flag;
	int ret = 0;
	unsigned int i = 0;
	int ret2;
	int type;
	if(tree->type == AST_DECL_ARRAY){
		
		for(unsigned int j = 0; j < 2; j++){
			float check = std::atof(tree->nodes[j].ptr_t->Lexeme.c_str());
			//std::cout << check << "\n";
			check -= (int)check;
			if(check != 0){
				std::cout << "Line " << tree->nodes[j].ptr_t->row << 
				": Error: array borders must be integer, but it is " << tree->nodes[j].ptr_t->Lexeme <<"\n";
				return -1;
			}
		}
	}else if(isBinop(tree)){
		flg = SEM_EXPR;
	}else if(tree->type == AST_ASSIGN){
		struct ast *assign = tree->nodes[0].ptr_n;
		type = sem_id(assign, flg, curr_level, curr_sublevel);
		if(type < 0)
			return -1;
		if(assign->nodes[1].type == AST_TYPE_NODE && tree->nodes[1].ptr_n->type == AST_TYPE_ID){
			ret2 = sem_id(assign, flg, curr_level, curr_sublevel);
		}else if(isBinop(tree->nodes[1].ptr_n) == false){
			ret2 = TypeHandler(tree->nodes[1].ptr_n->nodes[0].ptr_t);
		}
		flg = SEM_EXPR;
		i++;
	}
	unsigned int cs = 0;
	if(flg == SEM_DEFAULT){
		for(; i < tree->nodes.size(); i++){
			if(tree->nodes[i].type == AST_TYPE_NODE){
				if(tree->nodes[i].ptr_n->type == AST_FUNC_DECL){
					ret += sem_rec(tree->nodes[i].ptr_n, flg, 1, cs);
					++cs;
				}else
					ret += sem_rec(tree->nodes[i].ptr_n, flg, curr_level, curr_sublevel);
			}
		}
	}else if(flg == SEM_EXPR){
		//for(; i < tree->nodes.size(); i++){
		if(isBinop(tree->nodes[1].ptr_n))
			ret2 = sem_expr(tree->nodes[1].ptr_n, curr_level, curr_sublevel);
		if(ret2 < 0)
			return -1;
		//}
		if(tree->type == AST_ASSIGN){
			if(!AssignCheck(type, ret2)){
				std::cout << "Error: " << TC_NAMES[ret2] << " cannot be assigned to " <<
				TC_NAMES[type] << "\n";
				return -1;
			}
		}
	}
	return ret;
}


struct ast * sem_analyzer(struct ast *tree, class symtab *t){
	tab = t;
	int ret = sem_rec(tree, SEM_DEFAULT, 0, 0);
	if(ret >= 0){
		std::cout << "Your semantycs is awesome!\n";
		return tree;
	}
	else{
		std::cout << "Your semantycs isn't awesome. Try again\n";
		return NULL;
	}
}
