#define CATCH_CONFIG_MAIN
#include "catch.hpp"
//#include "symtab.h"
//#include "lex.h"
#include "parse.h"
#include "ast.h"
#include "sem.h"



TEST_CASE ("error","check_error"){
	struct ast *tree = parse_test("badsem.pas", false);
	symtab s(tree);
	tree = sem_analyzer(tree, &s);
	REQUIRE(tree == NULL);
}

TEST_CASE ("sem","check_sem"){
	struct ast *tree = parse_test("utestsite.pas", false);
	symtab s(tree);
	tree = sem_analyzer(tree, &s);
	REQUIRE(tree != NULL);
	struct ast *ptr1 = tree->nodes[1].ptr_n->nodes[0].ptr_n->nodes[0].ptr_n;
	int ret = sem_id(ptr1, 0, 0, 0);
	REQUIRE(ret == TC_T_REAL);
	ptr1->nodes[0].ptr_t->Lexeme = "kek";
	ret = sem_id(ptr1, 0, 0, 0);
	REQUIRE(ret == -1);
}