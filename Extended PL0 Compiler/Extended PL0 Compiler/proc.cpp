#include "stdafx.h"
#include "header.h"
#include <memory.h>
#include <string.h>

extern enum symbol sym;
extern char tmpsym[];
extern char tmpvar[MAX_VAR_NUM][MAX_SYM_LENGTH];
extern void block(FILE* fin);
extern int tc;
extern int bc;
extern int level;
extern int display[];
extern Table table[];
extern BlockTable blockTable[];
extern int lc;
extern bool skipfpp;
extern int errcnt;

int dx;//数据地址分配
int tmptc;//暂存
int psz;
bool rtnflag;
char fun[MAX_SYM_LENGTH];



void formalparapart(FILE* fin){//检查tmpvar循环
	int i=0,j=0;
	bool paraflag=false;
	memset(tmpvar,0,sizeof(tmpvar));
	if(sym== VAR){
		paraflag=true;
		getsym(fin);
	}
	if(sym == IDENT){
		strcpy(tmpvar[i++],tmpsym);
		getsym(fin);
		while(sym == COMMA){
			getsym(fin);
			if(sym == IDENT){
				strcpy(tmpvar[i++],tmpsym);
				getsym(fin);
			}
			else{
				//应是标识符
				error(10);
				skip(&skipfpp,fin);
				//跳过
			}
		}
		if(sym == COLON){
			getsym(fin);
			if(sym == INTEGER){
				//生成相应代码
				getsym(fin);
				//printf("This is a formalparapart of integer\n");
				psz+=i;
				for(j=0;j<i;j++){
					entervar(tmpvar[j],INTTYPE);
					if(paraflag)
						table[tc].normal=false;
				}
				return ;
			}
			else if(sym == REAL){
				getsym(fin);
				//printf("This is a formalparapart of real\n");
				psz+=i;
				for(j=0;j<i;j++){
					entervar(tmpvar[j],REALTYPE);
					if(paraflag)
						table[tc].normal=false;
				}
				return ;
			}
			else if(sym == CHAR){
				getsym(fin);
				//printf("This is a formalparapart of char\n");
				psz+=i;
				for(j=0;j<i;j++){
					entervar(tmpvar[j],CHARTYPE);
					if(paraflag)
						table[tc].normal=false;
				}
			}
			else{
				//应是基本类型
				error(33);
			}
		}
		else{
			//应是冒号
			error(17);
		}
	}
	else{
		//应是标识符
		error(10);
	}	
}

void procedure(FILE* fin){
	psz=0;
	if(sym == PROC){
		getsym(fin);
		if(sym == IDENT){
			enter_b(tmpsym,PROCEDURE);
			enterBlock();
			table[tc].normal=true;
			table[tc].ref=bc;
			getsym(fin);
			if(sym == LPARENT){//含参过程
				level+=1;
				display[level]=bc;
				dx=5;
				getsym(fin);
				formalparapart(fin);
				//生成相应代码
				while(sym == SEMICOLON){
					getsym(fin);
					formalparapart(fin);
					//生成相应代码
				}
				if(sym == RPARENT){
					level-=1;
					getsym(fin);
					if(sym == SEMICOLON){
						getsym(fin);
						//printf("This is a para procedure header!");
						blockTable[bc].lastpara=tc;
						blockTable[bc].psize=psz;
						block(fin);
						emit(Exp);
						return;
					}
					else{
						//应是分号过程首部没有结尾
						error(34);
					}
				}
				else{
					//应是右括号，形式参数表没有结尾
					error(34);
				}
			}
			else if(sym == SEMICOLON){//无参过程
				getsym(fin);
				display[level+1]=bc;
				dx=5;
				//printf("This is a no para procedure header!\n");//到这里是过程首部结束
				blockTable[bc].lastpara=tc;
				blockTable[bc].psize=psz;
				block(fin);
				emit(Exp);
				return ;
			}
			else{
				//应是左括号
				error(37);
			}
		}
		else{
			//应是标识符
			error(10);
		}
	}
	else{
		//应是过程
		error(38);
	}
}

