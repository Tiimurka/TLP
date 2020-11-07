#include "parse.h"

std::ifstream fin;

bool is_dump_tokens;

std::vector <struct Token> GT;

unsigned int prev_col;

bool is_corr_t(struct Token *t){
	if(GT.size() == 1)
		return true;
	if(get_curr_col() == prev_col)
		return false;
	return true;
}

struct Token *GNT (){
	Token *t;
	t = GetNextToken(&fin);
	if(is_dump_tokens){
		if(is_corr_t(t))
			GT.push_back(*t);
	}
	prev_col = get_curr_col();
	if(t->TokenClass == TC_UNKNOWN){
		std::cout << "Line " << t->row << ": lexical error, found " << t->Lexeme <<"\n"; 
		return NULL;
	}
	return t;
}

void DT (std::vector<struct Token> T){
	for(unsigned int i = 0; i < T.size(); i++)
		std::cout << "Loc=<" << T[i].row<<":"<<T[i].col<<"> "
		<< TC_NAMES[T[i].TokenClass] << " '" << T[i].Lexeme << "'\n";
}

struct ast *parse_test(const char *filename, bool isdt){
	fin.open(filename);
	is_dump_tokens = isdt;
	LexInit(&fin);
	Token *t;
	int result = 0;
	t = GNT();
	if(t == NULL)
		return NULL;
	while(t->TokenClass == TC_ENDLINE)
		t = GNT();
	struct ast *tree = ast_create(AST_PROG);
	result += parse_prog(t, tree);
	fin.close();
	if(is_dump_tokens)
		DT(GT);
	if(result == 0){
		std::cout << "Your program is awesome!\n";
		return tree;
	}
	else{
		std::cout << "Your program isn't awesome. (" << result << ") Try again.\n";
		return NULL;
	}
}

void simskip(Token **token){
	Token *tmp_token  = *token;
	while(tmp_token != NULL && (tmp_token->TokenClass == TC_ENDLINE || tmp_token->TokenClass == TC_COMMENT))
		tmp_token = GNT();
	*token = tmp_token;
}

