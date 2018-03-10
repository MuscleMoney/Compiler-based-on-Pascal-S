#include "stdafx.h"
#include "header.h"
#include <string.h>
#include <malloc.h>

extern symbol sym;
extern char tmpsym[];
extern char tmp[];
extern bool skipexpr;
extern Table table[];
extern BlockTable blockTable[];
extern ArrayTable arrayTable[];
extern int level;
extern int rcc;
extern void enterReal(double r);

extern void expression(FILE* fin, Item* x);
extern void term(FILE* fin, Item* x);
extern void factor(FILE* fin, Item *x);

DataType ResultType(Item* x, Item* y){
	if(x->dt > REALTYPE || y->dt > REALTYPE){
		//��������
		error(44);
		return NULTYPE;
	}
	else if(x->dt ==NULTYPE || y->dt==NULTYPE){
		error(44);
		return NULTYPE;
	}
	else if(x->dt == REALTYPE && y->dt == REALTYPE){
		return REALTYPE;
	}
	else if(x->dt == REALTYPE && y->dt < REALTYPE){
		emit1(Flt,0);
		return REALTYPE;
	}
	else if(y->dt == REALTYPE && x->dt < REALTYPE){
		emit1(Flt,1);
		return REALTYPE;
	}
	else
		return INTTYPE;
}

void expression(FILE* fin, Item* x){ 
	Item* y;
	enum symbol e_op;
	e_op=NUL;
	y=(Item *)malloc(sizeof(Item));
	if(sym == PLUS || sym == MINUS){
		e_op = sym;
		getsym(fin);
	}
	term(fin,x);
	if(e_op!=NUL && x->dt==CHARTYPE)
		x->dt=INTTYPE;
	if(x->dt>REALTYPE){
		error(44);
	}
	if(e_op == MINUS)
		emit(Mus);
	while(sym == PLUS || sym == MINUS){
		e_op=sym;
		getsym(fin);
		term(fin,y);
		if(y->dt>REALTYPE){
		//�������ʹ���
			error(44);
		}
		x->dt = ResultType(x,y);
		if(x->dt <= INTTYPE){
			if(e_op == PLUS)
				emit(Add);
			else
				emit(Sub);
		}
		else if(x->dt == REALTYPE){
			if(e_op == PLUS)
				emit(Adr);
			else
				emit(Sur);
		}
	}	
}

void term(FILE* fin, Item* x){
	Item *y;
	enum symbol t_op;
	t_op=NUL;
	y=(Item *)malloc(sizeof(Item));
	factor(fin,x);
	while(sym == TIMES || sym == DIV){
		t_op=sym;
		getsym(fin);
		factor(fin,y);
		if(t_op == TIMES){
			x->dt=ResultType(x,y);
			if(x->dt <= INTTYPE){
				emit(Mul);
			}
			else if(x->dt == REALTYPE){
				emit(Mur);
			}
			else{
				//��˷�����
				error(45);
			}
		}
		else if(t_op == DIV){
			if(x->dt <= INTTYPE && y->dt == REALTYPE){
				emit1(Flt,1);
				x->dt=REALTYPE;
				emit(Dir);
			}
			else if(x->dt == REALTYPE && y->dt <= INTTYPE){
				emit1(Flt,0);
				y->dt=REALTYPE;
				emit(Dir);
			}
			else if((x->dt == INTTYPE || x->dt == CHARTYPE) && (y->dt == INTTYPE || y->dt == CHARTYPE)){
				emit(Div);
			}
			else if(x->dt == REALTYPE && y->dt == REALTYPE){
				emit(Dir);
			}
			else{
				//���������
				error(46);
			}
		}
	}
	return ;
}

