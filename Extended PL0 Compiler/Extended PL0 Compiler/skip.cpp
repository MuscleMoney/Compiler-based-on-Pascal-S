#include "stdafx.h"
#include "header.h"

extern symbol sym;
//�ò������ʽ����ʾҪֹͣ�����ķ���
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


//�﷨�����ӳ����ڲ��������������ӳ��򣬲�Ӱ�����������﷨������
void skip(bool* skipitem, FILE* fin){
	refresh();
	while(!(*skipitem)){
		getsym(fin);
		refresh();
	}
}