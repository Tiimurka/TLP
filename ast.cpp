#include "ast.h"

struct ast *ast_create(unsigned int type){
	struct ast *a = new ast;
	a->type = type;
	return a;
}

struct ast_node *node_create(struct Token *t, struct ast *n){ 
	struct ast_node *node = new ast_node;
	if(t != NULL){
		node->type = AST_TYPE_TOKEN;
		node->ptr_t = new Token;
		node->ptr_t->TokenClass = t->TokenClass;
		node->ptr_t->Lexeme = t->Lexeme;
		node->ptr_t->row = t->row;
		node->ptr_n = NULL;
	}else{
		node->type = AST_TYPE_NODE;
		node->ptr_t = NULL;
		node->ptr_n = n;
	}
	return node;
}

void node_insert(struct ast *tree, struct Token *t, unsigned int type){
	struct ast_node *node;
	if(t == NULL){
		struct ast *n = ast_create(type);
		node = node_create(NULL, n);
	}else
		node = node_create(t, NULL);
	tree->nodes.push_back(*node);
}

int get_prior(int type, int par_pr){
	int p = par_pr*10;
	switch (type){
		case TC_PLUS:
		case TC_MINUS:
		case TC_ISEQUAL:
		case TC_ISNOTEQUAL:
		case TC_ISMORE:
		case TC_ISLESS:
			return p+1;
		case TC_ASTER:
		case TC_SLASH:
		case TC_DIV:
		case TC_MOD:
		case TC_OR:
		case TC_AND:
			return p+2;
	}
	return 9999;
}

unsigned int get_type(std::string Lexeme){
	char str[5];
	unsigned int i;
	for(i = 0; i < Lexeme.size(); i++)
		str[i] = Lexeme[i];
	str[i] = '\0';
	if (std::strcmp(str, "+") == 0)
		return AST_BINOP_PLUS;
	else if (std::strcmp(str, "-") == 0)
		return AST_BINOP_MINUS;
	else if (std::strcmp(str, "*") == 0)
		return AST_BINOP_ASTER;
	else if (std::strcmp(str, "/") == 0)
		return AST_BINOP_SLASH;
	else if (std::strcmp(str, "div") == 0)
		return AST_BINOP_DIV;
	else if (std::strcmp(str, "mod") == 0)
		return AST_BINOP_MOD;
	else if (std::strcmp(str, "=") == 0)
		return AST_LOGIC_ISEQUAL;
	else if (std::strcmp(str, "<>") == 0)
		return AST_LOGIC_ISNOTEQUAL;
	else if (std::strcmp(str, "<") == 0)
		return AST_LOGIC_ISLESS;
	else if (std::strcmp(str, ">") == 0)
		return AST_LOGIC_ISMORE;
	else if (std::strcmp(str, "or") == 0)
		return AST_LOGIC_OR;
	else if (std::strcmp(str, "and") == 0)
		return AST_LOGIC_AND;
	else
		return 9999;
}

bool MinusHandler(char *pos, std::vector<Token> *Tokens){
	if(*pos != '-')
		return false;
	if(*pos == '-' && Tokens->size() == 0)
		return true;
	unsigned int b = 1;
	while(*(pos-b) == ' ')
		b++;
	unsigned int f = 1;
	while(*(pos+f) == ' ')
		f++;
	if(*(pos-b) == ')' || *(pos+f) == '(')
		return false;
	if(*pos == '-' && ((*(pos-b) >= 'A' && *(pos-b) <= 'Z') || (*(pos-b) >= 'a' && *(pos-b) <= 'z') || *(pos-b) == '_' || *(pos-b) == ']'
		|| (*(pos-b) >= '0' && *(pos-b) <= '9')))
		return false;
	else return true;
}

 bool is_corr_sym (char *sym,std::vector<Token> *Tokens){
	if(((*sym >= 'A' && *sym <= 'Z') || (*sym >= 'a' && *sym <= 'z') || *sym == '_') ||
	(*sym == '%' || *sym == '&') || (*sym >= '0' && *sym <= '9') || MinusHandler(sym, Tokens)){
			return true;}
	return false;
 }

bool is_corr_ending(char *sym){
	while(*sym != '+' && *sym != '-' && *sym != '/' && *sym != '*' && *sym != '='
	&& *sym != '<' && *sym != '>' && *sym != ' ' 
	&& *sym != ')' && *sym != ';' && *sym != ',' && *sym != '\0')
		return true;
	return false;
}

bool is_f_sym_is_num(char *sym){
	unsigned int f = 1;
	while(*(sym+f) == ' ')
		f++;
	if(*(sym+f) >= '0' && *(sym+f) <= '9')
		return true;
	return false;
}