int parse_prog(Token *token, struct ast *tree){
	int result = 0;
	Token *tmp_token;
	tmp_token = token;
	
	node_insert(tree, NULL, AST_PRE);
	result += parse_pre(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
	if(tree->nodes[tree->nodes.size()-1].ptr_n->nodes.size() == 0){
		tree->nodes.pop_back();
	}
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	node_insert(tree, NULL, AST_MAIN);
	result += parse_main(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
	return result;
}

int parse_pre(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	simskip(&tmp_token);
	if(tmp_token->TokenClass == TC_BEGIN){
		move_lex_pos(-5);
		return ret;
	}
	while (tmp_token->TokenClass != TC_BEGIN){
		ret += parse_pre_part(tmp_token, tree);
		if (ret != 0)
			return -1;
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		simskip(&tmp_token);
	}
	if(tmp_token->TokenClass == TC_BEGIN)
		move_lex_pos(-5);
	return ret;
}

int parse_pre_part(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	if(tmp_token->TokenClass == TC_VAR){
		node_insert(tree, NULL, AST_VAR);
		ret+=parse_var(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
		if (ret != 0)
			return -1;
	}else if (tmp_token->TokenClass == TC_FUNC){
		node_insert(tree, NULL, AST_FUNC_DECL);
		ret+=parse_func_decl(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
		if (ret != 0)
			return -1;
	}else{
		std::cout << "Line " << tmp_token->row << ": unknown pre_part, found " << tmp_token->Lexeme << "\n"; 
		return -1;
	}
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	if(tmp_token->TokenClass == TC_SEMICOLON){
		return ret;
	}else{
		std::cout << "Line " << tmp_token->row << ": missing semicolon, found " << tmp_token->Lexeme << "\n"; 
		return -1;
	}
}

int parse_var(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	node_insert(tree, NULL, AST_VAR_IDS);
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	simskip(&tmp_token);
	ret+=parse_ids(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
	if (ret != 0)
		return -1;
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	if(tmp_token->TokenClass == TC_COLON){
		ret+=parse_var_decl(tmp_token, tree);
		if (ret != 0)
			return -1;
	}else{
		std::cout << "Line " << tmp_token->row << ": missing colon, found " << tmp_token->Lexeme << "\n";
		return -1;
	}
	return ret;
}

int parse_ids(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	
	if(tmp_token->TokenClass == TC_ID){
		node_insert(tree, tmp_token, 9999);
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_COMMA){
			
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			ret += parse_ids(tmp_token, tree);
			if (ret != 0)
				return -1;
		}else if(tmp_token->TokenClass == TC_COLON){
			move_lex_pos(-1);
			return ret;
		}else{
			std::cout << "Line " << tmp_token->row << ": wrong declaration of ids, found " << tmp_token->Lexeme << "\n";
			return -1;
		}
	}
	return ret;
}

int parse_var_decl(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	if(tmp_token->TokenClass == TC_T_ARRAY){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		node_insert(tree, NULL, AST_DECL_ARRAY);
		ret += parse_decl_array(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
		if (ret != 0)
			return -1;
	}else {
		ret += parse_types(tmp_token, tree);
		if (ret != 0)
			return -1;
	}
	return ret;
}

int check_decl_array(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token = token;
	if(tmp_token->TokenClass == TC_LBRACKET){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_NUM){
			node_insert(tree, tmp_token, 9999);
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass == TC_DOUBLEDOT){
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass == TC_NUM){
					node_insert(tree, tmp_token, 9999);
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					if(tmp_token->TokenClass == TC_RBRACKET){
						tmp_token = GNT();
						if(tmp_token == NULL)
							return -1;
						if(tmp_token->TokenClass != TC_OF){
							std::cout << "Line " << tmp_token->row << ": missing TC_OF, found " << tmp_token->Lexeme << "\n";
							return -1;
						}
					}else{
						std::cout << "Line " << tmp_token->row << ": missing TC_RBRACKET, found " << tmp_token->Lexeme << "\n";
						return -1;
					}
				}else{
					std::cout << "Line " << tmp_token->row << ": missing TC_NUM, found " << tmp_token->Lexeme << "\n";
					return -1;
				}
			}else{
				std::cout << "Line " << tmp_token->row << ": missing TC_DOUBLEDOT, found " << tmp_token->Lexeme << "\n";
				return -1;
			}
		}else{
			std::cout << "Line " << tmp_token->row << ": missing TC_NUM, found " << tmp_token->Lexeme << "\n";
			return -1;
		}
	}else{
		std::cout << "Line " << tmp_token->row << ": missing TC_LBRACKET, found " << tmp_token->Lexeme << "\n";
		return -1;
	}
	return ret;
}

int parse_decl_array(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token = token;
	check_decl_array(tmp_token, tree);
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	ret+=parse_types(tmp_token, tree);
	if (ret != 0)
		return -1;
	return ret;
}

int parse_types(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	switch(tmp_token->TokenClass){
		case TC_T_INT:
		case TC_T_REAL:
		case TC_T_STRING:
			node_insert(tree, tmp_token, 9999);
			return 0;
		default:
			std::cout << "Line " << tmp_token->row << ": Error: declaration type should be, TC_T_INT, TC_T_REAL or TC_T_STRING, but this is "
			<< TC_NAMES[tmp_token->TokenClass] << "\n";
			return -1;
	}
	return ret;
}

int check_func_decl(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	if(tmp_token->TokenClass == TC_ID){
		node_insert(tree, tmp_token, 9999);
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_LPAREN){
			tmp_token = GNT();
			if(tmp_token->TokenClass != TC_RPAREN){
				move_lex_pos(tmp_token->Lexeme.size() * (-1));
				node_insert(tree, NULL, AST_VAR);
				ret+=parse_var(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
				if (ret != 0)
					return -1;
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
			}
			if(tmp_token->TokenClass == TC_RPAREN){
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass == TC_COLON){
					ret+=parse_var_decl(tmp_token, tree);
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					if(tmp_token->TokenClass == TC_SEMICOLON){
						tmp_token = GNT();
						if(tmp_token == NULL)
							return -1;
						simskip(&tmp_token);
						if(tmp_token->TokenClass != TC_BEGIN){
							std::cout << "Line " << tmp_token->row << ": missing beginning of the function, found " << tmp_token->Lexeme << "\n";
							return -1;
						}
					}else{
						std::cout << "Line " << tmp_token->row << ": missing semicolon, found " << tmp_token->Lexeme << "\n";
						return -1;
					}
				}else{
					std::cout << "Line " << tmp_token->row << ": missing colon, found " << tmp_token->Lexeme << "\n";
					return -1;
				}
			}else{
				std::cout << "Line " << tmp_token->row << ": missing TC_RPAREN, found " << tmp_token->Lexeme << "\n";
				return -1;
			}
		}else{
			std::cout << "Line " << tmp_token->row << ": missing TC_LPAREN, found " << tmp_token->Lexeme << "\n";
			return -1;
		}
	}else{
		std::cout << "Line " << tmp_token->row << ": missing function name, found " << tmp_token->Lexeme << "\n";
		return -1;
	}
	return ret;
}

int parse_func_decl(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token = token;
	check_func_decl(tmp_token, tree);
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	node_insert(tree, NULL, AST_F_BODY);
	ret+=parse_f_body(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
	if (ret != 0)
		return -1;
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	if(tmp_token->TokenClass == TC_END_FUNC){
		return ret;
	}else{
		std::cout << "Line " << tmp_token->row << ": missing function ending, found " << tmp_token->Lexeme << "\n";
		return -1;
	}
	return ret;
}

int parse_main(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	simskip(&tmp_token);
	switch (tmp_token->TokenClass){
		case TC_BEGIN:
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			simskip(&tmp_token);
			ret += parse_m_cont(tmp_token, tree);
			if (ret != 0)
				return -1;
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass == TC_END_PROG){
				break;
			}else{
				std::cout << "Line " << tmp_token->row << ": missing TC_END_PROG, found " << tmp_token->Lexeme << "\n";
				return -1;
			}
			break;
		default:
			std::cout << "Line " << tmp_token->row << ": wrong program beginning, found " << tmp_token->Lexeme <<  "\n"; 
			return -1;
	}
	return ret;
}

int parse_m_cont(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	simskip(&tmp_token);
	if(tmp_token->TokenClass == TC_IF){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		node_insert(tree, NULL, AST_CHOICE);
		ret += parse_if(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
		if (ret != 0)
			return -1;
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_END_PROG){
				move_lex_pos(-4);
				return ret;
		}else{
			ret += parse_m_cont(tmp_token, tree);
			if (ret != 0)
				return -1;
		}
	}else{
		
		if(tmp_token->TokenClass == TC_END_PROG){
				move_lex_pos(-4);
				return ret;
		}
		
		ret += parse_sent(tmp_token, tree);
		if (ret != 0)
			return -1;
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_SEMICOLON){
			tmp_token = GNT();
			simskip(&tmp_token);
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass == TC_END_PROG){
				move_lex_pos(-4);
				return ret;
			}else{
				ret += parse_m_cont(tmp_token, tree);
				if (ret != 0)
					return -1;
			}
		}else{
			std::cout << "Line " << tmp_token->row << ": missing semicolon, found " << TC_NAMES[tmp_token->TokenClass] << "\n"; 
			return -1;
		}
		return ret;
	}
	return ret;
}

