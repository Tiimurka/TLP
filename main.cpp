#include "parse.h"
#include "ast.h"

int main(){
	//std::cout << "hi" << std::endl;	
	struct ast *tree = parse_test("hello.pas");
	ast_print(tree, 0);
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
