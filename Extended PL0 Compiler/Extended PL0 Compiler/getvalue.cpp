//finished
#include "stdafx.h"
#include "header.h"
#include <string.h>
#include <math.h>

extern enum symbol sym;
extern char tmpsym[];
extern char ch;
extern bool skipgcv;

/*getinvalue()
目的：分析NUM符号的绝对值
条件：sym==NUM
不向下预读，从已有预读中返回整数值
*/

int getintvalue(){
	int i,v=0;
	if(sym == NUM){
		for(i=0;i<strlen(tmpsym);i++)
			v=v*10+tmpsym[i]-'0';
		return v;
	}
	else{		
		printf("sym isn't NUM! Program error!");
		return 0;
	}
}
/*
测试getintvalue()
*/
void testgetintvalue(FILE* fin){
	getch(fin);
	while(ch!=EOF){
		getsym(fin);
		printf("%d\n",getintvalue());
	}
	return;
}
/*
目的：分析小数部分
条件：sym=NUM
返回double值
*/
double getdecimalpart(){
	int i;
	double v=0.0;
	if(sym == NUM){
		for(i=0;i<strlen(tmpsym);i++)
			v=v+pow(0.1,i+1)*(tmpsym[i]-'0');
		return v;
	}
	else{
		printf("sym isn't NUM! Program error!");
		return 0.0;
	}
}
//测试getdecimalpart();
void testgetdecimalpart(FILE* fin){
	getch(fin);
	while(ch!=EOF){
		getsym(fin);
		printf("%.10f\n",getdecimalpart());
	}
	return;
}
/*
分析字符的值
sym == QUOTE
*/
int getcharvalue(FILE *fin){
	int i=0;
	if(sym == QUOTE){
		getsym(fin);
		if(sym == IDENT || sym == NUM){
			if(strlen(tmpsym)==1){
				i=tmpsym[0];
				getsym(fin);
				if(sym == QUOTE){
					getsym(fin);
					return i;
				}
				else{
					error(5);
					skip(&skipgcv,fin);
					return 0;
				}
			}
			else{
				error(5);
				skip(&skipgcv,fin);
				return 0;//常量引号内部只能有一个字母
			}		
		}
		else
			error(5);
			skip(&skipgcv,fin);
			return 0;//引号后内容有误	
	}
	else{
		printf("sym isn't QUOTE! Program error!\n");
		skip(&skipgcv,fin);
		return 0;
	}
}

void testgetcharvalue(FILE* fin){
	getch(fin);
	while(ch!=EOF){
		getsym(fin);
		printf("%c\n",getcharvalue(fin));
	}
	return ;
}