void expr_lex (char *buf, std::vector<Token> *Tokens){
	char str [50];
	Token t;
	char *parse = buf;
	int i;
	int par_pr = 0;
	bool is_arr = false, is_str = false, is_wm = false;
	while(1){
		i = 0;
		while(*parse == ' ')
			parse++;
		if(is_corr_sym(parse, Tokens)){
			if(*parse == '-'){
				if(is_f_sym_is_num(parse) == false)
					is_wm = true;
				else{
					str[i] = *parse;
					i++;
				}
				parse++;
			}
			while(is_corr_ending(parse)){
				str[i] = *parse;
				i++;
				parse++;
				if(*parse == '['){
					str[i] = '%';
					i++;
					parse++;
					while(*parse != ']'){
						str[i] = *parse;
						i++;
						parse++;
					}
					str[i] = '\0';
					i++;
					is_arr = true;
				}
			}
			str[i] = '\0';
		}else if (*parse == '\0'){
			break;
		}else if (*parse == '\''){
			do{
				str[i] = *parse;
				++parse; ++i;
			}while(*parse != '\'');
			str[i] = *parse;
			i++;
			str[i] = '\0';
			is_str = true;
			i++;
			parse++;
		}else{
			if(*parse == '<' && *(parse+1) == '>'){
				str[i] = *parse;
				str[i+1] = *(parse+1);
				str[i+2] = '\0';
				parse+=2;
			}else{
				str[i] = *parse;
				str[i+1] = '\0';
				parse++;
			}
			
		}
		int check;
		if(is_arr && is_wm){
			check = TC_ARR_WM;
			is_arr = false;
			is_wm = false;
		}else if(is_arr){
			check = TC_ARR;
			is_arr = false;
		}else if(is_str){
			check = TC_STRING;
			is_str = false;
		}else if(is_wm){
			check = TC_ID_WM;
			is_wm = false;
		}else
			check = LexAdd(str);
		if(check == TC_SEMICOLON || check == TC_ENDLINE || check == TC_FINAL || check == TC_UNKNOWN)
			break;
		if(check == TC_LPAREN){
			++par_pr;
		}else if(check == TC_RPAREN){
			--par_pr;
		}else{
			t.TokenClass = check;
			t.Lexeme = str;
			t.row = get_prior(check, par_pr);
			Tokens->push_back(t);
		}
	}
}

void vec_copy(std::vector<Token> old_vec, std::vector<Token> *new_vec, unsigned int l, unsigned int r){
	for(unsigned int i = l; i <= r; i++){
		new_vec->push_back(old_vec[i]);
	}
}

void arr_ins (struct ast *tree, std::string ustr, bool is_wm){
	struct ast *ptr1 = tree;
	if(is_wm == true){
		Token wm;
		wm.TokenClass = TC_NUM;
		wm.Lexeme = "-1";
		wm.row = 9999;
		node_insert(ptr1, &(wm), 9999);
		node_insert(ptr1, NULL, AST_TYPE_ID);
		ptr1 = ptr1->nodes[ptr1->nodes.size()-1].ptr_n;
	}
	node_insert(ptr1, NULL, AST_ARR);
	struct ast *ptr2 = ptr1->nodes[ptr1->nodes.size()-1].ptr_n;
	Token id;
	unsigned int i = 0;
	char str1[50];
	for(; ustr[i] != '%';i++){
		str1[i] = ustr[i];
	}
	str1[i] = '\0';
	id.Lexeme = str1;
	i++;
	id.TokenClass = TC_ID;
	node_insert(ptr2, &id, 9999);
	Token ind;
	char str2[50];
	unsigned int j = 0;
	for(; ustr[i] != '\0';i++){
		str2[j] = ustr[i];
		j++;
	}
	str2[j] = '\0';
	ind.Lexeme = str2;
	ind.TokenClass = LexAdd(str2);
	node_insert(ptr2, &ind, 9999);
}

void ex_node_handler(struct ast *tree, Token t){
	struct ast *ptr1;
	if(t.TokenClass == TC_ID || t.TokenClass == TC_ARR){
		node_insert(tree, NULL, AST_TYPE_ID);
		ptr1 = tree->nodes[tree->nodes.size()-1].ptr_n;
		if(t.TokenClass == TC_ARR)
			arr_ins(ptr1, t.Lexeme, false);
		else
			node_insert(ptr1, &(t), 9999);
	}else if(t.TokenClass == TC_ARR_WM){
		node_insert(tree, NULL, AST_BINOP_ASTER);
		ptr1 = tree->nodes[tree->nodes.size()-1].ptr_n;
		arr_ins(ptr1, t.Lexeme, true);
	}else if(t.TokenClass == TC_ID_WM){
		node_insert(tree, NULL, AST_BINOP_ASTER);
		ptr1 = tree->nodes[tree->nodes.size()-1].ptr_n;
		t.TokenClass = TC_ID;
		node_insert(ptr1, NULL, AST_TYPE_ID);
		node_insert(ptr1->nodes[ptr1->nodes.size()-1].ptr_n, &(t), 9999);
		Token wm;
		wm.TokenClass = TC_NUM;
		wm.Lexeme = "-1";
		wm.row = 9999;
		node_insert(ptr1, &(wm), 9999);
	}else
		node_insert(tree, &(t), 9999);
}