int parse_if(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	node_insert(tree, NULL, AST_IF);
	ret += parse_if_part(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
	if (ret != 0)
		return -1;
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	if(tmp_token->TokenClass == TC_END_FUNC){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		simskip(&tmp_token);
		if(tmp_token->TokenClass == TC_SEMICOLON){
			return ret;
		}
	}
	simskip(&tmp_token);
	if(tmp_token->TokenClass == TC_ELSE){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		simskip(&tmp_token);
		if(tmp_token->TokenClass == TC_IF){
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			ret += parse_if(tmp_token, tree);
			if (ret != 0)
				return -1;
		}else{
			node_insert(tree, NULL, AST_ELSE);
			if(tmp_token->TokenClass == TC_ID){
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass == TC_ASSIGN){
					ret += parse_assign(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
					if (ret != 0)
						return -1;
				}else{
					std::cout << "Line " << tmp_token->row << ": only assign operations allowed, but found " << tmp_token->Lexeme << "\n"; 
					return -1;
				}
			}else if (tmp_token->TokenClass == TC_BEGIN){
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				simskip(&tmp_token);
				ret += parse_f_body(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
				if (ret != 0)
					return -1;
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass == TC_END_FUNC){
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					if(tmp_token->TokenClass == TC_SEMICOLON){
						return ret;
					}else{
						std::cout << "Line " << tmp_token->row << ": missing semicolon, found " << tmp_token->Lexeme << "\n"; 
						return -1;
					}
				}else{
					std::cout << "Line " << tmp_token->row << ": missing 'else' end, found " << tmp_token->Lexeme << "\n"; 
					return -1;
				}
			}else{
				std::cout << "Line " << tmp_token->row << ": missing TC_BEGIN, found " << tmp_token->Lexeme << "\n"; 
				return -1;
			}
		}
		
	}
	return ret;
}

int parse_if_part(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	char *test;
	test = get_current_buf();
	char sendbuf[50];
	int i;
	for(i = 0; tmp_token->Lexeme[i] != '\0'; i++){
		sendbuf[i] = tmp_token->Lexeme[i];
	}
	for(;(*test != '\0') && (*(test+1) != 't' && *(test+2) != 'h' && *(test+3) != 'e' && *(test+4) != 'n'); i++){
		sendbuf[i] = *test;
		++test;
	}
	sendbuf[i] = ';';
	sendbuf[i+1] = '\0';
	ret += parse_num_expr(tmp_token);
	if (ret != 0)
		return -1;
	node_insert(tree, NULL, AST_IF_COND);
	expr_insert(tree->nodes[tree->nodes.size()-1].ptr_n, sendbuf);
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	simskip(&tmp_token);
	if(tmp_token->TokenClass == TC_THEN){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		simskip(&tmp_token);
		if(tmp_token->TokenClass == TC_ID){
			bool is_arr = false;
				struct Token *idptr = tmp_token;
				struct Token *ind;
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass == TC_LBRACKET){
					is_arr = true;
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					if(tmp_token->TokenClass != TC_ID && tmp_token->TokenClass != TC_NUM){
						std::cout << "Line " << tmp_token->row << ": wrong index, found "<< tmp_token->Lexeme << "\n";
						return -1;
					}
					ind = tmp_token;
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					if(tmp_token->TokenClass != TC_RBRACKET){
						std::cout << "Line " << tmp_token->row << ": missing TC_RBRACKET, found "<< tmp_token->Lexeme << "\n";
						return -1;
					}
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
				}
				if(tmp_token->TokenClass == TC_ASSIGN){
					node_insert(tree, NULL, AST_ASSIGN);
					struct ast *ptr1 = tree->nodes[tree->nodes.size()-1].ptr_n;
					node_insert(ptr1, NULL, AST_TYPE_ID);
					if(is_arr){
						struct ast *ptr2 = ptr1->nodes[ptr1->nodes.size()-1].ptr_n;
						node_insert(ptr2, NULL, AST_ARR);
						struct ast *ptr3 = ptr2->nodes[ptr2->nodes.size()-1].ptr_n;
						node_insert(ptr3, idptr, 9999);
						node_insert(ptr3, ind, 9999);
					}else
						node_insert(ptr1->nodes[ptr1->nodes.size()-1].ptr_n, idptr, 9999);
				
				
					ret += parse_assign(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
					if (ret != 0)
						return -1;
				}else{
					std::cout << "Line " << tmp_token->row << ": only assign operations allowed, but found " << tmp_token->Lexeme << "\n"; 
					return -1;
				}
		}else if (tmp_token->TokenClass == TC_BEGIN){
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			simskip(&tmp_token);
			node_insert(tree, NULL, AST_F_BODY);
			ret += parse_f_body(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
			if (ret != 0)
				return -1;
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass == TC_END_FUNC){
				move_lex_pos(-3);
				return ret;
			}else{
				std::cout << "Line " << tmp_token->row << ": missing 'if' end, found " << tmp_token->Lexeme << "\n"; 
				return -1;
			}
		}else{
			std::cout << "Line " << tmp_token->row << ": missing TC_BEGIN, found " << tmp_token->Lexeme << "\n"; 
			return -1;
		}
	}
	return ret;
}

int parse_f_body(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	simskip(&tmp_token);
	if(tmp_token->TokenClass == TC_IF){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		node_insert(tree, NULL, AST_CHOICE);
		ret += parse_if(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
		if (ret != 0)
			return -1;
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		simskip(&tmp_token);
		if(tmp_token->TokenClass == TC_UNTIL){
			return ret;
		}else if(tmp_token->TokenClass == TC_END_FUNC){
			move_lex_pos(-3);
			return ret;
		}else{
			ret += parse_f_body(tmp_token, tree);
			if (ret != 0)
				return -1;
		}
	}else{
	
		ret += parse_sent(tmp_token, tree);
		if (ret != 0)
			return -1;
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_SEMICOLON){
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token == NULL)
				return -1;
			simskip(&tmp_token);
			if(tmp_token->TokenClass == TC_UNTIL){
				return ret;
			}else if(tmp_token->TokenClass == TC_END_FUNC){
				move_lex_pos(-3);
				return ret;
			}else{
				ret += parse_f_body(tmp_token, tree);
				if (ret != 0)
					return -1;
			}
		}else{
			std::cout << "Line " << tmp_token->row << ": missing semicolon, found " << TC_NAMES[tmp_token->TokenClass] << "\n"; 
			return -1;
		}
		return ret;
	}
	return ret;
}

