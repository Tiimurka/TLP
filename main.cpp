#include "parse.h"
#include "ast.h"
#include "sem.h"
#include "symtab.h"
#include "codegen.h"

bool is_pas(std::string str){
	if(str.size() < 5)
		return false;
	if(str[str.size()-4] == '.' && str[str.size()-3] == 'p' && str[str.size()-2] == 'a' && str[str.size()-1] == 's')
		return true;
	return false;
}

int main(int argc, char *argv[]){
	bool is_dump_tokens = false, is_dump_ast = false, is_dump_symtab = false, is_dump_asm = false, is_fn_set = false;
	std::string str = "arrtest.pas";
	if(argc != 1){
		for(int i = 1; i < argc; i++){
			if(std::strcmp(argv[i], "--dump-tokens") == 0){
				if(!is_dump_tokens)
					is_dump_tokens = true;
				else{
					std::cout << "Error: multiple command '--dump-tokens'\n";
					return -1;
				}
			}else if(std::strcmp(argv[i], "--dump-symtab") == 0){
				if(!is_dump_symtab)
					is_dump_symtab = true;
				else{
					std::cout << "Error: multiple command '--dump-symtab'\n";
					return -1;
				}
			}else if(std::strcmp(argv[i], "--dump-ast") == 0){
				if(!is_dump_ast)
					is_dump_ast = true;
				else{
					std::cout << "Error: multiple command '--dump-ast'\n";
					return -1;
				}
			}else if(std::strcmp(argv[i], "--dump-asm") == 0){
				if(!is_dump_asm)
					is_dump_asm = true;
				else{
					std::cout << "Error: multiple command '--dump-asm'\n";
					return -1;
				}
			}else {
				std::string check = argv[i];
				if(is_pas(check)){
					if(!is_fn_set){
						str = argv[i];
						is_fn_set = true;
					}else{
						std::cout << "Error: filename is already set\n";
						return -1;
					}
				}else{
					std::cout << "Error: incorrect argument\n";
					return -1;
				}
			}
		}
	}
	struct ast *tree = parse_test(str.c_str(), is_dump_tokens);
	if(is_dump_ast)
		ast_print(tree, 0);
	symtab s(tree);
	if(is_dump_symtab)
		s.show();
	//struct tabnode* n = s.get("a8");
	//if (n == NULL)
		//std::cout << "var not found\n";
	//else
		//std::cout << "Check: " << n->level << " " << n->sublevel << "\n";
	sem_analyzer(tree, &s);
	if(is_dump_asm)
		codegen(tree, &s);
	//show_node(tree, 0);
	//show_node(tree->nodes[0].ptr_n, 1);
	//show_node(tree->nodes[1].ptr_n, 1);
	/*char str[] = "(2*(((322+1)*(2*2)) mod (2+3))) div 10;";
	struct ast *tree = ast_create(0);
	char *test = str;
	expr_insert(tree, test);
	std::cout << AST_NAMES[tree->type] << "\n";
	std::cout << AST_NAMES[tree->nodes[0].ptr_n->type] << "\n";
	std::cout << AST_NAMES[tree->nodes[0].ptr_n->nodes[1].ptr_n->type] << "\n";
	std::cout << tree->nodes[1].ptr_t->Lexeme << "\n";*/
	
	/*for (unsigned int i = 0; i < Tokens.size(); i++){
		std::cout << TC_NAMES[Tokens[i].TokenClass] << " " << Tokens[i].Lexeme << std::endl;
	}
	std::cout<< "Result: " << check << std::endl;*/
	return 0;
}