void ex_ins_rec(struct ast *tree, std::vector<Token> vec){
	unsigned int size = vec.size();
	if(size == 3){
		tree->type = get_type(vec[1].Lexeme);
		ex_node_handler(tree, vec[0]);
		ex_node_handler(tree, vec[2]);
		return;
	}
	unsigned int min = 999999;
	unsigned int pos = 0;
	for(unsigned int i = 0; i < size; i++){
		if(vec[i].row <= min){
			min = vec[i].row;
			pos = i;
		}
	}
	tree->type = get_type(vec[pos].Lexeme);
	std::vector<struct Token> left;
	vec_copy(vec, &left, 0, pos-1);
	std::vector<struct Token> right;
	vec_copy(vec, &right, pos+1, size-1);
	
	if(left.size() == 1){
		ex_node_handler(tree, left[0]);
	}else{
		node_insert(tree, NULL, 0);
		ex_ins_rec(tree->nodes[0].ptr_n, left);
	}
	if(right.size() == 1){
		ex_node_handler(tree, right[0]);
	}else{
		node_insert(tree, NULL, 0);
		ex_ins_rec(tree->nodes[1].ptr_n, right);
	}
}

void expr_insert(struct ast *tree, char *expr){
	std::vector<struct Token> Tokens;
	expr_lex(expr, &Tokens);
	if(Tokens.size() == 1){
			if(Tokens[0].TokenClass == TC_NUM){
				tree->type = AST_TYPE_NUM;
				node_insert(tree, &(Tokens[0]), 9999);
			}else if(Tokens[0].TokenClass == TC_ID){
				tree->type = AST_TYPE_ID;
				node_insert(tree, &(Tokens[0]), 9999);
			}else if(Tokens[0].TokenClass == TC_ARR){
				tree->type = AST_TYPE_ID;
				arr_ins (tree, Tokens[0].Lexeme, false);
			}else if(Tokens[0].TokenClass == TC_ARR_WM){
				tree->type = AST_TYPE_ID;
				arr_ins (tree, Tokens[0].Lexeme, true);
			}else if(Tokens[0].TokenClass == TC_STRING){
				tree->type = AST_TYPE_STRING;
				node_insert(tree, &(Tokens[0]), 9999);
			}else if(Tokens[0].TokenClass == TC_ID_WM){
				tree->type = AST_BINOP_ASTER;
				Tokens[0].TokenClass = TC_ID;
				node_insert(tree, &(Tokens[0]), 9999);
				Token wm;
				wm.TokenClass = TC_NUM;
				wm.Lexeme = "-1";
				wm.row = 9999;
				node_insert(tree, &(wm), 9999);
			}
			return;
	}
	ex_ins_rec(tree, Tokens);
}

bool show_node (struct ast *node, unsigned int tablevel)
{
	for(unsigned int j = 0; j < tablevel; j++)
			std::cout << "    ";
    if (node == NULL){
        printf ("Node is not exist\n\n");
        return false;
    }

	std::cout << "Node " << AST_NAMES[node->type] << ", " << node->nodes.size() << " elements: ";
	for(unsigned int i = 0; i < node->nodes.size(); i++){
		if(node->nodes[i].type == AST_TYPE_NODE)
			std::cout << AST_NAMES[node->nodes[i].ptr_n->type] << ", ";
		else if(node->nodes[i].type == AST_TYPE_TOKEN)
			std::cout << TC_NAMES[node->nodes[i].ptr_t->TokenClass] << " = " << node->nodes[i].ptr_t->Lexeme << ", ";
	}
	std::cout << "\n";
	return true;
}

void ast_print (struct ast *tree, unsigned int tablevel){
	bool check = show_node (tree, tablevel);
	if(!check)
		return;
	for (unsigned int i = 0; i < tree->nodes.size(); i++){
		if(tree->nodes[i].type == AST_TYPE_NODE)
			ast_print (tree->nodes[i].ptr_n, tablevel+1);
	}
}



