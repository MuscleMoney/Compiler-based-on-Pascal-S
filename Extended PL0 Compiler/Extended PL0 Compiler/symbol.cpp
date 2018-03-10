//finished
#include "stdafx.h"
#include "header.h"
#include <memory.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>



int cl=1;//行计数
int cc;//行字符计数
int cn;//总字数
char ch;//预读字符
char tmpsym[MAX_SYM_LENGTH];//单词缓存
char tmpline[MAX_LINE_LENGTH];//行缓存
char tmpstring[MAX_STRING_LENGTH];//字符串缓存
//测试输出用
char outsym[45][10]={"NUL","IDENT", "NUM",
	"CHAR", "INTEGER", "REAL", "STRING",
	"LPARENT", "RPARENT", "LBRACK", "RBRACK",
	"COMMA","COLON", "SEMICOLON", "PERIOD","QUOTE", "BECOMES",
	"PLUS", "MINUS", "TIMES", "DIV",
	"EQL", "NEQ", "GTR", "GEQ", "LSS", "LEQ", 
	"CONST", "VAR", "ARRAY", "PROC", "FUNC",
	"IF", "THEN", "ELSE", "CASE", "OF", "FOR","TO","DOWNTO","DO",
	"BEGIN", "END", "READ", "WRITE"};
//枚举变量symbol声明
enum symbol sym;

void getch(FILE *fin){//已经预读一个字符
	if(ch==EOF){
		fclose(fin);
	}
	else if(ch=='\n'){
		cl++;
		cc=0;
		memset(tmpline,0,MAX_LINE_LENGTH);
	}
	ch=fgetc(fin);
	tmpline[cc++]=ch;
	cn++;
}

int strcmpa(char* Str1, char* Str2){//相等返回0 不相等返回1
	int i,j;
	i=strlen(Str1);
	j=strlen(Str2);
	if(i!=j)
		return 1;
	else{
		for(j=0;j<i;j++){
			if(isdigit(Str1[j])){
				if(Str1[j]!=Str2[j])
					return 1;
			}
			else if(isalpha(Str1[j])){
				if(Str1[j]!=toupper(Str2[j]) && Str1[j]!=tolower(Str2[j])){
					return 1;
				}
			}
		}
		return 0;
	}
}

