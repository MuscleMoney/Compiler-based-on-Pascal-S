#include "stdafx.h"
#include "header.h"
#include <string.h>
#include <memory.h>
#include <malloc.h>

extern symbol sym;
extern char tmpsym[]; 
extern char ch;
extern bool skipconst,skipconstdecl,skipvar,skipvardecl;

extern int tc;
extern Table table[];
extern int rcc;
extern void enterReal(double r);




//�﷨����	ȫ�ֱ���
//vardecl ȫ�ֱ���
char tmpvar[MAX_VAR_NUM][MAX_SYM_LENGTH];
char tmp[MAX_SYM_LENGTH];
int tmpi;
double tmpr;

/*
����˵�����ַ���
sym==CONST
*/

void constant(FILE* fin, Item* x){
	tmpi=0;tmpr=0;
	bool minus=false;
	bool isreal=false;
	if(sym == PLUS || sym == MINUS){
		minus = (sym==MINUS ? true : false);
		getsym(fin);
	}
	if(sym == NUM){
		tmpi=getintvalue();//
		getsym(fin);
		if(sym == PERIOD){
			getsym(fin);
			if(sym == NUM){
				tmpr=getdecimalpart();
				tmpr+=tmpi;
				if(minus)
					tmpr=-tmpr;
				//printf("const: %s real %f\n",tmp,tmpr);
				x->dt = REALTYPE;
				x->ref=0;
				getsym(fin);
				return ;
			}
			else{
				error(6);
				skip(&skipconst,fin);
				return ;
			}
		}
		else if(sym == COMMA || sym == SEMICOLON || sym == COLON){
			if(minus)
				tmpi=-tmpi;
			//printf("const: %s integer %d\n",tmp,tmpi);
			x->dt = INTTYPE;
			return ;
		}
		else{
			error(7);
			skip(&skipconst,fin);
			return ;
		}	
	}
	else if(sym == QUOTE){
		tmpi=getcharvalue(fin);
		//�Ƿ���Ҫ���i?
		if(!((tmpi>=48 && tmpi<=57)||
			 (tmpi>=65 && tmpi<=90)||
			 (tmpi>=97 && tmpi<=122))){
				 //�ַ��������Ϸ�
				 error(42);
				 skip(&skipconst,fin);
		}
		//printf("const: %s char %c %d\n",tmp,tmpi,tmpi);
		x->dt = CHARTYPE;
		x->ref=0;
		return ;
	}
	else{
		error(8);
		skip(&skipconst,fin);
		return ;//�����ַ���������
	}
}

void constdecl(FILE* fin){
	Item *x;
	x=(Item *)malloc(sizeof(Item));
	if(sym==CONST){
		getsym(fin);
		if(sym == IDENT){
			memset(tmp,0,MAX_SYM_LENGTH);
			strcpy(tmp,tmpsym);//�ݴ泣����ʶ����Ϣ
			getsym(fin);
			if(sym == EQL){
				getsym(fin);
				if(sym == PLUS ||sym == MINUS ||sym == QUOTE ||sym ==NUM){
					constant(fin,x);
					enter_b(tmp,CONSTANT);
					if(x->dt == REALTYPE){
						enterReal(tmpr);
						table[tc].dataType=x->dt;
						table[tc].adr=rcc;
					}
					else{
						table[tc].dataType=x->dt;
						table[tc].adr=tmpi;
					}
				}
				else{
					//�����ַ���������
					error(8);
					skip(&skipconstdecl,fin);
				}
			}
			else{
				//�˴�ӦΪ�Ⱥ�
				error(9);
				//skip;
				skip(&skipconstdecl,fin);
			}
		}
		while(sym == COMMA){
			getsym(fin);
			if(sym == IDENT){
				strcpy(tmp,tmpsym);//�ݴ泣����ʶ����Ϣ
				getsym(fin);
				if(sym == EQL){
					getsym(fin);
					if(sym == PLUS ||sym == MINUS ||sym == QUOTE ||sym ==NUM){
						constant(fin,x);
						enter_b(tmp,CONSTANT);
						if(x->dt == REALTYPE){
							enterReal(tmpr);
							table[tc].dataType=x->dt;
							table[tc].adr=rcc;
						}
						else{
							table[tc].dataType=x->dt;
							table[tc].adr=tmpi;
						}
					}
					else{
						//�����ַ���������
						error(8);
						//skip;
						skip(&skipconstdecl,fin);
					}
				}
				else{
					//�˴�ӦΪ�Ⱥ�
					error(9);
					skip(&skipconstdecl,fin);
					//skip;
				}
			}
			else{
				//�˴�ӦΪ��ʶ��
				error(10);
				skip(&skipconstdecl,fin);
				//skip;
			}
		}
		if(sym == SEMICOLON){
			getsym(fin);
			//printf("This is a constdecl\n");
			return ;
		}
		else{
			//���������쳣����
			error(11);
			skip(&skipconstdecl,fin);
		}
	}
	else{
		printf("sym isn't CONST! Program error!");
	}
}

