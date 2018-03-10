// Extended PL0 Compiler.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "header.h"
#include <string.h>
#include <malloc.h>

//����
extern int cl;
extern int cc;
extern char ch;
extern enum symbol sym;
extern char tmpsym[];//���ʻ���
extern char tmpline[];//�л���
extern char tmpstring[];//�ַ�������
extern char outsym[][10];
extern int errcnt;
char path[81];



int _tmain(int argc, _TCHAR* argv[])
{
	int i=0;
	
	Item *x;
	x=(Item *)malloc(sizeof(Item));

	FILE *fin;
	printf("�������ļ�·��:");
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

