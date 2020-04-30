#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "symtab.h"
#include "lex.h"
#include "parse.h"
#include "ast.h"

TEST_CASE ("symtab","check_get"){
	struct ast *tree = parse_test("nod.pas");
	symtab s(tree);
	struct tabnode* n1 = s.get("k");
	REQUIRE(n1 != NULL);
	REQUIRE(n1->level == 0);
	REQUIRE(n1->sublevel == 0);
	REQUIRE(n1->type == TC_T_INT);
	REQUIRE(n1->is_arr == false);
	struct tabnode* n2 = s.get("a1");
	REQUIRE(n2 != NULL);
	REQUIRE(n2->level == 1);
	REQUIRE(n2->sublevel == 0);
	REQUIRE(n2->type == TC_T_REAL);
	REQUIRE(n2->is_arr == false);
	struct tabnode* n3 = s.get("arr");
	REQUIRE(n3->level == 0);
	REQUIRE(n3->sublevel == 0);
	REQUIRE(n3->type == TC_T_INT);
	REQUIRE(n3->is_arr == true);
	REQUIRE(n3->arr_ub == -11);
	REQUIRE(n3->arr_lb == 10);
	struct tabnode* n4 = s.get("azazaz");
	REQUIRE(n4 == NULL);
}