#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "lex.h"

//std::vector<Token> test_v;

/*TEST_CASE("lex", "check_return"){
	REQUIRE(Lex("nod.pas", test_v) == true);
	REQUIRE(Lex("bad_nod.pas", test_v) == false);
}*/

//char *test = '//';

TEST_CASE ("NumSetter", "check_modset"){
	char test1[3] = "//";
	char test2[3] = ":=";
	char test3[2] = "*";
	char test4[2] = "\'";
	char test5[2] = "a";
	char *pos = test1;
	REQUIRE(ModSetter(pos, MODE_OTHER) == MODE_COMMENT);
	pos = test2;
	REQUIRE(ModSetter(pos, MODE_OTHER) == MODE_SYM2);
	pos = test3;
	REQUIRE(ModSetter(pos, MODE_OTHER) == MODE_SYM1);
	pos = test4;
	REQUIRE(ModSetter(pos, MODE_OTHER) == MODE_STRING);
	pos = test5;
	REQUIRE(ModSetter(pos, MODE_OTHER) == MODE_OTHER);
}

TEST_CASE ("IdCheck", "check_id"){
	char test1[] = "_sdsd90ii";
	char test2[] = "90er";
	char *pos = test1;
	REQUIRE(IdCheck(pos) == true);
	pos = test2;
	REQUIRE(IdCheck(pos) == false);
}

TEST_CASE ("NumCheck", "check_num"){
	char test1[] = "$AD2";
	char test2[] = "$FAQ";
	char test3[] = "%010101";
	char test4[] = "%010102";
	char test5[] = "104";
	char test6[] = "1000F";
	char test7[] = "&88";
	char test8[] = "&18";
	char *pos = test1;
	REQUIRE(NumCheck(pos) == true);
	pos = test2;
	REQUIRE(NumCheck(pos) == false);
	pos = test3;
	REQUIRE(NumCheck(pos) == true);
	pos = test4;
	REQUIRE(NumCheck(pos) == false);
	pos = test5;
	REQUIRE(NumCheck(pos) == true);
	pos = test6;
	REQUIRE(NumCheck(pos) == false);
	pos = test7;
	REQUIRE(NumCheck(pos) == false);
	pos = test8;
	REQUIRE(NumCheck(pos) == false);
}

TEST_CASE ("LexAdd", "check_add"){
	char test1[] = "var";
	char test2[] = "*";
	char test3[] = "uses";
	char test4[] = "string";
	char test5[] = "kek";
	char test6[] = "0azazaz0";
	char *pos = test1;
	REQUIRE(LexAdd(pos) == TC_VAR);
	pos = test2;
	REQUIRE(LexAdd(pos) == TC_ASTER);
	pos = test3;
	REQUIRE(LexAdd(pos) == TC_USES);
	pos = test4;
	REQUIRE(LexAdd(pos) == TC_T_STRING);
	pos = test5;
	REQUIRE(LexAdd(pos) == TC_ID);
	pos = test6;
	REQUIRE(LexAdd(pos) == TC_UNKNOWN);
}

TEST_CASE ("GetNextToken", "check_token"){
	std::ifstream fin;
	fin.open("utestsite.pas");
	LexInit(&fin);
	Token *t = GetNextToken(&fin);
	REQUIRE(t->row == 1);
	REQUIRE(t->Lexeme == "var");
	REQUIRE(t->TokenClass == TC_VAR);
	t = GetNextToken(&fin);
	REQUIRE(t->row == 1);
	REQUIRE(t->Lexeme == "\0");
	REQUIRE(t->TokenClass == TC_ENDLINE);
	t = GetNextToken(&fin);
	REQUIRE(t->row == 2);
	REQUIRE(t->Lexeme == "k");
	REQUIRE(t->TokenClass == TC_ID);
	t = GetNextToken(&fin);
	REQUIRE(t->row == 2);
	REQUIRE(t->Lexeme == ",");
	REQUIRE(t->TokenClass == TC_COMMA);
}