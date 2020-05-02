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
		//std::cout << T[i].row << " " << TC_NAMES[T[i].TokenClass] << " " << T[i].Lexeme << std::endl;
		std::cout << "Loc=<" << T[i].row<<":"<<T[i].col<<"> "
		<< TC_NAMES[T[i].TokenClass] << " '" << T[i].Lexeme << "'\n";
}

struct ast *parse_test(const char *filename, bool isdt){
	//std::cout << "I am entering parse_test!\n"; 
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
	//std::cout << t->row << " " << TC_NAMES[t->TokenClass] << " " << t->Lexeme << std::endl;
	struct ast *tree = ast_create(AST_PROG);
	result += parse_prog(t, tree);
	//std::cout << "result: " << result << "\n"; 
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
	//std::cout << "I am entering parse_prog, current token is " << token->Lexeme << "\n";
	int result = 0;
	Token *tmp_token;
	tmp_token = token;
	
	node_insert(tree, NULL, AST_PRE);
	result += parse_pre(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
	if(tree->nodes[tree->nodes.size()-1].ptr_n->nodes.size() == 0){
		//std::cout << "Popping back!\n";
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
	//std::cout << "I am entering parse_pre, current token is " << token->Lexeme << "\n"; 
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	//tmp_token = GNT();
	simskip(&tmp_token);
	//while(tmp_token->TokenClass == TC_ENDLINE)
		//tmp_token = GNT();
	if(tmp_token->TokenClass == TC_BEGIN){
		//std::cout << "zaz\n";
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
	//if(tmp_token->TokenClass != TC_BEGIN){
		//ret += parse_pre_part(tmp_token, tree);
		//if (ret != 0)
			//return -1;
	//}
	//else
	if(tmp_token->TokenClass == TC_BEGIN)
		move_lex_pos(-5);
	return ret;
}

int parse_pre_part(Token *token, struct ast *tree){
	//std::cout << "I am entering parse_pre_part, current token is " << token->Lexeme << "\n"; 
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
		//tmp_token = GNT();
		return ret;
	}else{
		std::cout << "Line " << tmp_token->row << ": missing semicolon, found " << tmp_token->Lexeme << "\n"; 
		return -1;
	}
}

int parse_var(Token *token, struct ast *tree){
	//std::cout << "I am entering parse_var, current token is " << token->Lexeme << "\n"; 
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	//tmp_token = GNT();
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
		//ret+=parse_var_decl(tmp_token);
		return -1;
	}
	return ret;
}

int parse_ids(Token *token, struct ast *tree){
	//std::cout << "I am entering parse_ids, current token is " << token->Lexeme << "\n";
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
	//std::cout << "I am entering parse_var_decl, current token is " << token->Lexeme << "\n"; 
	int ret = 0;
	Token *tmp_token;
	//tmp_token = token;
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

int parse_decl_array(Token *token, struct ast *tree){
	//std::cout << "I am entering parse_decl_array, current token is " << token->Lexeme << "\n"; 
	int ret = 0;
	Token *tmp_token = token;;
	//tmp_token = GNT();
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
						if(tmp_token->TokenClass == TC_OF){
							tmp_token = GNT();
							if(tmp_token == NULL)
								return -1;
							ret+=parse_types(tmp_token, tree);
							if (ret != 0)
								return -1;
						}else{
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

int parse_types(Token *token, struct ast *tree){
	//std::cout << "I am entering parse_types, current token is " << token->Lexeme << "\n"; 
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

int parse_func_decl(Token *token, struct ast *tree){
	//std::cout << "I am entering parse_func, current token is " << token->Lexeme << "\n";
	int ret = 0;
	Token *tmp_token;
	tmp_token = GNT();
	if(tmp_token == NULL)
		return -1;
	if(tmp_token->TokenClass == TC_ID){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_LPAREN){
			//!!!
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
						if(tmp_token->TokenClass == TC_BEGIN){
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
						}else{
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
}

int parse_main(Token *token, struct ast *tree){
	//std::cout << "I am entering parse_main, current token is " << token->Lexeme << "\n";
	//std::cout << current_row;
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
			//tmp_token = GNT();
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
	//std::cout << "I am entering parse_m_cont, current token is " << token->Lexeme << "\n";
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	//switch (token->TokenClass){
		
	//}
	//tmp_token = GNT();
	simskip(&tmp_token);
	//tmp_token = GNT();
	
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
	//std::cout << "I am entering parse_if, current token is " << token->Lexeme << "\n";
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
		//std::cout << "catch!\n";
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
			//node_insert(tree, NULL, AST_ELSEIF);
			ret += parse_if(tmp_token, tree);
			if (ret != 0)
				return -1;
			//tmp_token = GNT();
			//if(tmp_token == NULL)
				//return -1;
			//simskip(&tmp_token);
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
				//std::cout << "Starting 'else'!\n";
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				simskip(&tmp_token);
				ret += parse_f_body(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
				if (ret != 0)
					return -1;
				//std::cout << "'else' body parsing finished!\n";
				tmp_token = GNT();
				if(tmp_token == NULL)
					return -1;
				if(tmp_token->TokenClass == TC_END_FUNC){
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					if(tmp_token->TokenClass == TC_SEMICOLON){
						//tmp_token = GNT();
						//if(tmp_token == NULL)
							//return -1;
						//move_lex_pos(-1);
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
	//if(tmp_token->TokenClass == TC_BEGIN){
		
	//}
	return ret;
}

int parse_if_part(Token *token, struct ast *tree){
	//std::cout << "I am entering parse_if_part, current token is " << token->Lexeme << "\n";
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
			//std::cout << "ololololo\n";
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
				//tmp_token = GNT();
				//if(tmp_token == NULL)
					//return -1;
				/*if(tmp_token->TokenClass == TC_SEMICOLON){
					tmp_token = GNT();
					if(tmp_token == NULL)
						return -1;
					return ret;
				}else{
					std::cout << "Line " << tmp_token->row << ": missing semicolon, found " << tmp_token->Lexeme << "\n"; 
					return -1;
				}*/
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
	//std::cout << "I am entering parse_f_body, current token is " << token->Lexeme << "\n";
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	//switch (token->TokenClass){
		
	//}
	//tmp_token = GNT();
	simskip(&tmp_token);
	//tmp_token = GNT();
	
	if(tmp_token->TokenClass == TC_IF){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		node_insert(tree, NULL, AST_CHOICE);
		ret += parse_if(tmp_token, tree->nodes[tree->nodes.size()-1].ptr_n);
		if (ret != 0)
			return -1;
		tmp_token = GNT();
		//std::cout << "Current TC (after leving parse_if and GNT) is " << tmp_token->Lexeme << " on row " << tmp_token->row << "\n";
		if(tmp_token == NULL)
			return -1;
		simskip(&tmp_token);
		if(tmp_token->TokenClass == TC_UNTIL){
			//std::cout << "Current TC (in f_body, 'until' case) is " << tmp_token->Lexeme << " on row " << tmp_token->row << "\n";
			return ret;
		}else if(tmp_token->TokenClass == TC_END_FUNC){
			//std::cout << "Current TC (in f_body, 'end_func' case) is " << tmp_token->Lexeme << " on row " << tmp_token->row << "\n";
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
		//std::cout << "Current TC is " << tmp_token->Lexeme << " on row " << tmp_token->row << "\n";
		//simskip(&tmp_token);
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_SEMICOLON){
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			//std::cout << "Current TC is " << TC_NAMES[tmp_token->TokenClass] << " on row " << tmp_token->row << "\n";
			if(tmp_token == NULL)
				return -1;
			simskip(&tmp_token);
			if(tmp_token->TokenClass == TC_UNTIL){
				//tmp_token = GNT();
				//move_lex_pos(-5);
				//std::cout << "Current TC (in f_body, 'until' case) is " << tmp_token->Lexeme << " on row " << tmp_token->row << "\n";
				return ret;
			}else if(tmp_token->TokenClass == TC_END_FUNC){
				//std::cout << "Current TC (in f_body, 'end_func' case) is " << tmp_token->Lexeme << " on row " << tmp_token->row << "\n";
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

int parse_sent (Token *token, struct ast *tree){
	//std::cout << "I am entering parse_sent, current token is " << token->Lexeme << "\n";
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	//tmp_token = GNT();
	if(tmp_token->TokenClass == TC_ID){
		bool is_arr = false;
		struct Token *ptr = tmp_token;
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
				//move_lex_pos(-1);
				return ret;
			}else{
				std::cout << "Line " << tmp_token->row << ": wrong end of function\n";
				return -1;
			}
		}else if(tmp_token->TokenClass == TC_ASSIGN){
			//tmp_token = GNT();
			node_insert(tree, NULL, AST_ASSIGN);
			struct ast *ptr1 = tree->nodes[tree->nodes.size()-1].ptr_n;
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
	}else if(tmp_token->TokenClass == TC_REPEAT){
		//std::cout << "Starting RepUn!\n";
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
		//std::cout << "Current TC (in repun, before moving) is " << tmp_token->Lexeme << " on row " << tmp_token->row << "\n";
		move_lex_pos(-5);
		//std::cout << "test1\n"; 
		tmp_token = GNT();
		//std::cout << "test2\n"; 
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_UNTIL){
			//std::cout << "test3\n";
			char *test = get_current_buf();
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			
			//std::cout << "buf = " << test << "\n";
			ret+=parse_num_expr(tmp_token);
				if (ret != 0)
					return -1;
			node_insert(ptr, NULL, 0);
			expr_insert(ptr->nodes[ptr->nodes.size()-1].ptr_n, test);
			return ret;
		}else{
			std::cout << "Line " << tmp_token->row << ": Error: wrong ending of repeat-until construction, found "<< tmp_token->Lexeme << "\n";
		}
	}else if (tmp_token->TokenClass == TC_WHILE){
		//std::cout << "Starting 'while'!\n";
		node_insert(tree, NULL, AST_WHILE);
		struct ast *ptr = tree->nodes[tree->nodes.size()-1].ptr_n;
		char *test = get_current_buf();
		//std::cout << "buf = " << test << "\n";
		char sendbuf[50];
		int i = 0;
		for(;(*test != '\0') && (*(test+1) != 'd' && *(test+2) != 'o'); i++){
			sendbuf[i] = *test;
			++test;
		}
		sendbuf[i] = ';';
		sendbuf[i+1] = '\0';
		//std::cout << "sendbuf = " << test << "\n";
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
		
	}else if(tmp_token->TokenClass == TC_FOR){
		//std::cout << "Starting 'while'!\n";
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
				//tmp_token = GNT();
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
	}else{
		std::cout << "Line " << tmp_token->row <<
			": Error: missing id, found "
			<< TC_NAMES[tmp_token->TokenClass] << "\n";
	}
	return ret;
}

int parse_f_cont (Token *token, struct ast *tree){
	//std::cout << "I am entering parse_f_cont, current token is " << token->Lexeme << "\n";
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	//node_insert(tree, NULL, AST_ARG);
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
	//std::cout << "I am entering parse_arg, current token is " << token->Lexeme << "\n";
	int ret = 0;
	Token *tmp_token;
	tmp_token = token;
	char *test;
	switch(tmp_token->TokenClass){
		//case TC_STRING:
			//node_insert(tree, tmp_token, 9999);
			//return 0;
		case TC_ID:
		case TC_NUM:
		case TC_STRING:
		case TC_LPAREN:
			test = get_current_buf();
			char sendbuf[50];
			int i;
			for(i = 0; tmp_token->Lexeme[i] != '\0'; i++){
				sendbuf[i] = tmp_token->Lexeme[i];
			}
			for(; (*test != '\0' && *test != ',' && *test != ')'); i++){
				sendbuf[i] = *test;
				++test;
			}
			//sendbuf[i+1] = *test;
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
	//std::cout << "I am entering parse_assign, current token is " << token->Lexeme << "\n";
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
	/*if(tmp_token->TokenClass == TC_ID){
		tmp_token = GNT();
		if(tmp_token == NULL)
			return -1;
		if(tmp_token->TokenClass == TC_SEMICOLON){
			move_lex_pos(-1);
			node_insert(tree, tmp_token, 9999);
			return 0;
		}
	}else{*/
		//std::cout << "expr: " << test << "\n";
		ret += parse_num_expr(tmp_token);
		if (ret != 0)
			return -1;
		node_insert(tree, NULL, 0);
		expr_insert(tree->nodes[tree->nodes.size()-1].ptr_n, sendbuf);
		//if(tmp_token->TokenClass == TC_SEMICOLON)
			//move_lex_pos(-1);
	//}
	//std::cout << "Current TC in assign is " << TC_NAMES[tmp_token->TokenClass] << " on row " << tmp_token->row << "\n";
	
	return ret;
}

int parse_num_expr(Token *token){
	//std::cout << "I am entering parse_num_expr, current token is " << token->Lexeme << "\n";
	int ret = 0;
	Token *tmp_token = token;
	//int parscore = 0;
	//if(tmp_token->TokenClass == TC_LPAREN)
		//parscore++;
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
				/*case TC_LBRACKET:
					//!!!
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
						tmp_token = GNT();
						if(tmp_token == NULL)
							return -1;
						
					}*/
				case TC_SEMICOLON:
				case TC_COMMA:
					move_lex_pos(-1);
					return ret;
				case TC_THEN:
					//std::cout << "Current token is " << tmp_token->Lexeme << ", moving back\n";
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
	//std::cout << "I am entering parse_num_fact, current token is " << token->Lexeme << "\n";
	int ret = 0;
	Token *tmp_token = token;
	//tmp_token = GNT();
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
			//int l = tmp_token->Lexeme.length();
			//l*=-1;
			//move_lex_pos(l);
			tmp_token = GNT();
			if(tmp_token == NULL)
				return -1;
			//std::cout << "AZAZAZ, current token is " << tmp_token->Lexeme << "\n";
			if(tmp_token->TokenClass == TC_RPAREN){
				//tmp_token = GNT();
				//move_lex_pos(-1);
				return 0;
			}/*else if(tmp_token->TokenClass == TC_SEMICOLON){
				move_lex_pos(-1);
				return 0;
			}*/else{
				std::cout << "Line " << tmp_token->row <<
			": Error: wrong ending, found "
			<< TC_NAMES[tmp_token->TokenClass] << "\n";
			return -1;
			}
	}
	return ret;
}