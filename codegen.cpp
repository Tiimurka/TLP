#include "codegen.h"

std::vector <std::string> asm_data;
std::vector <std::string> asm_text;

unsigned int fnum;
class symtab *gtab;

int gen_var(struct ast *tree){
	struct ast *ids = tree->nodes[0].ptr_n;
	for(unsigned int i = 0; i < ids->nodes.size(); i++){
		std::string vec_str;
		vec_str.append(ids->nodes[i].ptr_t->Lexeme);
		vec_str.append(": ");
		if(tree->nodes[1].ptr_t->TokenClass == TC_T_INT){
			vec_str.append(".long");
		}else if(tree->nodes[1].ptr_t->TokenClass == TC_T_STRING){
			vec_str.append(".string");
		}
		asm_data.push_back(vec_str);
		vec_str.clear();
		vec_str.append("\n");
		asm_data.push_back(vec_str);
	}
	return 0;
}

int gen_pre(struct ast *tree){
	for(unsigned int i = 0; i < tree->nodes.size(); i++){
		if(tree->nodes[i].ptr_n->type == AST_VAR){
			gen_var(tree->nodes[i].ptr_n);
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

int gen_func(struct ast *tree){
	int shrink = 1;
	int check = is_inout(tree->nodes[0].ptr_t->Lexeme);
	if(check != 0){
		std::string format;
		std::string fid;
		std::string exstr;
		fid.append("F");
		fid.append(std::to_string(fnum));
		format.append(fid);
		format.append(": ");
		format.append(".string ");
		format.append("\"");
		struct ast *node;
		if(tree->nodes.size() > 1){
			for(unsigned int i = 1; i < tree->nodes.size(); i++){
				node = tree->nodes[i].ptr_n;
				if(node->type == AST_TYPE_STRING){
					node->nodes[0].ptr_t->Lexeme.erase(0,1);
					node->nodes[0].ptr_t->Lexeme.erase((node->nodes[0].ptr_t->Lexeme.size()-1),1);
					format.append(node->nodes[0].ptr_t->Lexeme);
				}else if(node->type == AST_TYPE_ID){
					struct tabnode* n = gtab->get(node->nodes[0].ptr_t->Lexeme);
					if(n->type == TC_T_INT){
						format.append("%d");
					}else if(n->type == TC_T_STRING){
						format.append("%s");
					}
				}
			}
			
			for(unsigned int i = tree->nodes.size()-1; i > 0; i--){
				node = tree->nodes[i].ptr_n;
				if(node->type == AST_TYPE_ID){
					node = tree->nodes[i].ptr_n;
					std::string fstr;
					fstr.append("pushl ");
					fstr.append("$");
					fstr.append(node->nodes[0].ptr_t->Lexeme);
					asm_text.push_back(fstr);
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
		asm_text.push_back(exstr);
		exstr.clear();
		exstr.append("call ");
		if(check == 1 || check == 2)
			exstr.append("printf");
		else if(check == 3 || check == 4)
			exstr.append("scanf");
		asm_text.push_back(exstr);
		exstr.clear();
		exstr.append("addl  $");
		exstr.append(std::to_string(shrink*4));
		exstr.append(", %esp");
		asm_text.push_back(exstr);
		++fnum;
	}
	return 0;
}

int gen_main(struct ast *tree){
	for(unsigned int i = 0; i < tree->nodes.size(); i++){
		if(tree->nodes[i].ptr_n->type == AST_FUNC){
			gen_func(tree->nodes[i].ptr_n);
		}
	}
	return 0;
}

int codegen(struct ast *tree, class symtab *t){
	fnum = 0;
	gtab = t;
	gen_pre(tree->nodes[0].ptr_n);
	gen_main(tree->nodes[1].ptr_n);
	std::ofstream fout;
	fout.open("asmtest.s");
	//std::cout << ".data\n";
	fout << ".data\n";
	for(unsigned int i = 0; i < asm_data.size(); i++)
		//std::cout << asm_data[i] << "\n";
		fout << asm_data[i] << "\n";
	//std::cout << ".text\n";
	//std::cout << ".globl main\n";
	//std::cout << "main:\n";
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