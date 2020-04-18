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
	//if(type == AST_FUNC)
		//std::cout << "ALERT\n";
	struct ast_node *node;
	if(t == NULL){
		struct ast *n = ast_create(type);
		node = node_create(NULL, n);
	}else
		node = node_create(t, NULL);
	tree->nodes.push_back(*node);
}

/*struct ast_node *node_create(struct ast *n){
	struct ast_node *node = new ast_node;
	node->type = AST_TYPE_NODE;
	node->ptr_t = NULL;
	node->ptr_n = n;
	return node;
}

void node_insert(struct ast *tree, unsigned int type){
	struct ast *n = ast_create(type);
	struct ast_node *node = node_create(n);
	tree->nodes.push_back(*node);
}*/

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
	//i++;
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

 bool is_corr_sym (char sym){
	if(((sym >= 'A' && sym <= 'Z') || (sym >= 'a' && sym <= 'z') || sym == '_') ||
	(sym == '%' || sym == '&') || (sym >= '0' && sym <= '9'))
		return true;
	return false;
 }

bool is_corr_ending(char sym){
	while(sym != '+' && sym != '-' && sym != '/' && sym != '*' && sym != '='
	&& sym != '<' && sym != '>' && sym != ' ' 
	&& sym != ')' && sym != ';' && sym != ',' && sym != '\0')
		return true;
	return false;
}

void expr_lex (char *buf, std::vector<Token> *Tokens){
	char str [50];
	Token t;
	char *parse = buf;
	int i;
	int par_pr = 0;
	while(1){
		i = 0;
		while(*parse == ' ')
			parse++;
		if(is_corr_sym(*parse)){
			while(is_corr_ending(*parse)){
				str[i] = *parse;
				i++;
				parse++;
			}
			str[i] = '\0';
		}else if (*parse == '\0'){
			break;
		}else{
			if(*parse == '<' && *(parse+1) == '>'){
				str[i] = *parse;
				str[i+1] = *(parse+1);
				str[i+2] = '\0';
			}else{
				str[i] = *parse;
				str[i+1] = '\0';
				parse++;
			}
			
		}
		/*str[i] = *parse;
		str[i+1] = '\0';
		i++;
		parse++;*/
		/*while(LexAdd(str) == TC_UNKNOWN){
			while(*parse == ' ')
				parse++;
			while()
			str[i] = *parse;
			str[i+1] = '\0';
			i++;
			parse++;
		}*/
		int check = LexAdd(str);
		
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
		//parse++;
	}
}

void vec_copy(std::vector<Token> old_vec, std::vector<Token> *new_vec, unsigned int l, unsigned int r){
	for(unsigned int i = l; i <= r; i++){
		new_vec->push_back(old_vec[i]);
	}
}

void ex_ins_rec(struct ast *tree, std::vector<Token> vec){
	unsigned int size = vec.size();
	if(size == 3){
		//for(unsigned int i = 0; i < vec.size(); i++){
			//std::cout << vec[i].row << " " << TC_NAMES[vec[i].TokenClass] << " " << vec[i].Lexeme << std::endl;
		//}
		tree->type = get_type(vec[1].Lexeme);
		node_insert(tree, &(vec[0]), 9999);
		node_insert(tree, &(vec[2]), 9999);
		//std::cout << "Adding finished: " << tree->nodes[0].ptr_t->Lexeme << " " << vec[1].Lexeme << " " << tree->nodes[1].ptr_t->Lexeme << "\n";
		//std::cout << "\n";
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
	//std::cout << "dividing by " << vec[pos].Lexeme << ", pos = " << pos << ":\n";
	//for(unsigned int i = 0; i < vec.size(); i++){
		//std::cout << vec[i].row << " " << TC_NAMES[vec[i].TokenClass] << " " << vec[i].Lexeme << std::endl;
	//}
	//std::cout << "\n";
	//unsigned int type = get_type(vec[pos].Lexeme);
	//if(tree == NULL)
		//std::cout << "AZAZAZ\n";
	tree->type = get_type(vec[pos].Lexeme);
	std::vector<struct Token> left;
	vec_copy(vec, &left, 0, pos-1);
	std::vector<struct Token> right;
	vec_copy(vec, &right, pos+1, size-1);
	
	if(left.size() == 1){
		node_insert(tree, &(left[0]), 9999);
		//std::cout << "Adding finished: " << left[0].Lexeme << "\n";
		//return;
	}else{
		node_insert(tree, NULL, 0);
		ex_ins_rec(tree->nodes[0].ptr_n, left);
	}
	if(right.size() == 1){
		node_insert(tree, &(right[0]), 9999);
		//std::cout << "Adding finished: " << right[0].Lexeme << "\n";
		//return;
	}else{
		node_insert(tree, NULL, 0);
		ex_ins_rec(tree->nodes[1].ptr_n, right);
	}
}

void expr_insert(struct ast *tree, char *expr){
	//std::cout << "Hi! I am adding expr: " << expr << "\n";
	std::vector<struct Token> Tokens;
	expr_lex(expr, &Tokens);
	if(Tokens.size() == 1){
		if(Tokens[0].TokenClass == TC_NUM || Tokens[0].TokenClass == TC_ID){
			//std::cout << "There is only one token! Changing node type!\n";
			if(Tokens[0].TokenClass == TC_NUM){
				tree->type = AST_TYPE_NUM;
				node_insert(tree, &(Tokens[0]), 9999);
			}else if(Tokens[0].TokenClass == TC_ID){
				tree->type = AST_TYPE_ID;
				node_insert(tree, &(Tokens[0]), 9999);
			}
			return;
		}
	}
	//for(unsigned int i = 0; i < Tokens.size(); i++){
		//std::cout << Tokens[i].row << " " << TC_NAMES[Tokens[i].TokenClass] << " " << Tokens[i].Lexeme << std::endl;
	//}
	//std::cout << "\n";
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

	//if(node->type == AST_TYPE_TOKEN){}

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
	//struct ast_node *crutch = node_create(NULL, root);
	bool check = show_node (tree, tablevel);
	if(!check)
		return;
	for (unsigned int i = 0; i < tree->nodes.size(); i++){
		if(tree->nodes[i].type == AST_TYPE_NODE)
			ast_print (tree->nodes[i].ptr_n, tablevel+1);
	}
}



