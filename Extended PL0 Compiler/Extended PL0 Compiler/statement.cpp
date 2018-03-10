#include "stdafx.h"
#include "header.h"
#include <string.h>
#include <malloc.h>

extern symbol sym;
extern bool skipstmt, skipcdt, skipcaseelmt;
extern char tmpsym[];
extern Table table[];
extern BlockTable blockTable[];
extern ArrayTable arrayTable[];
extern int display[];
extern int level;
extern int position(char name[]);
extern int lc;
extern int tmpi;
extern double tmpr;
extern Order CodeTable[];
extern int sc;
extern char fun[];
extern char tmp[];
extern bool rtnflag;

CaseTab casetab[CSMAX];
int exittab[CSMAX];
int cs;
int exit;
enum symbol op;
enum symbol cdtop;


void statement(FILE* fin){
	Item* x,*y;
	x=(Item *)malloc(sizeof(Item));
	y=(Item *)malloc(sizeof(Item));
	int i=0,posi;
	int paracnt,lastpara;
	int lc1,lc2;
	double r=0;
	bool minus =false;
	if(sym == IDENT){
		posi=position(tmpsym);
		strcpy(tmp,tmpsym);
		if(posi==0){
			//��ʶ��������
			error(58);
			skip(&skipstmt,fin);
		}
		getsym(fin);
		x->dt = table[posi].dataType;
		x->ref = table[posi].ref;
		if(table[posi].identType == CONSTANT){
			//���ܸ�������ֵ
			error(52);
			skip(&skipstmt,fin);
		}
		else if(table[posi].identType == VARIABLE || table[posi].identType == FUNCTION){
			if(table[posi].identType == FUNCTION){
				table[posi].ref=display[level];
				emit2((table[posi].normal?Lda:Lod),table[posi].lev+1,0);
				if(strcmpa(fun,tmp)==0){
					rtnflag=true;
				}
			}
			else
				emit2((table[posi].normal?Lda:Lod),table[posi].lev,table[posi].adr);
			if(sym == BECOMES){
				if(table[posi].dataType == ARRAYTYPE){
					//�����ʶ��ȱ���±�
					error(53);
					skip(&skipstmt,fin);
				}
				else{
					getsym(fin);
					expression(fin,y);
					if(y->dt == x->dt){
						emit(Sto);
					}
					else if(y->dt == INTTYPE && x->dt == REALTYPE){
						emit1(Flt,0);
						emit(Sto);
						
					}
					else{
						//��ֵ����������ʹ���
						error(54);
						skip(&skipstmt,fin);
					}
					//printf("This is a var becomes or func return statement\n");
				}
			}
			else if(sym == LBRACK){
				if(table[posi].dataType != ARRAYTYPE){
					//Ӧ������Ԫ�ظ�ֵ
					skip(&skipstmt,fin);
				}
				else{
					x->dt = arrayTable[table[posi].ref].dt;
					getsym(fin);
					expression(fin,y);
					if(y->dt != INTTYPE){
						//�±�����Ӧ������
						error(48);
						skip(&skipstmt,fin);
					}
					else{
						emit1(Idx,x->ref);
						if(sym == RBRACK){
							getsym(fin);
							if(sym == BECOMES){
								getsym(fin);
								expression(fin,y);
								if(y->dt == x->dt){
									emit(Sto);
								}
								else if(y->dt == INTTYPE && x->dt == REALTYPE){
									emit1(Flt,0);
									emit(Sto);
								}
								else{
									//��ֵ����������ʹ���
									error(54);
								}
								//printf("This is a arr element becomes statement\n");
								return ;
							}
							else{
								error(30);
								skip(&skipstmt,fin);
								//Ӧ�Ǹ�ֵ
							}
						}
						else{
							error(13);
							skip(&skipstmt,fin);
							//Ӧ����������
						}
					}
				}
			}
			else{
				//��ֵ����ַ�����
				error(56);
			}
		}
		else if(table[posi].identType == PROCEDURE){
			emit1(Mks,posi);
			lastpara=blockTable[table[posi].ref].lastpara;
			paracnt=posi;
			if(sym == SEMICOLON || sym == END){
				if(lastpara!=paracnt){
					error(49);
					skip(&skipstmt,fin);
					return ;
				}
				else{
					emit1(Cal,blockTable[table[posi].ref].psize-1);
					if(table[posi].lev<level)
						emit2(Dis,table[posi].lev,level);
					return ;
				}
			}
			else if(sym == LPARENT){
				getsym(fin);
				paracnt+=1;
				expression(fin,y);
				if(y->dt != table[paracnt].dataType){
					//���ͼ��
					error(50);
				}
				if(paracnt>lastpara){
						//������������
					error(49);
					skip(&skipstmt,fin);
				}
				while(sym == COMMA){//���뺯������
					getsym(fin);
					paracnt+=1;
					expression(fin,y);
					if(y->dt != table[paracnt].dataType){
						//����������������
						error(50);
					}
					if(paracnt>lastpara){
						//������������
						error(49);
						skip(&skipstmt,fin);
					}
				}
				if(sym ==RPARENT){
					//printf("This is a function factor\n");
					if(paracnt<lastpara){
						error(51);
					//�ٲ���
					}
					emit1(Cal,blockTable[table[posi].ref].psize-1);
					if(table[posi].lev<level)
						emit2(Dis,table[posi].lev,level);
					getsym(fin);
					return ;
				}
				else{
					//ӦΪ������
					error(21);
					skip(&skipstmt,fin);
				}
			}
			else{
				//��ʶ����������
				error(47);
				skip(&skipstmt,fin);
			}
		}
		else{
			//��ʶ����������
			error(47);
			skip(&skipstmt,fin);
		}
	}
/*���Ͽ�ͷ�Ǳ�ʶ�������*/
	else if(sym == IF){//IF���
		getsym(fin);
		condition(fin);
		lc1=lc;
		emit(Jpc);
		if(sym == THEN){
			getsym(fin);
			//������Ӧ����
			statement(fin);
			if(sym == ELSE){
				getsym(fin);
				lc2=lc;
				emit(Jmp);
				CodeTable[lc1].y=lc;
				statement(fin);
				CodeTable[lc2].y=lc;
				//printf("This is a if-else statement\n");
				return;
			}
			CodeTable[lc1].y=lc;
			//printf("This is a if statement\n");
			return;
		}
		else{
			error(23);
			skip(&skipstmt,fin);
			//�˴�Ӧ��then��if��䲻����
		}
	}
	else if(sym == CASE){//������
		cs=0; exit=0;
		getsym(fin);
		expression(fin,x);
		if(x->dt != INTTYPE && x->dt != CHARTYPE){
			//caseԪ����������
			error(57);
		}
		lc1=lc;
		emit(Swt);
		if(sym == OF){
			getsym(fin);
			caseelement(fin);
			while(sym == SEMICOLON){
				getsym(fin);
				caseelement(fin);
			}
			CodeTable[lc1].y=lc;
			for(i=0;i<cs;i++){
				emit1(Cas,casetab[i].val);
				emit1(Cas,casetab[i].lc);
			}
			emit1(Jmp,0);
			for(i=0;i<exit;i++){
				CodeTable[exittab[i]].y=lc;
			}
			if(sym == END){
				getsym(fin);
				//printf("This is a case statement\n");
				return ;
			}
			else{
				error(24);
				skip(&skipstmt,fin);
				//�˴�Ӧ��end,caseδ��������
			}
		}
		else{
			error(25);
			skip(&skipstmt,fin);
			//�˴�Ӧ��of,case������
		}
	}
	else if(sym == BEGIN){//�������
		getsym(fin);
		statement(fin);
		while(sym == SEMICOLON){
			getsym(fin);
			statement(fin);
		}
		if(sym == END){
			getsym(fin);
			//printf("This is a complex statement\n");
			return;
		}
		else{
			error(26);
			skip(&skipstmt,fin);
			//�˴�ӦΪEND���������δ��������
		}
	}
	else if(sym == READ){//�����
		getsym(fin);
		if(sym ==LPARENT){
			getsym(fin);
			if(sym == IDENT){
				//��֤��ʶ��
				posi=position(tmpsym);
				if(posi == 0){
					//��ʶ��������
					error(58);
				}
				if(table[posi].identType!=VARIABLE){
					//��������
					error(59);
				}
				if(table[posi].dataType == NULTYPE || table[posi].dataType == ARRAYTYPE){
					//������������Ͳ�֧��
					error(60);
				}
				x->dt=table[posi].dataType;
				x->ref=table[posi].ref;
				if(table[posi].normal){
					emit2(Lda,table[posi].lev,table[posi].adr);
				}
				else{
					emit2(Lod,table[posi].lev,table[posi].adr);
				}
				switch(x->dt){
				case INTTYPE: emit1(Red,1);break;
				case REALTYPE: emit1(Red,2);break;
				case CHARTYPE: emit1(Red,3);break;
				default:break;
				}
				getsym(fin);
				while(sym == COMMA){
					getsym(fin);
					if(sym == IDENT){
						posi=position(tmpsym);
						if(posi == 0){
							//��ʶ��������
							error(58);
						}
						if(table[posi].identType!=VARIABLE){
							//��������
							error(59);
						}
						if(table[posi].dataType == NULTYPE || table[posi].dataType == ARRAYTYPE){
							//������������Ͳ�֧��
							error(60);
						}
						x->dt=table[posi].dataType;
						x->ref=table[posi].ref;
						if(table[posi].normal){
							emit2(Lda,table[posi].lev,table[posi].adr);
						}
						else{
							emit2(Lod,table[posi].lev,table[posi].adr);
						}
						switch(x->dt){
						case INTTYPE: emit1(Red,1);break;
						case REALTYPE: emit1(Red,2);break;
						case CHARTYPE: emit1(Red,3);break;
						default:break;
						}
						emit(Wrl);
						getsym(fin);
					}
					else{
						//ӦΪ��ʶ��
						error(10);
						skip(&skipstmt,fin);
						//����
					}
				}
				if(sym == RPARENT){
					getsym(fin);
					//printf("This is a READ statement\n");
					return ;
				}
				else{
					//�˴�ӦΪ������
					error(21);
					skip(&skipstmt,fin);
				}
			}
			else{
				//�˴�ӦΪ��ʶ��
				error(10);
				skip(&skipstmt,fin);
			}
		}
		else{
			//�˴�ӦΪ������
			error(37);
			skip(&skipstmt,fin);
		}
	}
	else if(sym == WRITE){//д���
		getsym(fin);
		if(sym == LPARENT){
			getsym(fin);
			if(sym == STRING){
				enterString();
				getsym(fin);
				if(sym ==COMMA){
					getsym(fin);
					emit1(Wrs,sc);
					expression(fin,x);
					if(sym == RPARENT){
						getsym(fin);
						switch(x->dt){
						case INTTYPE: emit1(Wrw,1);break;
						case REALTYPE: emit1(Wrw,2);break;
						case CHARTYPE: emit1(Wrw,3);break;
						default:break;
						}
						/*д����������˼��*/
						//printf("This is a write(string,expression) statement\n");
						emit(Wrl);
						return ;
					}
					else{
						//д���δ����
						error(21);
						skip(&skipstmt,fin);
					}
				}
				else if(sym == RPARENT){
					getsym(fin);
					//printf("This is a write(string) statement\n");
					emit1(Wrs,sc);
					emit(Wrl);
					return ;
				}
				else{
					//�˴�ӦΪ������ д���δ����
					error(21);
					skip(&skipstmt,fin);
				}
			}
			else if(sym == PLUS || sym == MINUS || sym == LPARENT || sym == IDENT || sym == NUM)/*���ʽ��ʼ�ķ���*/{
				expression(fin,x);
				if(sym == RPARENT){
					switch(x->dt){
					case INTTYPE: emit1(Wrw,1);break;
					case REALTYPE: emit1(Wrw,2);break;
					case CHARTYPE: emit1(Wrw,3);break;
					default:break;
					}
					emit(Wrl);
					getsym(fin);
				}
				else{
					//�˴�ӦΪ������
					error(21);
					skip(&skipstmt,fin);
				}
			}
			else{
				//�˴�ӦΪ�ַ�������ʽ��д������
				error(27);
				skip(&skipstmt,fin);
			}
		}
		else{
			//�˴�ӦΪ������
			error(37);
			skip(&skipstmt,fin);
		}
	}
	else if(sym == SEMICOLON || sym ==  END){//�����
		//printf("This is a empty statement\n");
		return;
	}
	else if(sym == FOR){
		getsym(fin);
		if(sym == IDENT){
			posi=position(tmpsym);
			if(posi == 0){
				//δ�ҵ���ʶ��
				error(58);
			}
			if(table[posi].identType!=VARIABLE){
				//��ʶ������ӦΪ����
				error(59);
			}
			if(table[posi].dataType!=INTTYPE){
				//����ӦΪ����
				error(61);
			}
			if(!table[posi].normal){
				//����Ϊ�β�
				error(62);
			}
			getsym(fin);
			emit2(Lda,table[posi].lev,table[posi].adr);
			if(sym == BECOMES){
				getsym(fin);
				expression(fin,x);//���ͼ��
				if(x->dt!=INTTYPE){
					//���ʽ����ӦΪ����
					error(63);
				}
				//������Ӧ����
				if(sym == TO){
					getsym(fin);
					expression(fin,x);//���ͼ��
					if(x->dt!=INTTYPE){
						//���ʽ����ӦΪ����
						error(63);
					}
					lc1=lc;
					emit(F1u);
					if(sym == DO){
						getsym(fin);
						lc2=lc;
						statement(fin);
						emit1(F2u,lc2);
						CodeTable[lc1].y=lc;
						//printf("This is a for statement(to)\n");
						return ;
					}
					else{
						//�˴�ӦΪDO
						error(28);
						skip(&skipstmt,fin);
					}
				}
				else if(sym == DOWNTO){
					getsym(fin);
					expression(fin,x);//���ͼ��
					if(x->dt != INTTYPE){
						//forѭ�����ʽ����ӦΪ����
						error(63);
					}
					lc1=lc;
					emit(F1d);
					if(sym == DO){
						getsym(fin);
						lc2=lc;
						statement(fin);
						emit1(F2d,lc2);
						CodeTable[lc1].y=lc;
						//printf("This is a for statement(downto)\n");
						return ;
					}
					else{
						//�˴�ӦΪDO
						error(28);
						skip(&skipstmt,fin);
					}
				}
				else{
					//�˴�Ӧ��TO��DOWNTO,for �������
					error(29);
					skip(&skipstmt,fin);
				}
			}
			else{
				//�˴�ӦΪ��ֵ����
				error(30);
				skip(&skipstmt,fin);
			}
		}
		else{
			//�˴�ӦΪ��ʶ��
			error(10);
			skip(&skipstmt,fin);
		}
	}
	else{
		//��俪ʼ��������
		error(31);
		skip(&skipstmt,fin);
	}
}