void factor(FILE* fin,Item* x){
	int i=0;int posi,paracnt,lastpara;
	Item* y;
	y=(Item *)malloc(sizeof(Item));
	double r=0;
	char minus=false;
	memset(tmp,0,MAX_SYM_LENGTH);
	if(sym == IDENT){
		strcpy(tmp,tmpsym);
		//���ҷ��ű� //��֤��ʶ�� else skip
		posi=position(tmp);
		if(posi == 0){
			//�Ҳ�����ʶ��
			error(58);
			skip(&skipexpr,fin);
		}
		getsym(fin);
		if(sym == LBRACK){//����Ԫ������ //tmp in arrtable
			if(!(table[posi].identType==VARIABLE && table[posi].dataType == ARRAYTYPE)){
				//��ʶ����������
				error(47);
				skip(&skipexpr,fin);
			}
			x->dt=arrayTable[table[posi].ref].dt;
			x->ref=table[posi].ref;
			if(table[posi].normal)//?
				emit2(Lda,level,table[posi].adr);
			else
				emit2(Lod,level,table[posi].adr);
			getsym(fin);
			expression(fin,y);
			if(y->dt!=INTTYPE){
				//�±�����ӦΪ�޷�������
				error(48);
				//���м��
			}
			emit1(Idx,x->ref);//
			if(sym == RBRACK){
				getsym(fin);
				//���ҷ��ű�
				//������������pcode
				//printf("This is a array element factor��%s[]\n",tmp);
			}
			emit(Ldt);
		}
		else if(sym == LPARENT){//�������� // tmp in functable
			
			if(table[posi].identType != FUNCTION){
				//��ʶ�����ʹ���
				error(47);
			}
			x->dt=table[posi].dataType;
			x->ref=table[posi].ref;
			emit1(Mks,posi);
			lastpara=blockTable[table[posi].ref].lastpara;
			paracnt=posi;
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
				skip(&skipexpr,fin);
			}
			while(sym == COMMA){//���뺯������
				getsym(fin);
				paracnt+=1;
				expression(fin,y);
				if(y->dt!= table[paracnt].dataType){
					//���ͼ��
					//����������������
					error(50);
				}
				if(paracnt>lastpara){
					//������������
					error(49);
					skip(&skipexpr,fin);
				}
			}
			if(sym ==RPARENT){
				//printf("This is a function factor: %s()\n",tmp);
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
				skip(&skipexpr,fin);
			}
		}
		else{//������������ //tmp in consttable or vartable
			//printf("This is a ident factor: %s \n",tmp);
			//������������pcode
			if(table[posi].identType == CONSTANT){
				x->dt=table[posi].dataType;
				x->ref=0;
				if(x->dt==REALTYPE)
					emit1(Ldr,table[posi].adr);
				else
					emit1(Ldc,table[posi].adr);
			}
			else if(table[posi].identType == VARIABLE){
				x->dt=table[posi].dataType;
				x->ref=table[posi].ref;
				if(x->dt>=CHARTYPE && x->dt<=REALTYPE){
					if(table[posi].normal)
						emit2(Lod,table[posi].lev,table[posi].adr);
					else
						emit2(Ldi,table[posi].lev,table[posi].adr);
				}
				else{
					if(table[posi].normal)
						emit2(Lda,table[posi].lev,table[posi].adr);
					else
						emit2(Lod,table[posi].lev,table[posi].adr);
				}
			}
			else if(table[posi].identType == FUNCTION){
				emit1(Mks,posi);
				lastpara=blockTable[table[posi].ref].lastpara;
				paracnt=posi;
				if(lastpara !=posi){
					error(47);
					skip(&skipexpr,fin);
				}
				else{
					emit1(Cal,blockTable[table[posi].ref].psize-1);
					if(table[posi].lev<level)
						emit2(Dis,table[posi].lev,level);
					if(sym == SEMICOLON){
						return ;
					}
					else{
						error(41);
						skip(&skipexpr,fin);
					}
				}
			}
			else{
				//��ʶ����������
				error(47);
				skip(&skipexpr,fin);
			}
		}
	}
	else if(sym == NUM){//�޷����������޷���ʵ��
		i=getintvalue();
		getsym(fin);
		if(sym == PERIOD){//ʵ������
			getsym(fin);
			if(sym == NUM){
				r=getdecimalpart();
				r+=i;
				getsym(fin);
				//printf("This is a real factor: %f\n",r);
				enterReal(r);
				emit1(Ldr,rcc);
				x->dt=REALTYPE;
				x->ref=0;
				return ;
			}
		}
		else{//��������
			//printf("This is a integer factor: %d\n",i);	
			x->dt=INTTYPE;
			x->ref=0;
			emit1(Ldc,i);
		}
	}
	else if(sym == LPARENT){//'('<���ʽ>')'
		getsym(fin);
		expression(fin,x);
		if(sym == RPARENT){
			getsym(fin);
			//printf("This is a (expression) factor\n");
			return;
		}
		else{
			//ӦΪ������
			error(21);
			skip(&skipexpr,fin);
			return;
		} 
	}
	else{
		//�����ַ����ʹ��� skip
		error(22);
		skip(&skipexpr,fin);
		return;
	}
}

void testfactor(FILE* fin){
	Item *x;
	x=(Item *)malloc(sizeof(Item));
	getch(fin);
	getsym(fin);
	factor(fin,x);
}

void testterm(FILE* fin){
	Item *x;
	x=(Item *)malloc(sizeof(Item));
	getch(fin);
	getsym(fin);
	term(fin,x);
}

void testexpression(FILE* fin){
	Item *x;
	x=(Item *)malloc(sizeof(Item));
	getch(fin);
	getsym(fin);
	expression(fin,x);
}
//-a*b/c+00123*0123.0456-(-a+b*c/d-f*A(a,-b,B[a],c[012],+D[(123-5*6)]))