void getsym(FILE *fin){
	int i=0;
	sym=NUL;
	memset(tmpsym,0,MAX_SYM_LENGTH);
	while(ch==' '|| ch=='\t'|| ch=='\n')
		getch(fin);
	if(isalpha(ch)){
		do{
			tmpsym[i++]=ch;
			getch(fin);
		}while(i<MAX_SYM_LENGTH && (isalpha(ch) || isdigit(ch)));
		if(i>=MAX_SYM_LENGTH){//标识符长度过长，报错，跳过
			error(0);
		}
		switch(strlen(tmpsym)){
		case 1:{
			sym=IDENT;
			break;
			   }
		case 2:{
			if(!strcmp(tmpsym,"if"))
				sym=IF;
			
			else if(!strcmp(tmpsym,"of"))
				sym=OF;			
			else if(!strcmp(tmpsym,"to"))
				sym=TO;
			else if(!strcmp(tmpsym,"do"))
				sym=DO;
			else
				sym=IDENT;
			break;
			   }
		case 3:{
			if(!strcmp(tmpsym,"for"))
				sym=FOR;
			else if(!strcmp(tmpsym,"end"))
				sym=END;
			else if(!strcmp(tmpsym,"var"))
				sym=VAR;
			else
				sym=IDENT;
			break;
			   }
		case 4:{
			if(!strcmp(tmpsym,"char"))
				sym=CHAR;
			else if(!strcmp(tmpsym,"real"))
				sym=REAL;
			else if(!strcmp(tmpsym,"then"))
				sym=THEN;
			else if(!strcmp(tmpsym,"else"))
				sym=ELSE;
			else if(!strcmp(tmpsym,"case"))
				sym=CASE;
			else if(!strcmp(tmpsym,"read"))
				sym=READ;
			else
				sym=IDENT;
			break;
			   }
		case 5:{
			if(!strcmp(tmpsym,"begin"))
				sym=BEGIN;
			else if(!strcmp(tmpsym,"const"))
				sym=CONST;
			else if(!strcmp(tmpsym,"array"))
				sym=ARRAY;
			else if(!strcmp(tmpsym,"write"))
				sym=WRITE;
			else 
				sym=IDENT;
			break;
			   }
		case 6:{
			if(!strcmp(tmpsym,"downto"))
				sym=DOWNTO;
			else
				sym=IDENT;
			break;
			   }
		case 7:{
			if(!strcmp(tmpsym,"integer"))
				sym=INTEGER;
			else
				sym=IDENT;
			break;
			   }
		case 8:{
			if(!strcmp(tmpsym,"function"))
				sym=FUNC;
			else 
				sym=IDENT;
			break;
			   }
		case 9:{
			if(!strcmp(tmpsym, "procedure"))
				sym=PROC;
			else
				sym=IDENT;
			break;
			   }
		default:{
			sym=IDENT;
			break;
				}
		}
	}
	else if(isdigit(ch)){//数字
		sym=NUM;
		do{
			tmpsym[i++]=ch;
			getch(fin);
		}while(i<MAX_NUM_LENGTH && isdigit(ch));
		if(i>=MAX_NUM_LENGTH)//数字长度过长，报错，跳过
			error(1);
	}
	else if(ch == '<'){//小于，小于等于，不等于
		tmpsym[0]=ch;
		getch(fin);
		if(ch == '>'){
			tmpsym[1]=ch;
			sym=NEQ;
			getch(fin);
		}
		if(ch == '='){
			tmpsym[1]=ch;
			sym=LEQ;
			getch(fin);
		}
		else
			sym=LSS;
	}
	else if(ch == '>'){//大于，大于等于
		tmpsym[0]=ch;
		getch(fin);
		if(ch == '='){
			tmpsym[1]=ch;
			sym=GEQ;
			getch(fin);
		}
		else
			sym=GTR;
	}
	else if(ch == ':'){//赋值或冒号
		tmpsym[0]=ch;
		getch(fin);
		if(ch == '='){
			tmpsym[1]=ch;
			sym=BECOMES;
			getch(fin);			
		}
		else
			sym=COLON;
	}
	else if(ch=='\"'){//字符串
		memset(tmpstring,0,MAX_STRING_LENGTH);
		getch(fin);
		while(ch == 32 || ch == 33 || (ch>=35 && ch<=126)){//字符合法，长度不越界
			tmpstring[i++]=ch;
			if(i<MAX_STRING_LENGTH)
				getch(fin);
			else{
				error(2);//字符串长度过长
				break;
			}
		}
		if(ch=='\"'){
			sym=STRING;
			getch(fin);
		}
		else 
			error(3);//字符串未结束
	}
	else{
		tmpsym[0]=ch;
		switch(ch){
		case '+': sym=PLUS; break;
		case '-': sym=MINUS; break;
		case '*': sym=TIMES; break;
		case '/': sym=DIV; break;
		case '(': sym=LPARENT; break;
		case ')': sym=RPARENT; break;
		case '[': sym=LBRACK; break;
		case ']': sym=RBRACK; break;
		case ';': sym=SEMICOLON; break;
		case ',': sym=COMMA; break;
		case '.': sym=PERIOD; break;
		case '=': sym=EQL; break;
		case '\'': sym=QUOTE; break;
		case EOF: sym=NUL; break;
			//非法字符，报错，跳过
		default: sym=NUL; error(4); getch(fin); break;
		}
		getch(fin);
	}
}

void testgetsym(FILE * fin){
	int i=0;
	getch(fin);
		while(ch!=EOF){
			getsym(fin);
			if(sym == STRING){
				printf("%d %s %s\n",i++,outsym[sym],tmpstring);
			}
			else
				printf("%d %s %s\n",i++,outsym[sym],tmpsym);
		}
	return;
}