int parse_repun(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		node_insert(tree, NULL, AST_REPUN);
		struct ast *ptr = tree->nodes[tree->nodes.size()-1].ptr_n;
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		node_insert(ptr, NULL, AST_F_BODY);
		ret+=parse_f_body(tmp_token, ptr->nodes[ptr->nodes.size()-1].ptr_n);
		
		if (ret != 0)
			return -1;
		move_lex_pos(-5);
		tmp_token = GNT(); 
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_UNTIL){
			char *test = get_current_buf();
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			ret+=parse_num_expr(tmp_token);
				if (ret != 0)
					return -1;
			node_insert(ptr, NULL, 0);
			expr_insert(ptr->nodes[ptr->nodes.size()-1].ptr_n, test);
			return ret;
		}else{
			std::cout << "Line " << tmp_token->row << ": Error: wrong ending of repeat-until construction, found "<< tmp_token->Lexeme << "\n";
		}
		return ret;
}

int parse_while(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	node_insert(tree, NULL, AST_WHILE);
		struct ast *ptr = tree->nodes[tree->nodes.size()-1].ptr_n;
		char *test = get_current_buf();
		char sendbuf[50];
		int i = 0;
		for(;(*test != '\0') && (*(test+1) != 'd' && *(test+2) != 'o'); i++){
			sendbuf[i] = *test;
			++test;
		}
		sendbuf[i] = ';';
		sendbuf[i+1] = '\0';
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		ret+=parse_num_expr(tmp_token);
		if (ret != 0)
			return -1;
		node_insert(ptr, NULL, 0);
		expr_insert(ptr->nodes[ptr->nodes.size()-1].ptr_n, sendbuf);
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_DO){
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			simskip(&tmp_token);
			if(tmp_token->TokenClass == TC_BEGIN){
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				node_insert(ptr, NULL, AST_F_BODY);
				ret+=parse_f_body(tmp_token, ptr->nodes[ptr->nodes.size()-1].ptr_n);
				if (ret != 0)
					return -1;
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass == TC_END_FUNC){
					return ret;
				}
			}else{
				std::cout << "Line " << tmp_token->row <<
				": Error: missing 'begin', found "
				<< TC_NAMES[tmp_token->TokenClass] << "\n";
				return -1;
			}
		}else{
			std::cout << "Line " << tmp_token->row <<
			": Error: missing 'do', found "
			<< TC_NAMES[tmp_token->TokenClass] << "\n";
			return -1;
		}
		return ret;
}

