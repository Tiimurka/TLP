#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "parse.h"
#include "sem.h"
#include "symtab.h"
#include "codegen.h"

struct ast *tree;

TEST_CASE ("codegen_test1", "check_var_decl"){
	tree = parse_test("utestsite.pas", false);
	symtab s(tree);
	codegen_init(&s);
	tree = sem_analyzer(tree, &s);
	struct ast *ptr = tree->nodes[0].ptr_n;
	gen_var(ptr->nodes[0].ptr_n);
	REQUIRE(get_asm_line(0, 0) == "k: .long 0");
	REQUIRE(get_asm_line(0, 2) == "l: .long 0");
	REQUIRE(get_asm_line(0, 4) == "n: .long 0");
	clear_asm_line(0);
}

TEST_CASE ("codegen_test2", "check_func"){
	tree = parse_test("utestsite.pas", false);
	symtab s(tree);
	codegen_init(&s);
	struct ast *ptr = tree->nodes[1].ptr_n;
	//std::cout << AST_NAMES[ptr->nodes[1].ptr_n->type] << "\n";
	gen_func(ptr->nodes[1].ptr_n);
	REQUIRE(get_asm_line(0, 0) == "F0: .string \"c = \\n\"");
	REQUIRE(get_asm_line(2, 0) == "movl c, %esi");
	REQUIRE(get_asm_line(2, 1) == "movl $F0, %edi");
	REQUIRE(get_asm_line(2, 2) == "call printf");
	//REQUIRE(get_asm_line(2, 3) == "addl  $8, %esp");
	clear_asm_line(0);
	clear_asm_line(2);
}

TEST_CASE ("codegen_test3", "check_repun"){
	tree = parse_test("utestsite.pas", false);
	symtab s(tree);
	codegen_init(&s);
	struct ast *ptr = tree->nodes[1].ptr_n;
	//std::cout << AST_NAMES[ptr->nodes[5].ptr_n->type] << "\n";
	gen_loop(ptr->nodes[5].ptr_n);
	/*for(unsigned int i = 0;;i++){
		std::string str = get_asm_line(2, i);
		std::cout << i << " " << str << "\n";
		if(str == "###")
			break;
	}*/
	REQUIRE(get_asm_line(2, 0) == "L0:");
	REQUIRE(get_asm_line(2, 44) == "movl $0, %eax");
	REQUIRE(get_asm_line(2, 45) == "cmpl %eax, check");
	REQUIRE(get_asm_line(2, 46) == "jne L0");
	clear_asm_line(0);
	clear_asm_line(2);
}

TEST_CASE ("codegen_test4", "check_assign"){
	tree = parse_test("utestsite.pas", false);
	symtab s(tree);
	codegen_init(&s);
	struct ast *ptr = tree->nodes[1].ptr_n;
	//std::cout << AST_NAMES[ptr->nodes[6].ptr_n->type] << "\n";
	gen_assign(ptr->nodes[6].ptr_n);
	/*for(unsigned int i = 0;;i++){
		std::string str = get_asm_line(2, i);
		std::cout << i << " " << str << "\n";
		if(str == "###")
			break;
	}*/
	REQUIRE(get_asm_line(2, 0) == "movl k, %eax");
	REQUIRE(get_asm_line(2, 1) == "addl l, %eax");
	REQUIRE(get_asm_line(2, 2) == "movl %eax, pr1");
	REQUIRE(get_asm_line(2, 3) == "movl %eax , n");
	clear_asm_line(0);
	clear_asm_line(2);
}

TEST_CASE ("codegen_test5", "check_func_decl"){
	tree = parse_test("utestsite.pas", false);
	symtab s(tree);
	codegen_init(&s);
	struct ast *ptr = tree->nodes[0].ptr_n;
	//std::cout << AST_NAMES[ptr->nodes[3].ptr_n->type] << "\n";
	gen_decl(ptr->nodes[3].ptr_n);
	/*for(unsigned int i = 0;;i++){
		std::string str = get_asm_line(1, i);
		std::cout << i << " " << str << "\n";
		if(str == "###")
			break;
	}*/
	
	REQUIRE(get_asm_line(1, 0) == "test1:");
	REQUIRE(get_asm_line(1, 1) == "pushq %rbp");
	REQUIRE(get_asm_line(1, 2) == "movq %rsp, %rbp");
	REQUIRE(get_asm_line(1, 3) == "movl %eax, a1");
	REQUIRE(get_asm_line(1, 4) == "movl %ebx, c1");
	REQUIRE(get_asm_line(1, 26) == "leave");
	REQUIRE(get_asm_line(1, 27) == "ret");
	clear_asm_line(1);
}

TEST_CASE ("codegen_test6", "check_pn"){
	tree = parse_test("utestsite.pas", false);
	symtab s(tree);
	codegen_init(&s);
	struct ast *ptr = tree->nodes[0].ptr_n;
	//std::cout << AST_NAMES[ptr->nodes[3].ptr_n->type] << "\n";
	gen_decl(ptr->nodes[3].ptr_n);
	/*for(unsigned int i = 0;;i++){
		std::string str = get_asm_line(1, i);
		std::cout << i << " " << str << "\n";
		if(str == "###")
			break;
	}*/
	REQUIRE(get_asm_line(1, 10) == "movl %eax, pr1");
	REQUIRE(get_asm_line(1, 11) == "movl pr1, %eax");
	REQUIRE(get_asm_line(1, 12) == "addl k, %eax");
	REQUIRE(get_asm_line(1, 13) == "movl %eax, pr1");
	REQUIRE(get_asm_line(1, 14) == "movl pr1, %eax");
	REQUIRE(get_asm_line(1, 15) == "addl l, %eax");
	REQUIRE(get_asm_line(1, 16) == "movl %eax, pr1");
	REQUIRE(get_asm_line(1, 17) == "movl pr1, %eax");
	REQUIRE(get_asm_line(1, 18) == "addl n, %eax");
	REQUIRE(get_asm_line(1, 19) == "movl %eax, pr1");
}