void condition(FILE* fin){
	Item *x ,*y;
	x=(Item *)malloc(sizeof(Item));
	y=(Item *)malloc(sizeof(Item));
	expression(fin,x);
	cdtop=sym;
	getsym(fin);
	expression(fin,y);
	if(x->dt == y->dt && x->dt <=INTTYPE){
		switch(cdtop){
		case EQL: emit(Eql);break;	
		case NEQ: emit(Neq);break;	
		case GTR: emit(Grt);break;	
		case GEQ: emit(Geq);break;	
		case LSS: emit(Lss);break;	
		case LEQ: emit(Leq);break;	
		default: //Ӧ�ǹ�ϵ�����
			skip(&skipcdt,fin);
			error(32);
			break;
		}
	}
	else if(x->dt <= INTTYPE && y->dt == REALTYPE){
		x->dt=REALTYPE;
		emit1(Flt,1);
		switch(cdtop){
		case EQL: emit(Eqr);break;	
		case NEQ: emit(Ner);break;	
		case GTR: emit(Gtr);break;	
		case GEQ: emit(Ger);break;	
		case LSS: emit(Lsr);break;	
		case LEQ: emit(Ler);break;	
		default: //Ӧ�ǹ�ϵ�����
			skip(&skipcdt,fin);
			error(32);
			break;
		}
	}
	else if(x->dt == REALTYPE && y->dt <= INTTYPE){
		y->dt=REALTYPE;
		emit1(Flt,0);
		switch(cdtop){
		case EQL: emit(Eqr);break;	
		case NEQ: emit(Ner);break;	
		case GTR: emit(Gtr);break;	
		case GEQ: emit(Ger);break;	
		case LSS: emit(Lsr);break;	
		case LEQ: emit(Ler);break;	
		default: //Ӧ�ǹ�ϵ�����
			skip(&skipcdt,fin);
			error(32);
			break;
		}
	}
	else{
		//�������ʽ�������ʽ���ʹ���
		error(64);
	}
	x->dt=INTTYPE;
	return;
}

void caseelement(FILE* fin){
	int i;Item *x;
	x=(Item *)malloc(sizeof(Item));
	
	constant(fin,x);
	if(x->dt >INTTYPE){
		//�����Ԫ��ֻ�ܳ������ͻ��ַ��ͳ���
		error(65);
	}
	for(i=0;i<cs;i++){
		if(casetab[i].val == tmpi){
			//�����ظ�
			error(66);
		}
	}
	casetab[cs].val=tmpi;
	casetab[cs].lc=lc;
	cs+=1;
	if(cs>=CSMAX){
		//case��������
		error(72);
	}
	if(sym == COLON){
		getsym(fin);
		statement(fin);
		exittab[exit]=lc;
		exit+=1;
		emit(Jmp);
		return ;
	}
	else{
		//Ӧ��ð��
		skip(&skipcaseelmt,fin);
		error(17);
	}
}

void teststatement(FILE* fin){
	getch(fin);
	getsym(fin);
	statement(fin);
}