int parse_for(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	node_insert(tree, NULL, AST_FOR);
		struct ast *ptr = tree->nodes[tree->nodes.size()-1].ptr_n;
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_ID){
			struct Token *idptr = tmp_token;
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass == TC_LBRACKET){
				std::cout << "Line " << tmp_token->row << "Error : arrays cannot be used as counters\n";
				return -1;
			}
			if(tmp_token->TokenClass == TC_ASSIGN){
				node_insert(ptr, NULL, AST_ASSIGN);
				node_insert(ptr->nodes[ptr->nodes.size()-1].ptr_n, NULL, AST_TYPE_ID);
				struct ast *fp = ptr->nodes[ptr->nodes.size()-1].ptr_n;
				node_insert(fp->nodes[fp->nodes.size()-1].ptr_n, idptr, 9999);
				ret += parse_assign(tmp_token, ptr->nodes[ptr->nodes.size()-1].ptr_n);
				if (ret != 0)
					return -1;
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass == TC_TO || tmp_token->TokenClass == TC_DOWNTO){
					node_insert(ptr, tmp_token, 9999);
					ret += parse_assign(tmp_token, ptr);
					if (ret != 0)
						return -1;
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					if(tmp_token->TokenClass == TC_DO){
						tmp_token = GNT();
						if(tmp_token == NULL)
						return -1;
						simskip(&tmp_token);
						if(tmp_token->TokenClass == TC_BEGIN){
							tmp_token = GNT();
							if(tmp_token == NULL)
								return -1;
							simskip(&tmp_token);
							node_insert(ptr, NULL, AST_F_BODY);
							ret+=parse_f_body(tmp_token, ptr->nodes[ptr->nodes.size()-1].ptr_n);
							if (ret != 0)
								return -1;
							tmp_token = GNT();
							if(tmp_token == NULL)
								return -1;
							if(tmp_token->TokenClass == TC_END_FUNC){
								return ret;
							}
						}else{
						std::cout << "Line " << tmp_token->row <<
						": Error: missing 'begin', found "
						<< TC_NAMES[tmp_token->TokenClass] << "\n";
						return -1;
						}
					}else{
						std::cout << "Line " << tmp_token->row << ": missing TC_DO, found "<< tmp_token->Lexeme << "\n";
						return -1;
					}
				}else{
					std::cout << "Line " << tmp_token->row << ": missing TC_TO or TC_DOWNTO, found "<< tmp_token->Lexeme << "\n";
					return -1;
				}
			}else{
				std::cout << "Line " << tmp_token->row << ": wrong beginning of function or assignment, found "<< tmp_token->Lexeme << "\n";
				return -1;
			}
		}else{
			std::cout << "Line " << tmp_token->row << ": missing TC_ID after 'for', found "<< tmp_token->Lexeme << "\n";
			return -1;
		}
		return ret;
}

