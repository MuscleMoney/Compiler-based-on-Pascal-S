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

int dx;//���ݵ�ַ����
int tmptc;//�ݴ�
int psz;
bool rtnflag;
char fun[MAX_SYM_LENGTH];



void formalparapart(FILE* fin){//���tmpvarѭ��
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
				//Ӧ�Ǳ�ʶ��
				error(10);
				skip(&skipfpp,fin);
				//����
			}
		}
		if(sym == COLON){
			getsym(fin);
			if(sym == INTEGER){
				//������Ӧ����
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
				//Ӧ�ǻ�������
				error(33);
			}
		}
		else{
			//Ӧ��ð��
			error(17);
		}
	}
	else{
		//Ӧ�Ǳ�ʶ��
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
			if(sym == LPARENT){//���ι���
				level+=1;
				display[level]=bc;
				dx=5;
				getsym(fin);
				formalparapart(fin);
				//������Ӧ����
				while(sym == SEMICOLON){
					getsym(fin);
					formalparapart(fin);
					//������Ӧ����
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
						//Ӧ�ǷֺŹ����ײ�û�н�β
						error(34);
					}
				}
				else{
					//Ӧ�������ţ���ʽ������û�н�β
					error(34);
				}
			}
			else if(sym == SEMICOLON){//�޲ι���
				getsym(fin);
				display[level+1]=bc;
				dx=5;
				//printf("This is a no para procedure header!\n");//�������ǹ����ײ�����
				blockTable[bc].lastpara=tc;
				blockTable[bc].psize=psz;
				block(fin);
				emit(Exp);
				return ;
			}
			else{
				//Ӧ��������
				error(37);
			}
		}
		else{
			//Ӧ�Ǳ�ʶ��
			error(10);
		}
	}
	else{
		//Ӧ�ǹ���
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
				//������Ӧ����
				while(sym == SEMICOLON){
					getsym(fin);
					formalparapart(fin);
					//������Ӧ����
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
								//Ӧ�Ƿֺ�
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
								//Ӧ�Ƿֺ�
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
							//Ӧ�Ƿֺ�
								error(34);
							}
						}
						else{
							//Ӧ�ǻ�������
							error(33);
						}
					}
					else{
						//Ӧ�Ƿֺţ������ײ�û�н�β
						error(34);
					}
				}
				else{
					//Ӧ�������ţ���ʽ������û�н�β
					error(21);
				}
			}
			else if(sym == COLON){//�޲κ���
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
							//Ӧ�Ƿֺ�
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
							//Ӧ�Ƿֺ�
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
							//Ӧ�Ƿֺ�
						error(34);
					}
				}
				else{
					//Ӧ�ǻ�������
					error(33);
				}
			}
			else{
				//�����ײ�����
				error(40);
			}
		}
		else{
			//Ӧ�Ǳ�ʶ��
			error(10);
		}
	}
	else{
		//Ӧ�Ǻ���
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
		//���Խ��
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
			//Ӧ�Ƿֺţ����̻���˵��û��β
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
			//�˴�Ӧ��end
			error(24);
		}
	}
	else{
		//ӦΪbegin
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
		//����δ����
		error(36);
	}
}