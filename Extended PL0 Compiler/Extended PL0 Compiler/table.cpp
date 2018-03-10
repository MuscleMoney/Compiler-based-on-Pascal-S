#include "stdafx.h"
#include "header.h"
#include <string.h>

extern symbol sym;
extern char tmpsym[];
extern char tmpstring[];

 //符号表
int tc;
Table table[TABLE_MAX];
//数组表
int ac;
ArrayTable arrayTable[ARRAY_MAX];
//分程序表
int bc;
BlockTable blockTable[BLOCK_MAX];
//字符常量表
int sc;
char stringTable[STRING_MAX][MAX_STRING_LENGTH];
//实常量表
int rcc;
double realConstTable[REAL_CONST_MAX];

//层次
int level;
int display[LEVEL_MAX];

//
extern int position(char name[]);
extern int dx;

void enter(char name[], IdentType idt, DataType dt,int adr){
	tc+=1;
	if(tc>= TABLE_MAX){
		//符号表溢出
		error(67);
		return ;
	}
	else{
		strcpy(table[tc].name,name);
		table[tc].link=tc-1;
		table[tc].identType=idt;
		table[tc].dataType=dt;
		table[tc].ref=0;
		table[tc].normal=true;
		table[tc].lev=0;
		table[tc].adr=adr;
		return ;
	}
}

void enter_b(char name[], IdentType idt){
	int j,l;
	if(tc>=TABLE_MAX){
		//符号表溢出
		error(68);
		return ;
	}
	else{
		strcpy(table[0].name,name);
		j=blockTable[display[level]].last;
		l=j;
		while(strcmpa(table[j].name,name)!=0)
			j=table[j].link;
		if(j!=0 && level==table[j].lev){
			//标识符重复定义
			error(68);
		}
		else{
			tc+=1;
			strcpy(table[tc].name,name);
			table[tc].link=l;
			table[tc].identType=idt;
			table[tc].dataType=NULTYPE;
			table[tc].ref=0;
			table[tc].lev=level;
			table[tc].adr=0;
			table[tc].normal=false;
			blockTable[display[level]].last=tc;
		}
	}
}

int position(char name[]){
	int i,j;
	i=level;
	strcpy(table[0].name,name);
	do{
		j=blockTable[display[i]].last;
		while(strcmpa(table[j].name,name)){
			j=table[j].link;
		}
		i=i-1;
	}while(i>=0 && j==0);
	return j;
}

void entervar(char name[], DataType dt){
	if(tc>=TABLE_MAX){
		//符号表溢出
		error(67);
	}
	else{
		enter_b(name,VARIABLE);
		table[tc].dataType=dt;
		table[tc].adr=dx;
		table[tc].normal=true;
		dx+=1;
	}
}

void enterBlock(){
	bc+=1;
	if(bc>=BLOCK_MAX){
		//分程序表溢出
		error(69);
		return;
	}
	else{
		blockTable[bc].last=0;
		blockTable[bc].lastpara=0;
		return ;
	}
}

void enterArray(char name[], DataType dt, int size){
	if(ac>= ARRAY_MAX){
		//数组溢出
		error(70);
	}
	else{
		enter_b(name,VARIABLE);
		ac+=1;
		arrayTable[ac].dt=dt;
		arrayTable[ac].size=size;
		table[tc].dataType=ARRAYTYPE;
		table[tc].adr=dx;
		table[tc].normal=true;
		table[tc].ref=ac;
		dx+=size;
	}
}

void enterReal(double r){
	rcc+=1;
	if(rcc>=REAL_CONST_MAX){
		//实常量表溢出
		error(71);
	}
	else{
		realConstTable[rcc]=r;
	}
}

void enterString(){
	sc+=1;
	if(sc>=STRING_MAX){
		error(73);
	}
	strcpy(stringTable[sc],tmpstring);
}

void printtable(){
	int i;
	printf("Table----------\n");
	printf("No.    name            link   ident  data   ref    nrm    lev    adr\n");
	for(i=0;i<=tc;i++){
		printf("%-7d%-16s%-7d%-7d%-7d%-7d%-7d%-7d%-7d \n",
			i,table[i].name,table[i].link,table[i].identType,table[i].dataType,table[i].ref,table[i].normal,table[i].lev,table[i].adr);
	}
	printf("BlockTable----------\n");
	printf("No.  last  lastpara psze  vsze\n");
	for(i=0;i<=bc;i++){
		printf("%-5d%-6d%-9d%-6d%-4d  \n",i,blockTable[i].last,blockTable[i].lastpara,blockTable[i].psize,blockTable[i].vsize);
	}
	printf("ArrayTable----------\n");
	printf("No.   DataType Size\n");
	for(i=0;i<=ac;i++){
		printf("%-6d%-9d%-4d\n",i,arrayTable[i].dt,arrayTable[i].size);
	}
	printf("Rconst----------\n");
	printf("No.   value\n");
	for(i=0;i<=rcc;i++)
		printf("%-6d%.8f\n",i,realConstTable[i]);
	printf("StringTable----------\n");
	printf("No.   String\n");
	for(i=0;i<=sc;i++){
		printf("%-6d%s\n",i,stringTable[i]);
	}
}