int parse_op(Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token = token;
	bool is_arr = false;
		struct Token *ptr = tmp_token;
		unsigned int row = tmp_token->row;
		struct Token *ind;
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_LBRACKET){
			is_arr = true;
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass != TC_ID && tmp_token->TokenClass != TC_NUM){
				std::cout << "Line " << tmp_token->row << ": wrong index, found "<< tmp_token->Lexeme << "\n";
				return -1;
			}
			ind = tmp_token;
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass != TC_RBRACKET){
				std::cout << "Line " << tmp_token->row << ": missing TC_RBRACKET, found "<< tmp_token->Lexeme << "\n";
				return -1;
			}
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
		}
		if(tmp_token->TokenClass == TC_LPAREN){
			if(is_arr){
				std::cout << "Line " << tmp_token->row << "array element cannot be name of function\n";
				return -1;
			}
			node_insert(tree, NULL, AST_FUNC);
			node_insert(tree->nodes[tree->nodes.size()-1].ptr_n, ptr, 9999);
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			ret += parse_f_cont(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
			if (ret != 0)
				return -1;
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass == TC_RPAREN){
				return ret;
			}else{
				std::cout << "Line " << tmp_token->row << ": wrong end of function\n";
				return -1;
			}
		}else if(tmp_token->TokenClass == TC_ASSIGN){
			node_insert(tree, NULL, AST_ASSIGN);
			struct ast *ptr1 = tree->nodes[tree->nodes.size()-1].ptr_n;
			ptr1->row = row;
			node_insert(ptr1, NULL, AST_TYPE_ID);
			if(is_arr){
				struct ast *ptr2 = ptr1->nodes[ptr1->nodes.size()-1].ptr_n;
				node_insert(ptr2, NULL, AST_ARR);
				struct ast *ptr3 = ptr2->nodes[ptr2->nodes.size()-1].ptr_n;
				node_insert(ptr3, ptr, 9999);
				node_insert(ptr3, ind, 9999);
			}else
				node_insert(ptr1->nodes[ptr1->nodes.size()-1].ptr_n, ptr, 9999);
			ret += parse_assign(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
			if (ret != 0)
				return -1;
		}else{
			std::cout << "Line " << tmp_token->row << ": wrong beginning of function or assignment, found "<< tmp_token->Lexeme << "\n";
			return -1;
		}
		return ret;
}

