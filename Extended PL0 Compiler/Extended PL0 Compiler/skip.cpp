#include "stdafx.h"
#include "header.h"

extern symbol sym;
//用布尔表达式来表示要停止跳过的符号
bool skipblock=    (sym == SEMICOLON || sym == PERIOD || sym ==NUL),   
	 skipconstdecl=(sym == VAR || sym == PROC || sym == FUNC || sym == BEGIN || sym ==NUL),
	 skipvardecl=  (sym == FUNC || sym == PROC || sym == BEGIN || sym ==NUL),
	 skipproc=     (sym == FUNC || sym == PROC || sym == BEGIN || sym ==NUL),
	 skipfunc=     (sym == FUNC || sym == PROC || sym == BEGIN || sym ==NUL),
	 skipfpp=      (sym == SEMICOLON || sym == RPARENT || sym ==NUL),//formalparapart()
	 skipstmt=     (sym == SEMICOLON || sym == END || sym == ELSE || sym == COMMA || sym ==NUL),
	 skipgcv=      (sym == COMMA || sym == SEMICOLON || sym ==NUL),  //getcharvalue()
	 skipexpr=     (sym == RPARENT || sym == EQL || sym == NEQ || sym == GTR || sym == GEQ ||
	                sym == LSS || sym == LEQ || sym == TO || sym == DOWNTO || sym == DO || sym == OF||
				    sym == RBRACK || sym ==SEMICOLON || sym == END || sym ==NUL),
	 skipconst=    (sym == COLON || sym == COMMA || sym == SEMICOLON || sym ==NUL),
     skipvar=      (sym == FUNC || sym == PROC || sym == BEGIN || sym ==NUL),
     skipcdt=      (sym == THEN || sym ==NUL),
	 skipcaseelmt= (sym == SEMICOLON || sym == END || sym == NUL);

void refresh(){
	skipblock=    (sym == SEMICOLON || sym == PERIOD || sym ==NUL),   
	 skipconstdecl=(sym == VAR || sym == PROC || sym == FUNC || sym == BEGIN || sym ==NUL),
	 skipvardecl=  (sym == FUNC || sym == PROC || sym == BEGIN || sym ==NUL),
	 skipproc=     (sym == FUNC || sym == PROC || sym == BEGIN || sym ==NUL),
	 skipfunc=     (sym == FUNC || sym == PROC || sym == BEGIN || sym ==NUL),
	 skipfpp=      (sym == SEMICOLON || sym == RPARENT || sym ==NUL),//formalparapart()
	 skipstmt=     (sym == SEMICOLON || sym == END || sym == ELSE || sym == COMMA || sym ==NUL),
	 skipgcv=      (sym == COMMA || sym == SEMICOLON || sym ==NUL),  //getcharvalue()
	 skipexpr=     (sym == RPARENT || sym == EQL || sym == NEQ || sym == GTR || sym == GEQ ||
	                sym == LSS || sym == LEQ || sym == TO || sym == DOWNTO || sym == DO || sym == OF||
				    sym == RBRACK || sym ==SEMICOLON || sym == END || sym ==NUL),
	 skipconst=    (sym == COLON || sym == COMMA || sym == SEMICOLON || sym ==NUL),
     skipvar=      (sym == FUNC || sym == PROC || sym == BEGIN || sym ==NUL),
     skipcdt=      (sym == THEN || sym ==NUL),
	 skipcaseelmt= (sym == SEMICOLON || sym == END || sym == NUL);
}


//语法分析子程序内部出错，跳过整个子程序，不影响下面程序的语法分析。
void skip(bool* skipitem, FILE* fin){
	refresh();
	while(!(*skipitem)){
		getsym(fin);
		refresh();
	}
}