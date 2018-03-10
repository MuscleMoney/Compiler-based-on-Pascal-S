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
Ŀ�ģ�����NUM���ŵľ���ֵ
������sym==NUM
������Ԥ����������Ԥ���з�������ֵ
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
����getintvalue()
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
Ŀ�ģ�����С������
������sym=NUM
����doubleֵ
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
//����getdecimalpart();
void testgetdecimalpart(FILE* fin){
	getch(fin);
	while(ch!=EOF){
		getsym(fin);
		printf("%.10f\n",getdecimalpart());
	}
	return;
}
/*
�����ַ���ֵ
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
				return 0;//���������ڲ�ֻ����һ����ĸ
			}		
		}
		else
			error(5);
			skip(&skipgcv,fin);
			return 0;//���ź���������	
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
