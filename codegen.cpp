#include "codegen.h"

std::vector <std::string> asm_data;
std::vector <std::string> asm_func;
std::vector <std::string> asm_text;
//std::vector <struct Token> p_notation;

unsigned int fnum;
unsigned int bnum;
unsigned int lnum;
std::string g_strsave;
bool g_is_arr;
class symtab *gtab;
bool is_pr_created;
bool is_func;
//bool pr_num;


int line_insert(std::string strins){
	if(is_func)
		asm_func.push_back(strins);
	else
		asm_text.push_back(strins);
	
	return 0;
}

int gen_var(struct ast *tree){
	struct ast *ids = tree->nodes[0].ptr_n;
	for(unsigned int i = 0; i < ids->nodes.size(); i++){
		std::string vec_str;
		vec_str.append(ids->nodes[i].ptr_t->Lexeme);
		vec_str.append(": ");
		if(tree->nodes[1].type == AST_TYPE_TOKEN){
			if(tree->nodes[1].ptr_t->TokenClass == TC_T_INT){
				vec_str.append(".long 0");
			}else if(tree->nodes[1].ptr_t->TokenClass == TC_T_STRING){
				vec_str.append(".string");
			}
		}else{
			struct ast *arrinfo = tree->nodes[1].ptr_n;
			vec_str.append(".long ");
			int arrsize = abs(atoi(arrinfo->nodes[1].ptr_t->Lexeme.c_str()) - atoi(arrinfo->nodes[0].ptr_t->Lexeme.c_str()))+1;
			for(int i = 0; i < arrsize; i++){
				vec_str.append("0");
				if(i != arrsize-1)
					vec_str.append(", ");
			}
		}
		asm_data.push_back(vec_str);
		vec_str.clear();
		vec_str.append("\n");
		asm_data.push_back(vec_str);
	}
	return 0;
}