int parse_sent (Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	if(tmp_token->TokenClass == TC_ID){
		parse_op(tmp_token, tree);
	}else if(tmp_token->TokenClass == TC_REPEAT){
		parse_repun(tmp_token, tree);
	}else if (tmp_token->TokenClass == TC_WHILE){
		parse_while(tmp_token, tree);
	}else if(tmp_token->TokenClass == TC_FOR){
		parse_for(tmp_token, tree);
		
		
	}else{
		std::cout << "Line " << tmp_token->row <<
			": Error: missing id, found "
			<< TC_NAMES[tmp_token->TokenClass] << "\n";
	}
	return ret;
}

int parse_f_cont (Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	if (tmp_token->TokenClass == TC_RPAREN){
		move_lex_pos(-1);
		return 0;
	}
	ret += parse_arg(tmp_token, tree);
	if (ret != 0)
		return -1;
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	if(tmp_token->TokenClass == TC_COMMA){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		ret += parse_f_cont(tmp_token, tree);
		if (ret != 0)
			return -1;
	}else if (tmp_token->TokenClass == TC_RPAREN){
		move_lex_pos(-1);
		return 0;
	}else{
		std::cout << "Line " << tmp_token->row << ": Error: missing comma, found "<< TC_NAMES[tmp_token->TokenClass] << "\n";
		return -1;
	}
	return ret;
}

int parse_arg (Token *token, struct ast *tree){
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	char *test;
	switch(tmp_token->TokenClass){
		case TC_ID:
		case TC_NUM:
		case TC_STRING:
		case TC_LPAREN:
			test = get_current_buf();
			char sendbuf[50];
			int i;
			int parencounter;
			parencounter = 1;
			for(i = 0; tmp_token->Lexeme[i] != '\0'; i++){
				sendbuf[i] = tmp_token->Lexeme[i];
			}
			for(; (*test != '\0' && *test != ',' && parencounter != 0); i++){
				if(*test == ')')
					--parencounter;
				if(*test == ')')
					++parencounter;
				if(parencounter != 0){
					sendbuf[i] = *test;
					++test;
				}
			}
			sendbuf[i] = '\0';
			ret += parse_num_expr(tmp_token);
			if (ret != 0)
				return -1;
			node_insert(tree, NULL, 0);
			expr_insert(tree->nodes[tree->nodes.size()-1].ptr_n, sendbuf);
			return ret;
		default:
			std::cout << "Line " << tmp_token->row << ": Error: function argument should be, TC_ID, TC_STRING or TC_NUM this is " 
			<< TC_NAMES[tmp_token->TokenClass] << "\n";
			return -1;
	}
	return ret;
}

