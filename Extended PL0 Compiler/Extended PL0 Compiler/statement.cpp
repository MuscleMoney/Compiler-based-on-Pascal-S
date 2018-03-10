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
			//标识符不存在
			error(58);
			skip(&skipstmt,fin);
		}
		getsym(fin);
		x->dt = table[posi].dataType;
		x->ref = table[posi].ref;
		if(table[posi].identType == CONSTANT){
			//不能给常量赋值
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
					//数组标识符缺少下标
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
						//赋值语句数据类型错误
						error(54);
						skip(&skipstmt,fin);
					}
					//printf("This is a var becomes or func return statement\n");
				}
			}
			else if(sym == LBRACK){
				if(table[posi].dataType != ARRAYTYPE){
					//应是数组元素赋值
					skip(&skipstmt,fin);
				}
				else{
					x->dt = arrayTable[table[posi].ref].dt;
					getsym(fin);
					expression(fin,y);
					if(y->dt != INTTYPE){
						//下标类型应是整型
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
									//赋值语句数据类型错误
									error(54);
								}
								//printf("This is a arr element becomes statement\n");
								return ;
							}
							else{
								error(30);
								skip(&skipstmt,fin);
								//应是赋值
							}
						}
						else{
							error(13);
							skip(&skipstmt,fin);
							//应是右中括号
						}
					}
				}
			}
			else{
				//赋值语句字符有误
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
					//类型检查
					error(50);
				}
				if(paracnt>lastpara){
						//参数数量超出
					error(49);
					skip(&skipstmt,fin);
				}
				while(sym == COMMA){//补齐函数调用
					getsym(fin);
					paracnt+=1;
					expression(fin,y);
					if(y->dt != table[paracnt].dataType){
						//参数数据类型有误
						error(50);
					}
					if(paracnt>lastpara){
						//参数数量超出
						error(49);
						skip(&skipstmt,fin);
					}
				}
				if(sym ==RPARENT){
					//printf("This is a function factor\n");
					if(paracnt<lastpara){
						error(51);
					//少参数
					}
					emit1(Cal,blockTable[table[posi].ref].psize-1);
					if(table[posi].lev<level)
						emit2(Dis,table[posi].lev,level);
					getsym(fin);
					return ;
				}
				else{
					//应为右括号
					error(21);
					skip(&skipstmt,fin);
				}
			}
			else{
				//标识符类型有误
				error(47);
				skip(&skipstmt,fin);
			}
		}
		else{
			//标识符类型有误
			error(47);
			skip(&skipstmt,fin);
		}
	}
