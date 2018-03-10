// Extended PL0 Compiler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "header.h"
#include <string.h>
#include <malloc.h>

//引用
extern int cl;
extern int cc;
extern char ch;
extern enum symbol sym;
extern char tmpsym[];//单词缓存
extern char tmpline[];//行缓存
extern char tmpstring[];//字符串缓存
extern char outsym[][10];
extern int errcnt;
char path[81];



int _tmain(int argc, _TCHAR* argv[])
{
	int i=0;
	
	Item *x;
	x=(Item *)malloc(sizeof(Item));

	FILE *fin;
	printf("请输入文件路径:");
	scanf("%s",path);
	fin=fopen(path,"r");
	if(fin == NULL){
		printf("Wrong file path!\n");
		return 0;
	}
	else{
		program(fin);
		printtable();
		printcode();
		if(errcnt != 0){
			printf("Compiled with errors!\n");
		}
		else{
			printf("Compiled finished!\n");
			printf("Program area-----------------------\n");
			printf("\n");
			interpret();
		}
		
	}
	return 0;
}