bool is_do(char *pos){
	if(*(pos+1) == 'd' && *(pos+2) == 'o')
		return false;
	return true;
}

int parse_assign(Token *token, struct ast *tree){
	int ret = 0;
	char *test = get_current_buf();
	char sendbuf[50];
	int i = 0;
	for(;(*test != '\0') && is_do(test); i++){
		sendbuf[i] = *test;
		++test;
	}
	sendbuf[i] = ';';
	sendbuf[i+1] = '\0';
	Token *tmp_token = GNT();
	if(tmp_token == NULL)
			return -1;
		ret += parse_num_expr(tmp_token);
		if (ret != 0)
			return -1;
		node_insert(tree, NULL, 0);
		expr_insert(tree->nodes[tree->nodes.size()-1].ptr_n, sendbuf);
	return ret;
}

int parse_num_expr(Token *token){
	int ret = 0;
	Token *tmp_token = token;
	switch (tmp_token->TokenClass)
	{
		case TC_LPAREN:
		case TC_NUM:
		case TC_ID:
		case TC_STRING:
		case TC_MINUS:
			ret += parse_num_fact(tmp_token);
			if (ret != 0)
				return -1;
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			switch(tmp_token->TokenClass){
				case TC_RPAREN:
					move_lex_pos(-1);
					return ret;
				case TC_PLUS:
				case TC_MINUS:
				case TC_ASTER:
				case TC_SLASH:
				case TC_DIV:
				case TC_MOD:
				case TC_ISEQUAL:
				case TC_ISNOTEQUAL:
				case TC_ISLESS:
				case TC_ISMORE:
				case TC_OR:
				case TC_AND:
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					ret += parse_num_expr(tmp_token);
					if (ret != 0)
						return -1;
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
				case TC_SEMICOLON:
				case TC_COMMA:
					move_lex_pos(-1);
					return ret;
				case TC_THEN:
					move_lex_pos(-4);
					return ret;
				case TC_DO:
				case TC_TO:
					move_lex_pos(-2);
					return ret;
				case TC_DOWNTO:
					move_lex_pos(-6);
					return ret;
				default:
					std::cout << "Line " << tmp_token->row <<
					": Error: wrong symbol, found "
					<< TC_NAMES[tmp_token->TokenClass] << ", maybe you missed comma, semicolon or begginning of 'if'/'while' construction?" << "\n";
					
					return -1;
			}
		default:
			std::cout << "Line " << tmp_token->row <<
			": Error: wrong beginning of arithmetic operation, found "
			<< tmp_token->Lexeme << "\n";
			return -1;
		
	}
	return ret;
}

int parse_num_fact(Token *token){
	int ret = 0;
	Token *tmp_token = token;
	if(tmp_token->TokenClass == TC_MINUS){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
	}
	switch(tmp_token->TokenClass){
		case TC_ID:
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass == TC_LBRACKET){
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass != TC_ID && tmp_token->TokenClass != TC_NUM){
					std::cout << "Line " << tmp_token->row << ": wrong index, found "<< tmp_token->Lexeme << "\n";
					return -1;
				}
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass != TC_RBRACKET){
					std::cout << "Line " << tmp_token->row << ": missing TC_RBRACKET, found "<< tmp_token->Lexeme << "\n";
					return -1;
				}else{
					return 0;
					break;
				}
			}else{
				move_lex_pos(tmp_token->Lexeme.size() * (-1));
				return 0;
				break;
			}
			break;
		case TC_NUM:
		case TC_STRING:
			return 0;
			break;
		case TC_LPAREN:
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			ret += parse_num_expr(tmp_token);
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			if(tmp_token->TokenClass == TC_RPAREN){
				return 0;
			}else{
				std::cout << "Line " << tmp_token->row <<
			": Error: wrong ending, found "
			<< TC_NAMES[tmp_token->TokenClass] << "\n";
			return -1;
			}
	}
	return ret;
}