/*以上开头是标识符的语句*/
	else if(sym == IF){//IF语句
		getsym(fin);
		condition(fin);
		lc1=lc;
		emit(Jpc);
		if(sym == THEN){
			getsym(fin);
			//生成相应代码
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
			//此处应是then，if语句不完整
		}
	}
	else if(sym == CASE){//情况语句
		cs=0; exit=0;
		getsym(fin);
		expression(fin,x);
		if(x->dt != INTTYPE && x->dt != CHARTYPE){
			//case元素类型有误
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
				//此处应是end,case未正常结束
			}
		}
		else{
			error(25);
			skip(&skipstmt,fin);
			//此处应是of,case不完整
		}
	}
	else if(sym == BEGIN){//复合语句
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
			//此处应为END，复合语句未正常结束
		}
	}
	else if(sym == READ){//读语句
		getsym(fin);
		if(sym ==LPARENT){
			getsym(fin);
			if(sym == IDENT){
				//验证标识符
				posi=position(tmpsym);
				if(posi == 0){
					//标识符不存在
					error(58);
				}
				if(table[posi].identType!=VARIABLE){
					//读语句变量
					error(59);
				}
				if(table[posi].dataType == NULTYPE || table[posi].dataType == ARRAYTYPE){
					//读语句数据类型不支持
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
							//标识符不存在
							error(58);
						}
						if(table[posi].identType!=VARIABLE){
							//读语句变量
							error(59);
						}
						if(table[posi].dataType == NULTYPE || table[posi].dataType == ARRAYTYPE){
							//读语句数据类型不支持
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
						//应为标识符
						error(10);
						skip(&skipstmt,fin);
						//跳过
					}
				}
				if(sym == RPARENT){
					getsym(fin);
					//printf("This is a READ statement\n");
					return ;
				}
				else{
					//此处应为右括号
					error(21);
					skip(&skipstmt,fin);
				}
			}
			else{
				//此处应为标识符
				error(10);
				skip(&skipstmt,fin);
			}
		}
		else{
			//此处应为左括号
			error(37);
			skip(&skipstmt,fin);
		}
	}
	else if(sym == WRITE){//写语句
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
						/*写语句这里的意思？*/
						//printf("This is a write(string,expression) statement\n");
						emit(Wrl);
						return ;
					}
					else{
						//写语句未结束
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
					//此处应为右括号 写语句未结束
					error(21);
					skip(&skipstmt,fin);
				}
			}
			else if(sym == PLUS || sym == MINUS || sym == LPARENT || sym == IDENT || sym == NUM)/*表达式开始的符号*/{
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
					//此处应为右括号
					error(21);
					skip(&skipstmt,fin);
				}
			}
			else{
				//此处应为字符串或表达式，写语句错误
				error(27);
				skip(&skipstmt,fin);
			}
		}
		else{
			//此处应为左括号
			error(37);
			skip(&skipstmt,fin);
		}
	}
	else if(sym == SEMICOLON || sym ==  END){//空语句
		//printf("This is a empty statement\n");
		return;
	}
	else if(sym == FOR){
		getsym(fin);
		if(sym == IDENT){
			posi=position(tmpsym);
			if(posi == 0){
				//未找到标识符
				error(58);
			}
			if(table[posi].identType!=VARIABLE){
				//标识符类型应为变量
				error(59);
			}
			if(table[posi].dataType!=INTTYPE){
				//类型应为整型
				error(61);
			}
			if(!table[posi].normal){
				//不能为形参
				error(62);
			}
			getsym(fin);
			emit2(Lda,table[posi].lev,table[posi].adr);
			if(sym == BECOMES){
				getsym(fin);
				expression(fin,x);//类型检查
				if(x->dt!=INTTYPE){
					//表达式类型应为整型
					error(63);
				}
				//生成相应代码
				if(sym == TO){
					getsym(fin);
					expression(fin,x);//类型检查
					if(x->dt!=INTTYPE){
						//表达式类型应为整型
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
						//此处应为DO
						error(28);
						skip(&skipstmt,fin);
					}
				}
				else if(sym == DOWNTO){
					getsym(fin);
					expression(fin,x);//类型检查
					if(x->dt != INTTYPE){
						//for循环表达式类型应为整形
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
						//此处应为DO
						error(28);
						skip(&skipstmt,fin);
					}
				}
				else{
					//此处应是TO或DOWNTO,for 语句有误
					error(29);
					skip(&skipstmt,fin);
				}
			}
			else{
				//此处应为赋值符号
				error(30);
				skip(&skipstmt,fin);
			}
		}
		else{
			//此处应为标识符
			error(10);
			skip(&skipstmt,fin);
		}
	}
	else{
		//语句开始符号有误
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
		default: //应是关系运算符
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
		default: //应是关系运算符
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
		default: //应是关系运算符
			skip(&skipcdt,fin);
			error(32);
			break;
		}
	}
	else{
		//条件表达式两个表达式类型错误
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
		//情况表元素只能出现整型或字符型常量
		error(65);
	}
	for(i=0;i<cs;i++){
		if(casetab[i].val == tmpi){
			//常量重复
			error(66);
		}
	}
	casetab[cs].val=tmpi;
	casetab[cs].lc=lc;
	cs+=1;
	if(cs>=CSMAX){
		//case数量过多
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
		//应是冒号
		skip(&skipcaseelmt,fin);
		error(17);
	}
}

void teststatement(FILE* fin){
	getch(fin);
	getsym(fin);
	statement(fin);
}