void variable(FILE* fin){//ǰ��������sym==IDENT
	int t=0,j=0,k=0;
	memset(tmpvar,MAX_VAR_NUM,sizeof(char[MAX_SYM_LENGTH]));
	strcpy(tmpvar[k++],tmpsym);
	getsym(fin);
	while(sym == COMMA){
		getsym(fin);
		if(sym == IDENT){
			strcpy(tmpvar[k++],tmpsym);
			getsym(fin);
		}
		else{
			error(10);
			skip(&skipvar,fin);
			break;
		}
	}
	if(sym == COLON){
		getsym(fin);
		if(sym == INTEGER){
			getsym(fin);
			for(j=0;j<k;j++){
				//printf("vardecl: integer %s\n",tmpvar[j]);
				//��¼���ű� var integer tmpvar[j]
				entervar(tmpvar[j],INTTYPE);
			}
			return ;
		}
		else if(sym == REAL){
			getsym(fin);
			for(j=0;j<k;j++){
				//printf("vardecl: real %s\n",tmpvar[j]);
				//��¼���ű� var real tmpvar[j]
				entervar(tmpvar[j],REALTYPE);
			}
			return ;
		}
		else if(sym == CHAR){
			getsym(fin);
			for(j=0;j<k;j++){
				//printf("vardecl: char %s\n",tmpvar[j]);
				//��¼���ű� var char tmpvar[j]
				entervar(tmpvar[j],CHARTYPE);
			}
			return ;
		}
		else if(sym == ARRAY){
			getsym(fin);
			if(sym == LBRACK){
				getsym(fin);
				if(sym == NUM){
					t=getintvalue();
					if(t == 0){
						//���鳤�Ȳ���Ϊ��
						error(43);
						skip(&skipvar,fin);
					}
					getsym(fin);
					if(sym == RBRACK){
						getsym(fin);
						if(sym == OF){
							getsym(fin);
							if(sym == INTEGER){
								getsym(fin);
								for(j=0;j<k;j++){
									//printf("vardecl: integer_array %s[%d]\n",tmpvar[j],t);
									//��¼���ű� var integer_array tmpvar[j] length:t
									enterArray(tmpvar[j],INTTYPE,t);
								}
								return ;
							}
							else if(sym == REAL){
								getsym(fin);
								for(j=0;j<k;j++){
									//printf("vardecl: real_array %s[%d]\n",tmpvar[j],t);
									//��¼���ű� var real_array tmpvar[j] length:t
									enterArray(tmpvar[j],REALTYPE,t);
								}
								return ;
							}
							else if(sym == CHAR){
								getsym(fin);
								for(j=0;j<k;j++){
									//printf("vardecl: char_array %s[%d]\n",tmpvar[j],t);
									//��¼���ű� var char tmpvar[j] length:t
									enterArray(tmpvar[j],CHARTYPE,t);
								}
								return ;
							}
							else{
								//����������ʹ���
								skip(&skipvar,fin);
								error(12);
							}
						}
						else{
							//ӦΪOF
							skip(&skipvar,fin);
							error(25);
						}
					}
					else{
						//ӦΪ']'
						skip(&skipvar,fin);
						error(13);
					}
				}
				else{
					//���鳤��ӦΪNUM
					skip(&skipvar,fin);
					error(14);
				}
			}
			else{
				//Ӧ��'['
				skip(&skipvar,fin);
				error(15);
			}
		}
		else{
			//�������ʹ���
			skip(&skipvar,fin);
			error(16);
		}
	}
	else{
		//ӦΪð��
		skip(&skipvar,fin);
		error(17);
	}
}
/*�ص����*/
void vardecl(FILE* fin){//ǰ��������sym == VAR
	if(sym == VAR){
		getsym(fin);
		do{
			if(sym == IDENT){
				variable(fin);
				if(sym ==SEMICOLON)
					getsym(fin);
				else{
					//��������Ӧ���ԷֺŽ�β
					skip(&skipvardecl,fin);
					error(18);
				}
			}
			else{
				//�˴�ӦΪ��ʶ��
				skip(&skipvardecl,fin);
				error(10);
			}
		}while(sym == IDENT);
		if(sym == PROC || sym == FUNC || sym == BEGIN){
			//printf("vardecl ended!\n");
			return ;
		}
		else{
			//������������������
			error(19);
			skip(&skipvardecl,fin);
		}
	}
	else{
		printf("Sym isn't VAR!Program error!\n");
	}
}

void testconstdecl(FILE* fin){
	getch(fin);
	getsym(fin);
	constdecl(fin);
}

void testvardecl(FILE* fin){
	getch(fin);
	getsym(fin);
	vardecl(fin);
}