void function(FILE* fin){
	psz=0;
	rtnflag=false;
	if(sym == FUNC){
		getsym(fin);
		if(sym == IDENT){
			enter_b(tmpsym,FUNCTION);
			enterBlock();
			table[tc].normal=true;
			table[tc].ref=bc;
			tmptc=tc;
			strcpy(fun,tmpsym);
			getsym(fin);
			if(sym == LPARENT){
				level+=1;
				display[level]=bc;
				dx=5;
				getsym(fin);
				formalparapart(fin);
				//生成相应代码
				while(sym == SEMICOLON){
					getsym(fin);
					formalparapart(fin);
					//生成相应代码
				}
				if(sym == RPARENT){
					level-=1;
					getsym(fin);
					if(sym == COLON){
						getsym(fin);
						if(sym == INTEGER){
							getsym(fin);
							if(sym == SEMICOLON){
								getsym(fin);
								//printf("This is a integer para func header!\n");
								table[tmptc].dataType=INTTYPE;
								blockTable[bc].lastpara=tc;
								blockTable[bc].psize=psz;
								block(fin);
								emit(Exf);
								if(rtnflag==false)
									error(74);
								return;
							}
							else{
								//应是分号
								error(34);
							}
						}
						else if(sym == REAL){
							getsym(fin);
							if(sym == SEMICOLON){
								getsym(fin);
								//printf("This is a real para func header!\n");
								table[tmptc].dataType=REALTYPE;
								blockTable[bc].lastpara=tc;
								blockTable[bc].psize=psz;
								block(fin);
								emit(Exf);
								if(rtnflag==false)
									error(74);
								return;
							}
							else{
								//应是分号
								error(34);
							}
						}
						else if(sym == CHAR){
							getsym(fin);
							if(sym == SEMICOLON){
								getsym(fin);
								//printf("This is a char para func header!\n");
								table[tmptc].dataType=CHARTYPE;
								blockTable[bc].lastpara=tc;
								blockTable[bc].psize=psz;
								block(fin);
								emit(Exf);
								if(rtnflag==false)
									error(74);
								return;
							}
							else{
							//应是分号
								error(34);
							}
						}
						else{
							//应是基本类型
							error(33);
						}
					}
					else{
						//应是分号，函数首部没有结尾
						error(34);
					}
				}
				else{
					//应是右括号，形式参数表没有结尾
					error(21);
				}
			}
			else if(sym == COLON){//无参函数
				getsym(fin);
				display[level+1]=bc;
				dx=5;
				if(sym == INTEGER){
					getsym(fin);
					if(sym == SEMICOLON){
						getsym(fin);
						//printf("This is a integer no para func header!\n");
						table[tmptc].dataType=INTTYPE;
						blockTable[bc].lastpara=tc;
						blockTable[bc].psize=psz;
						block(fin);
						emit(Exf);
						if(rtnflag==false)
							error(74);
						return;
					}
					else{
							//应是分号
						error(34);
					}
				}
				else if(sym == REAL){
					getsym(fin);
					if(sym == SEMICOLON){
						getsym(fin);
						//printf("This is a real no para func header!\n");
						table[tmptc].dataType=REALTYPE;
						blockTable[bc].lastpara=tc;
						blockTable[bc].psize=psz;
						block(fin);
						emit(Exf);
						if(rtnflag==false)
							error(74);
						return;
					}
					else{
							//应是分号
						error(34);
					}
				}
				else if(sym == CHAR){
					getsym(fin);
					if(sym == SEMICOLON){
						getsym(fin);
						//printf("This is a char no para func header!\n");
						table[tmptc].dataType=CHARTYPE;
						blockTable[bc].lastpara=tc;
						blockTable[bc].psize=psz;
						block(fin);
						emit(Exf);
						if(rtnflag==false)
							error(74);
						return;
					}
					else{
							//应是分号
						error(34);
					}
				}
				else{
					//应是基本类型
					error(33);
				}
			}
			else{
				//声明首部错误
				error(40);
			}
		}
		else{
			//应是标识符
			error(10);
		}
	}
	else{
		//应是函数
		error(39);
	}
	return ;
}

void block(FILE* fin){
	int prt,prb;
	prt=tc-blockTable[bc].psize;
	level+=1;
	if(level == 1)
		dx=5;
	if(level>=LEVEL_MAX){
		//层次越界
	}
	prb=bc;
	table[prt].ref=prb;
	if(sym == CONST){
		constdecl(fin);
	}
	if(sym == VAR){
		vardecl(fin);
	}
	blockTable[prb].vsize=dx;
	while(sym == PROC || sym == FUNC){
		if(sym == PROC){
			procedure(fin);
		}
		else if(sym == FUNC){
			function(fin);
		}
		if(sym == SEMICOLON){
			getsym(fin);
		}
		else{
			//应是分号，过程或函数说明没结尾
			error(41);
		}
	}
	table[prt].adr=lc;//???
	if(sym == BEGIN){
		getsym(fin);
		statement(fin);
		while(sym == SEMICOLON){
			getsym(fin);
			statement(fin);
		}
		if(sym == END){
			getsym(fin);
			//printf("This is a block\n");
			level-=1;
			return ;
		}
		else{
			//此处应是end
			error(24);
		}
	}
	else{
		//应为begin
		error(35);
	}	
}

void program(FILE* fin){
	getch(fin);
	getsym(fin);
	block(fin);
	if(sym == PERIOD){
		emit(Hlt);
		printf("Program ended!\n");
		printf("Error number: %d\n",errcnt);
		printf("\n");
		return ;
	}
	else{
		//程序未结束
		error(36);
	}
}