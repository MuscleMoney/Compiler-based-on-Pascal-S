#include "stdafx.h"
#include "header.h"

//指令表
Order CodeTable[CODE_MAX];
//指令计数
int lc;

char Pcode_OUT[PCODEF_MAX][4]={//p指令
	"LDA","LOD","LDI","DIS","e1","e2","e3","e4","FCT","INT","JMP","JPC","SWT","CAS","F1U",
	"F2U","F1D","F2D","MKS","CAL","IDX","IXX","LDB","CPB","LDC","LDR",
	"FLT","RED","WRS","WRW","WRU","HLT","EXP","EXF","LDT","NOT","MUS",
	"WRR","STO","EQR","NER","LSR","LER","GTR","GER","EQL","NEQ","LSS",
	"LER","GRT","GEQ","ORR","ADD","SUB","ADR","SUR","AND","MUL","DIV",
	"MOD","MUR","DIR","RDL","WRL"
};

void emit(Pcodef f){
	if(lc>=CODE_MAX){
		//代码溢出
	}
	else{
		CodeTable[lc].f=f;
		lc+=1;
	}
}

void emit1(Pcodef f, int y){
	if(lc>=CODE_MAX){
		//代码溢出
	}
	else{
		CodeTable[lc].f=f;
		CodeTable[lc].y=y;
		lc+=1;
	}
}

void emit2(Pcodef f,int x,int y){
	if(lc>=CODE_MAX){
		//代码溢出
	}
	else{
		CodeTable[lc].f=f;
		CodeTable[lc].x=x;
		CodeTable[lc].y=y;
		lc+=1;
	}
}

void printcode(){
	int i;
	printf("Code----------\n");
	printf("No.   f     x     y\n");
	for(i=0;i<lc;i++){
		if(CodeTable[i].f<4)
			printf("%-6d%s   %-6d%-6d\n",i,Pcode_OUT[CodeTable[i].f],CodeTable[i].x,CodeTable[i].y);
		else if(CodeTable[i].f<31)
			printf("%-6d%s   -     %-6d\n",i,Pcode_OUT[CodeTable[i].f],CodeTable[i].y);
		else
			printf("%-6d%s   -     -\n",i,Pcode_OUT[CodeTable[i].f]);
	}
}