int gen_decl(struct ast *tree){
	is_func = true;
	bool unpack = false;
	int i = 1;
	std::string fstr;
	//std::cout << "current func is " << tree->nodes[0].ptr_t->Lexeme << "\n";
	fstr.append(tree->nodes[0].ptr_t->Lexeme);
	fstr.append(":");
	line_insert(fstr);
	fstr.clear();
	if(tree->nodes[i].ptr_n != NULL){
		gen_var(tree->nodes[i].ptr_n);
		unpack = true;
		i++;
	}
	i++;
	
	if(unpack){
		fstr.append("movl %eax, ");
		fstr.append(tree->nodes[1].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
		line_insert(fstr);
		fstr.clear();
		if(tree->nodes[1].ptr_n->nodes[0].ptr_n->nodes.size() == 2){
			fstr.append("movl %ebx, ");
			fstr.append(tree->nodes[1].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->Lexeme);
			line_insert(fstr);
			fstr.clear();
		}
	}
	//std::cout << "i = " << i << "\n";
	gen_main(tree->nodes[i].ptr_n);
	//std::cout << "azazaz\n";
	line_insert("ret");
	is_func = false;
	return 0;
}

int gen_pre(struct ast *tree){
	for(unsigned int i = 0; i < tree->nodes.size(); i++){
		if(tree->nodes[i].ptr_n->type == AST_VAR){
			gen_var(tree->nodes[i].ptr_n);
		}else if(tree->nodes[i].ptr_n->type == AST_FUNC_DECL){
			gen_decl(tree->nodes[i].ptr_n);
		}
	}
	return 0;
}

int is_inout(std::string str){
	if(str == "write")
		return 1;
	else if(str == "writeln")
		return 2;
	else if(str == "read")
		return 3;
	else if(str == "readln")
		return 4;
	else
		return 0;
}

bool is_binop(unsigned int type){
	switch (type){
		case AST_BINOP_PLUS:
		case AST_BINOP_MINUS:
		case AST_BINOP_ASTER:
		case AST_BINOP_DIV:
		case AST_BINOP_MOD:
			return true;
	}
	return false;
}

int op_type(unsigned int type){
	switch (type){
		case AST_BINOP_PLUS:
			return 1;
		case AST_BINOP_MINUS:
			return 2;
		case AST_BINOP_ASTER:
			return 3;
		case AST_BINOP_DIV:
			return 4;
		case AST_BINOP_MOD:
			return 5;
	}
	return 0;
}

std::string gen_pn(struct ast *tree, bool pn_num){
	//std::cout << AST_NAMES[tree->type] << "\n";
	std::string vec_str[6], str1, str2;
	int type = op_type(tree->type);
	int i = 0;
	//std::cout << "starting pn, current operation is " << AST_NAMES[tree->type] << "\n";
	if(!is_pr_created){
		vec_str[0].append("pr1: .long 0");
		asm_data.push_back(vec_str[0]);
		vec_str[0].clear();
		vec_str[0].append("pr2: .long 0");
		asm_data.push_back(vec_str[0]);
		vec_str[0].clear();
		is_pr_created = true;
	}
	if(type == 4 || type == 5){
		vec_str[i].append("movl $0, %edx");
		i++;
	}
	if(tree->nodes[0].type == AST_TYPE_NODE && tree->nodes[0].ptr_n->type == AST_TYPE_ID){
		if(tree->nodes[0].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr1arr1\n";
			vec_str[i].append("movl ");
			if(tree->nodes[0].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->TokenClass == TC_NUM)
				vec_str[i].append("$");
			vec_str[i].append(tree->nodes[0].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->Lexeme);
			vec_str[i].append(", %ecx");
			i++;
		}
	}
	vec_str[i].append("movl ");
	if(tree->nodes[0].type == AST_TYPE_TOKEN){
		//str1 = gen_pn_num(tree->nodes[0].ptr_t, false);
		vec_str[i].append("$");
		vec_str[i].append(tree->nodes[0].ptr_t->Lexeme);
	}else if(tree->nodes[0].ptr_n->type == AST_TYPE_ID){
		if(tree->nodes[0].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr2arr1\n";
			//std::cout << tree->nodes[0].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme <<"\n";
			vec_str[i].append(tree->nodes[0].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
			vec_str[i].append("(, %ecx, 4)");
		}else{
			vec_str[i].append(tree->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
		}
	}else{
		str1 = gen_pn(tree->nodes[0].ptr_n, false);
		vec_str[i].append(str1);
	}
	vec_str[i].append(", %eax");
	i++;
	if(type == 4 || type == 5){
		vec_str[i].append("movl $0, %edx");
		i++;
	}
	if(tree->nodes[1].ptr_n != NULL)
		//std::cout << "grm, "<< AST_NAMES[tree/*->nodes[1].ptr_n*/->type] <<"\n";
	if(tree->nodes[1].type == AST_TYPE_NODE && tree->nodes[1].ptr_n->type == AST_TYPE_ID){
		if(tree->nodes[1].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr1arr1\n";
			vec_str[i].append("movl ");
			if(tree->nodes[1].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->TokenClass == TC_NUM)
				vec_str[i].append("$");
			vec_str[i].append(tree->nodes[1].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->Lexeme);
			vec_str[i].append(", %ecx");
			i++;
		}
	}
	//std::cout << "azazaz\n";
	if(type == 1 )
		vec_str[i].append("addl ");
	else if(type == 2)
		vec_str[i].append("subl ");
	//else if(type == 3)
		//vec_str[i].append("imull ");
	else if(type == 3 || type == 4 || type == 5)
		vec_str[i].append("movl ");
	
	
	if(tree->nodes[1].type == AST_TYPE_TOKEN){
		vec_str[i].append("$");
		vec_str[i].append(tree->nodes[1].ptr_t->Lexeme);
	}else if(tree->nodes[1].ptr_n->type == AST_TYPE_ID){
		if(tree->nodes[1].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr2arr1\n";
			//std::cout << tree->nodes[1].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme <<"\n";
			vec_str[i].append(tree->nodes[1].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
			vec_str[i].append("(, %ecx, 4)");
		}else{
			vec_str[i].append(tree->nodes[1].ptr_n->nodes[0].ptr_t->Lexeme);
		}
	}else{
		str2 = gen_pn(tree->nodes[1].ptr_n, true);
		vec_str[i].append(str2);
	}
	if(type == 1 || type == 2){
		vec_str[i].append(", %eax");
	}else if(type == 3 || type == 4 || type == 5){
		vec_str[i].append(", %ebx");
	}
	i++;
	if(type == 4 || type == 5){
		vec_str[i].append("idivl %ebx");
		i++;
	}else if(type == 3){
		vec_str[i].append("imull %ebx");
		i++;
	}
	vec_str[i].append("movl ");
	if(type != 5){
		vec_str[i].append("%eax, ");
	}else
		vec_str[i].append("%edx, ");
	if(!pn_num)
		vec_str[i].append("pr1");
	else
		vec_str[i].append("pr2");
	i++;
	for(int j = 0; j < i; j++){
		line_insert(vec_str[j]);
	}
	if(!pn_num)
		return "pr1";
	else
		return "pr2";
}

int gen_func(struct ast *tree){
	//std::cout << "entered func!\n";
	int shrink = 1;
	int check = is_inout(tree->nodes[0].ptr_t->Lexeme);
	std::string exstr;
	if(check != 0){
		//std::cout << "check = "<< check <<"\n";
		std::string format;
		std::string fid;
		fid.append("F");
		fid.append(std::to_string(fnum));
		format.append(fid);
		format.append(": ");
		format.append(".string ");
		format.append("\"");
		struct ast *node;
		if(tree->nodes.size() > 1){
			for(unsigned int i = 1; i < tree->nodes.size(); i++){
				//std::cout << "i = " << i << "\n";
				node = tree->nodes[i].ptr_n;
				//std::cout << "node type is " << AST_NAMES[node->type] << "\n";
				if(node->type == AST_TYPE_STRING){
					node->nodes[0].ptr_t->Lexeme.erase(0,1);
					node->nodes[0].ptr_t->Lexeme.erase((node->nodes[0].ptr_t->Lexeme.size()-1),1);
					//std::cout << node->nodes[0].ptr_t->Lexeme << "\n";
					format.append(node->nodes[0].ptr_t->Lexeme);
				}else if(node->type == AST_TYPE_ID && node->nodes[0].type == AST_TYPE_TOKEN){
					//if(!is_func){
						struct tabnode* n = gtab->get(node->nodes[0].ptr_t->Lexeme);
						if(n->type == TC_T_INT){
							format.append("%d");
						}else if(n->type == TC_T_STRING){
							format.append("%s");
						}
					//}else{
						//format.append("%d");
					//}
				}else if(is_binop(node->type)){
					format.append("%d");
					//gen_pn(n);
				}else if(node->type == AST_TYPE_ID && node->nodes[0].type == AST_TYPE_NODE){
					format.append("%d");
				}
			}
			//std::cout << "arrrrrrrr, \n";
			for(unsigned int i = tree->nodes.size()-1; i > 0; i--){
				node = tree->nodes[i].ptr_n;
				//std::cout << "node type is " << AST_NAMES[node->type] << "\n";
				std::string fstr;
				if(node->type == AST_TYPE_ID && node->nodes[0].type == AST_TYPE_TOKEN){
					node = tree->nodes[i].ptr_n;
					//std::string fstr;
					fstr.append("pushl ");
					if(/*node->nodes[0].ptr_t->TokenClass == TC_ID || */check == 3 || check == 4)
						fstr.append("$");
					fstr.append(node->nodes[0].ptr_t->Lexeme);
					line_insert(fstr);
					//std::cout << "kek: " << node->nodes[0].ptr_t->Lexeme << "\n";
					++shrink;
				}else if(is_binop(node->type)){
					
					std::string pnres = gen_pn(node, false);
					fstr.append("pushl ");
					fstr.append(pnres);
					//std::cout << "wow: " << pnres << "\n";
					line_insert(fstr);
					++shrink;
				}else if(node->type == AST_TYPE_ID && node->nodes[0].type == AST_TYPE_NODE){
					//std::cout << "gmm\n";
					fstr.append("pushl ");
					struct ast *arrinfo = node->nodes[0].ptr_n;
					//std::cout << "generating " << arrinfo->nodes[0].ptr_t->Lexeme << "[" << arrinfo->nodes[1].ptr_t->Lexeme << "]\n";
					if(check == 1 || check == 2){
						
						std::string pre_arr;
						
						pre_arr.append("movl ");
						if(arrinfo->nodes[1].ptr_t->TokenClass == TC_NUM)
							pre_arr.append("$");
						pre_arr.append(arrinfo->nodes[1].ptr_t->Lexeme);
						pre_arr.append(", %ecx");
						line_insert(pre_arr);
						fstr.append(arrinfo->nodes[0].ptr_t->Lexeme);
						fstr.append("(, %ecx, 4)");
						line_insert(fstr);
					}else if(check == 3 || check == 4){
						fstr.append("$");
						fstr.append(arrinfo->nodes[0].ptr_t->Lexeme);
						fstr.append("$");
						fstr.append(std::to_string(atoi(arrinfo->nodes[1].ptr_t->Lexeme.c_str())*4));
					}
					++shrink;
				}
				
			}
		}
		if(check == 2)
			format.append("\\n");
		format.append("\"");
		asm_data.push_back(format);
		exstr.append("pushl $");
		exstr.append(fid);
		line_insert(exstr);
		exstr.clear();
		exstr.append("call ");
		if(check == 1 || check == 2)
			exstr.append("printf");
		else if(check == 3 || check == 4)
			exstr.append("scanf");
		line_insert(exstr);
		exstr.clear();
		exstr.append("addl  $");
		exstr.append(std::to_string(shrink*4));
		exstr.append(", %esp");
		line_insert(exstr);
		++fnum;
	}else{
		//std::cout << "current func is " << tree->nodes[0].ptr_t->Lexeme << "\n";
		if(tree->nodes.size() > 1){
			for(unsigned int i = 1; i < tree->nodes.size(); i++){
				//std::cout << "i = " << i << "\n";
				if(tree->nodes[i].ptr_n->nodes[0].type == AST_TYPE_NODE){
					exstr.append("movl ");
					if(tree->nodes[i].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->TokenClass == TC_NUM)
						exstr.append("$");
					exstr.append(tree->nodes[i].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->Lexeme);
					exstr.append(", %ecx");
					line_insert(exstr);
					exstr.clear();
				}
				exstr.append("movl ");
				if(tree->nodes[i].ptr_n->nodes[0].type == AST_TYPE_NODE){
					
					exstr.append(tree->nodes[i].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
					exstr.append("(, %ecx, 4)");
				}else if(tree->nodes[i].ptr_n->nodes[0].ptr_t->TokenClass == TC_NUM){
					exstr.append("$");
					exstr.append(tree->nodes[i].ptr_n->nodes[0].ptr_t->Lexeme);
				}else
					exstr.append(tree->nodes[i].ptr_n->nodes[0].ptr_t->Lexeme);
				if(i == 1)
					exstr.append(", %eax");
				else
					exstr.append(", %ebx");
				line_insert(exstr);
				exstr.clear();
				
					
			}
		}
		exstr.append("call ");
		exstr.append(tree->nodes[0].ptr_t->Lexeme);
		line_insert(exstr);
	}
	return 0;
}

std::string gen_assign(struct ast *tree){
	std::string fstr;
	std::string varname;
	fstr.append("movl ");
	g_strsave.clear();
	if(is_binop(tree->nodes[1].ptr_n->type)){
		/*std::string pnres = */gen_pn(tree->nodes[1].ptr_n, false);
		fstr.append("%eax ");
	}else if(tree->nodes[1].type == AST_TYPE_ID){
		if(tree->nodes[1].ptr_n->nodes[0].type == AST_TYPE_TOKEN && 
		tree->nodes[1].ptr_n->nodes[0].ptr_t->TokenClass == TC_STRING){
			std::string fid;
			fid.append("F");
			fid.append(std::to_string(fnum));
			std::string format;
			format.append(fid);
			format.append(": ");
			format.append(".string ");
			format.append("\"");
			std::string str = tree->nodes[1].ptr_n->nodes[0].ptr_t->Lexeme;
			str.erase(0,1);
			str.erase((str.size()-1),1);
			format.append(str);
			format.append("\"");
			asm_data.push_back(format);
			fstr.append("$");
			fstr.append(fid);
			++fnum;
		}else if(tree->nodes[1].ptr_n->nodes[0].type == AST_TYPE_TOKEN && 
		tree->nodes[1].ptr_n->nodes[0].ptr_t->TokenClass == TC_ID)
			fstr.append(tree->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
	}else if(tree->nodes[1].ptr_n->type == AST_TYPE_STRING){
			std::string fid;
			fid.append("F");
			fid.append(std::to_string(fnum));
			std::string format;
			format.append(fid);
			format.append(": ");
			format.append(".string ");
			format.append("\"");
			std::string str = tree->nodes[1].ptr_n->nodes[0].ptr_t->Lexeme;
			str.erase(0,1);
			str.erase((str.size()-1),1);
			format.append(str);
			format.append("\"");
			asm_data.push_back(format);
			fstr.append("$");
			fstr.append(fid);
			++fnum;
	}else if(tree->nodes[1].ptr_n->nodes[0].type == AST_TYPE_NODE){
		std::string pre_arr;
		struct ast *arrinfo = tree->nodes[1].ptr_n->nodes[0].ptr_n;
		pre_arr.append("movl $");
		pre_arr.append(arrinfo->nodes[1].ptr_t->Lexeme);
		pre_arr.append(", %ecx");
		line_insert(pre_arr);
		pre_arr.clear();
		pre_arr.append("movl ");
		pre_arr.append(arrinfo->nodes[0].ptr_t->Lexeme);
		pre_arr.append("(, %ecx, 4), %eax");
		fstr.append("%eax");
	}else{
		fstr.append("$");
		fstr.append(tree->nodes[1].ptr_n->nodes[0].ptr_t->Lexeme);
	}
	fstr.append(", ");
	if(tree->nodes[0].ptr_n->nodes[0].type == AST_TYPE_NODE){
		//std::cout << "time for arr! ";
		std::string pre_arr;
		struct ast *arrinfo = tree->nodes[0].ptr_n->nodes[0].ptr_n;
		//std::cout << arrinfo->nodes[0].ptr_t->Lexeme <<"["<<arrinfo->nodes[1].ptr_t->Lexeme<<"]\n";
		pre_arr.append("movl $");
		pre_arr.append(arrinfo->nodes[1].ptr_t->Lexeme);
		if(arrinfo->nodes[1].ptr_t->TokenClass == TC_NUM)
			g_strsave.append("$");
		g_strsave.append(arrinfo->nodes[1].ptr_t->Lexeme);
		pre_arr.append(", %ecx");
		line_insert(pre_arr);
		fstr.append(arrinfo->nodes[0].ptr_t->Lexeme);
		varname.append(arrinfo->nodes[0].ptr_t->Lexeme);
		fstr.append("(, %ecx, 4)");
		varname.append("(, %ecx, 4)");
		g_is_arr = true;
	}else{
		fstr.append(tree->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
		varname.append(tree->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
		g_is_arr = false;
	}
	line_insert(fstr);
	return varname;
}

int is_logic(unsigned int type){
	switch (type){
		case AST_LOGIC_ISEQUAL:
			return 1;
		case AST_LOGIC_ISNOTEQUAL:
			return 2;
		case AST_LOGIC_ISLESS:
			return 3;
		case AST_LOGIC_ISMORE:
			return 4;
	}
	return 0;
}

int gen_if(struct ast *tree, unsigned int curr_bnum, unsigned int cb, bool is_print_next){
	std::string vec_str[4];
	std::string str1;
	if(cb != 0){
		str1.append("C");
		str1.append(std::to_string(curr_bnum));
		str1.append("_");
		str1.append(std::to_string(cb));
		str1.append(":");
		line_insert(str1);
		str1.clear();
	}
	int i = 0;
	int check = is_logic(tree->nodes[0].ptr_n->type);
	struct ast *cond = tree->nodes[0].ptr_n;
	if(cond->nodes[1].type == AST_TYPE_NODE && cond->nodes[1].ptr_n->type == AST_TYPE_ID){
		if(cond->nodes[1].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr1arr1\n";
			vec_str[i].append("movl ");
			if(cond->nodes[1].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->TokenClass == TC_NUM)
				vec_str[i].append("$");
			vec_str[i].append(cond->nodes[1].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->Lexeme);
			vec_str[i].append(", %ecx");
			i++;
		}
	}
	
	vec_str[i].append("movl ");
	
	if(cond->nodes[1].type == AST_TYPE_TOKEN){
		//str1 = gen_pn_num(tree->nodes[0].ptr_t, false);
		vec_str[i].append("$");
		vec_str[i].append(cond->nodes[1].ptr_t->Lexeme);
	}else if(cond->nodes[1].ptr_n->type == AST_TYPE_ID){
		if(cond->nodes[1].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr2arr1\n";
			//std::cout << tree->nodes[0].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme <<"\n";
			vec_str[i].append(cond->nodes[1].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
			vec_str[i].append("(, %ecx, 4)");
		}else{
			vec_str[i].append(cond->nodes[1].ptr_n->nodes[0].ptr_t->Lexeme);
		}
	}else if(is_binop(cond->nodes[1].ptr_n->type)){
		str1 = gen_pn(cond->nodes[1].ptr_n, false);
		vec_str[i].append(str1);
	}
	vec_str[i].append(", %eax");
	i++;
	
	if(cond->nodes[0].type == AST_TYPE_NODE && cond->nodes[0].ptr_n->type == AST_TYPE_ID){
		if(cond->nodes[0].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr1arr1\n";
			vec_str[i].append("movl ");
			if(cond->nodes[0].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->TokenClass == TC_NUM)
				vec_str[i].append("$");
			vec_str[i].append(cond->nodes[0].ptr_n->nodes[0].ptr_n->nodes[1].ptr_t->Lexeme);
			vec_str[i].append(", %ecx");
			i++;
		}
	}
	
	vec_str[i].append("cmpl %eax, ");
	
	if(cond->nodes[0].ptr_n->type == AST_TYPE_ID){
		if(cond->nodes[0].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr2arr1\n";
			//std::cout << tree->nodes[0].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme <<"\n";
			vec_str[i].append(cond->nodes[0].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
			vec_str[i].append("(, %ecx, 4)");
		}else{
			vec_str[i].append(cond->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
		}
	}
	i++;
	
	for(int j = 0; j < i; j++){
		line_insert(vec_str[j]);
	}
	str1.clear();
	if(check == 1)
		str1.append("je ");
	if(check == 2)
		str1.append("jne ");
	if(check == 3)
		str1.append("jl ");
	if(check == 4)
		str1.append("jg ");
	str1.append("B");
	str1.append(std::to_string(curr_bnum));
	str1.append("_");
	str1.append(std::to_string(cb));
	line_insert(str1);
	str1.clear();
	if(is_print_next){
		str1.append("jmp C");
		str1.append(std::to_string(curr_bnum));
		str1.append("_");
		str1.append(std::to_string(cb+1));
		line_insert(str1);
		str1.clear();
	}
	str1.append("B");
	str1.append(std::to_string(curr_bnum));
	str1.append("_");
	str1.append(std::to_string(cb));
	str1.append(":");
	line_insert(str1);
	str1.clear();
	gen_main(tree->nodes[1].ptr_n);
	str1.append("jmp BE");
	str1.append(std::to_string(curr_bnum));
	line_insert(str1);
	return 0;
}

int gen_choice(struct ast *tree){
	std::string fstr;
	unsigned int curr_bnum = bnum;
	bnum++;
	for(unsigned int i = 0; i < tree->nodes.size(); i++){
		if(tree->nodes[i].ptr_n->type == AST_IF){
			bool check;
			if(i != tree->nodes.size()-1)
				check = true;
			else check = false;
			gen_if(tree->nodes[i].ptr_n, curr_bnum, i, check);
		}else if (tree->nodes[i].ptr_n->type == AST_ELSE){
			fstr.append("C");
			fstr.append(std::to_string(curr_bnum));
			fstr.append("_");
			fstr.append(std::to_string(i));
			fstr.append(":");
			line_insert(fstr);
			fstr.clear();
			gen_main(tree->nodes[i].ptr_n);
		}
	}
	fstr.append("BE");
	fstr.append(std::to_string(curr_bnum));
	fstr.append(":");
	line_insert(fstr);
	return 0;
}

int gen_loop(struct ast *tree){
	//std::cout << "current loop is " << AST_NAMES[tree->type] << "\n";
	unsigned int curr_lnum = lnum;
	std::string strsave;
	lnum++;
	//int i = 0;
	std::string str1;
	std::string str2;
	bool isarr;
	std::string varname;
	unsigned int check;
	struct ast *cond;
	int c;
	if(tree->type == AST_FOR){
		varname = gen_assign(tree->nodes[0].ptr_n);
		isarr = g_is_arr;
		check = tree->nodes[1].ptr_t->TokenClass;
		strsave = g_strsave;
	}else{
		//int c;
		if(tree->type == AST_WHILE)
			c = 0;
		else c = 1;
		check = tree->nodes[c].ptr_n->type;
		if(tree->nodes[c].ptr_n->nodes[0].ptr_n->nodes[0].type == AST_TYPE_TOKEN){
			varname = tree->nodes[c].ptr_n->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme;
			isarr = false;
		}else{
			cond = tree->nodes[c].ptr_n->nodes[0].ptr_n->nodes[0].ptr_n;
			varname.append(cond->nodes[0].ptr_t->Lexeme);
			varname.append("(, %ecx, 4)");
			if(cond->nodes[1].ptr_t->TokenClass == TC_NUM)
				str1.append("$");
			str1.append(cond->nodes[1].ptr_t->Lexeme);
			strsave = str1;
			str1.clear();
			isarr = true;
		}
	}
	
	if(tree->type == AST_FOR || tree->type == AST_WHILE){
		str1.append("jmp ");
		str1.append("L");
		str1.append(std::to_string(curr_lnum));
		str1.append("_check");
		line_insert(str1);
		str1.clear();
	}
	
	str1.append("L");
	str1.append(std::to_string(curr_lnum));
	str1.append(":");
	line_insert(str1);
	str1.clear();

	if(tree->type == AST_FOR)
		gen_main(tree->nodes[3].ptr_n);
	else if(tree->type == AST_WHILE)
		gen_main(tree->nodes[1].ptr_n);
	else if(tree->type == AST_REPUN)
		gen_main(tree->nodes[0].ptr_n);
	
	if (tree->type == AST_FOR){
		if(isarr){
			str1.append("movl $");
			str1.append(strsave);
			str1.append(", %ecx");
			line_insert(str1);
			str1.clear();
		}
		if(check == TC_TO)
			str1.append("incl ");
		else
			str1.append("decl ");
	
		str1.append(varname);
		line_insert(str1);
		str1.clear();
	}
	if(tree->type == AST_FOR || tree->type == AST_WHILE){
		str1.append("L");
		str1.append(std::to_string(curr_lnum));
		str1.append("_check:");
		line_insert(str1);
		str1.clear();
	}
	if(tree->type == AST_FOR)
		cond = tree->nodes[2].ptr_n;
	else if(tree->type == AST_WHILE){
		cond = tree->nodes[0].ptr_n->nodes[1].ptr_n;
	}else if(tree->type == AST_REPUN){
		cond = tree->nodes[1].ptr_n->nodes[1].ptr_n;
	}
	if(cond != NULL && cond->type == AST_TYPE_ID && cond->nodes[0].type == AST_TYPE_NODE){
		//if(cond->nodes[1].ptr_n->nodes[0].type == AST_TYPE_NODE){
			//std::cout << "tr1arr1\n";
			str1.append("movl $");
			str1.append(cond->nodes[0].ptr_n->nodes[1].ptr_t->Lexeme);
			str1.append(", %ecx");
			line_insert(str1);
			str1.clear();
		//}
	}
	
	str1.append("movl ");
	if(cond == NULL){
		str1.append("$");
		str1.append(tree->nodes[c].ptr_n->nodes[1].ptr_t->Lexeme);
	}else if(cond->type == AST_TYPE_NUM){
		str1.append("$");
		str1.append(cond->nodes[0].ptr_t->Lexeme);
	}else if(cond->type == AST_TYPE_ID){
		if(cond->nodes[0].type == AST_TYPE_NODE){
			str1.append(cond->nodes[0].ptr_n->nodes[0].ptr_t->Lexeme);
			str1.append("(, %ecx, 4)");
		}else{
			str1.append(cond->nodes[0].ptr_t->Lexeme);
		}
	}else if(is_binop(cond->type)){
		str2 = gen_pn(cond, false);
		str1.append(str2);
	}
	str1.append(", %eax");
	line_insert(str1);
	str1.clear();
	
	if(isarr){
		str1.append("movl ");
		str1.append(strsave);
		str1.append(", %ecx");
		line_insert(str1);
		str1.clear();
	}
	
	str1.append("cmpl %eax, ");
	str1.append(varname);
	line_insert(str1);
	str1.clear();
	if(tree->type == AST_FOR){
		if(check == TC_TO)
			str1.append("jng ");
		else
			str1.append("jnl ");
	}else{
		if(check == AST_LOGIC_ISEQUAL)
			str1.append("je ");
		else if(check == AST_LOGIC_ISNOTEQUAL)
			str1.append("jne ");
		else if(check == AST_LOGIC_ISLESS)
			str1.append("jl ");
		else if(check == AST_LOGIC_ISMORE)
			str1.append("jg ");
	}
	str1.append("L");
	str1.append(std::to_string(curr_lnum));
	line_insert(str1);
	str1.clear();
	
	return 0;
}
bool is_loop(unsigned int type){
	if(type == AST_FOR || type == AST_REPUN || type == AST_WHILE)
		return true;
	return false;
}
int gen_main(struct ast *tree){
	for(unsigned int i = 0; i < tree->nodes.size(); i++){
		if(tree->nodes[i].ptr_n->type == AST_FUNC){
			gen_func(tree->nodes[i].ptr_n);
		}else if(tree->nodes[i].ptr_n->type == AST_ASSIGN){
			gen_assign(tree->nodes[i].ptr_n);
		}else if(tree->nodes[i].ptr_n->type == AST_CHOICE){
			gen_choice(tree->nodes[i].ptr_n);
		}else if(is_loop(tree->nodes[i].ptr_n->type)){
			gen_loop(tree->nodes[i].ptr_n);
		}
	}
	return 0;
}

int codegen(struct ast *tree, class symtab *t){
	fnum = 0;
	bnum = 0;
	lnum = 0;
	gtab = t;
	is_pr_created = false;
	is_func = false;
	gen_pre(tree->nodes[0].ptr_n);
	//std::cout << "pre has been passed\n";
	gen_main(tree->nodes[1].ptr_n);
	//std::cout << "gen finished\n";
	std::ofstream fout;
	//fout.open("asmtest.s");
	//std::cout << ".data\n";
	fout << ".data\n";
	for(unsigned int i = 0; i < asm_data.size(); i++)
		//std::cout << asm_data[i] << "\n";
		fout << asm_data[i] << "\n";
	//std::cout << ".text\n";
	//std::cout << ".globl main\n";
	//std::cout << "main:\n";
	
	for(unsigned int i = 0; i < asm_func.size(); i++)
		//std::cout << asm_func[i] << "\n";
		fout << asm_func[i] << "\n";
	
	fout << ".text\n";
	fout << ".globl main\n";
	fout << "main:\n";
	for(unsigned int i = 0; i < asm_text.size(); i++)
		//std::cout << asm_text[i] << "\n";
		fout << asm_text[i] << "\n";
	//std::cout << "ret\n";
	fout << "ret\n";
	return 0;
}