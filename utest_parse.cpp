#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "lex.h"
#include "parse.h"
#include "ast.h"

TEST_CASE ("parse_test1", "check_goodvar"){
	struct ast *tree = parse_test("hello.pas");
	REQUIRE(tree != NULL);
	REQUIRE(tree->type == AST_PROG);
	REQUIRE(tree->nodes[0].ptr_n->type == AST_PRE);
	REQUIRE(tree->nodes[1].ptr_n->type == AST_MAIN);
	struct ast *ptr = tree->nodes[1].ptr_n;
	REQUIRE(ptr->nodes[0].ptr_n->type == AST_ASSIGN);
	REQUIRE(ptr->nodes[1].ptr_n->type == AST_CHOICE);
	REQUIRE(ptr->nodes[2].ptr_n->type == AST_REPUN);
	REQUIRE(ptr->nodes[3].ptr_n->type == AST_WHILE);
	REQUIRE(ptr->nodes[4].ptr_n->type == AST_FOR);
}

TEST_CASE ("parse_test2", "check_lexerr"){
	struct ast *tree = parse_test("lexerr.pas");
	REQUIRE(tree == NULL);
}

TEST_CASE ("parse_test3", "check_synterr"){
	struct ast *tree = parse_test("synterr.pas");
	REQUIRE(tree == NULL);
}

TEST_CASE ("expr_insert", "check_expr"){
	struct ast *tree = ast_create(0);
	REQUIRE(tree != NULL);
	node_insert(tree, NULL, 0);
	char sendbuf[] = "2+2;";
	expr_insert(tree->nodes[0].ptr_n, sendbuf);
	REQUIRE(tree->nodes[0].ptr_n->type == AST_BINOP_PLUS);
	struct ast *ptr = tree->nodes[0].ptr_n;
	REQUIRE(ptr->nodes[0].ptr_t->TokenClass == TC_NUM);
	REQUIRE(ptr->nodes[0].ptr_t->Lexeme == "2");
	REQUIRE(ptr->nodes[1].ptr_t->TokenClass == TC_NUM);
	REQUIRE(ptr->nodes[1].ptr_t->Lexeme == "2");
}