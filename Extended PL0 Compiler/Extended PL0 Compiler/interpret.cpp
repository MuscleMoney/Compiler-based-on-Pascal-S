#include "stdafx.h"
#include "header.h"
#include <memory.h>
//运行栈
Record Stack[STACK_MAX];
extern int display[];
extern Order CodeTable[];
extern BlockTable blockTable[];
extern Table table[];
extern ArrayTable arrayTable[];
extern double realConstTable[];
extern char stringTable[][MAX_STRING_LENGTH];

int pc,t,b;
int h1,h2,h3,h4;
int lncnt,ocnt,blkcnt,chrcnt;
enum PS{
	run,fin,caschk,divchk,inxchk,stkchk,lincchk,lngchk,redchk
}ps;

void initStack(int t){
	Stack[t].i=0;
	Stack[t].r=0.0;
	return ;
}

void inter(){
	int f,x,y,red;
	f=CodeTable[pc].f;
	x=CodeTable[pc].x;
	y=CodeTable[pc].y;
	switch(f/10){
	case 0:{
			switch(f){
			case Lda:
				t++;
				if(t>=STACK_MAX)
					ps=stkchk;
				else
					Stack[t].i=display[x]+y;
				break;
			case Lod:
				t++;
				if(t>=STACK_MAX)
					ps=stkchk;
				else
					Stack[t]=Stack[display[x]+y];
				break;
			case Ldi:
				t++;
				if(t>STACK_MAX)
					ps=stkchk;
				else
					Stack[t]=Stack[Stack[display[x]+y].i];
				break;
			case Dis:
				h1=y;
				h2=x;
				h3=b;
				while(h1>h2){
					display[h1]=h3;
					h1--;
					h3=Stack[h3+2].i;
				}
				break;
			case Fct:
				printf("Empty function\n");
				break;
			case Int:
				Stack[t].i=Stack[t].i+y;
				break;
			default:
				printf("function num error!\n");
				break;
			}
			break;
		}
	case 1:{
			switch(f){
			case Jmp:
				pc=y-1;
				break;
			case Jpc:
				if(Stack[t].i==0)
					pc=y-1;
				t--;
				break;
			case Swt:
				h1=Stack[t].i;
				t--;
				h2=y;
				h3=0;
				while(h3 == 0){
					if(CodeTable[h2].f!=Cas){
						h3=1;
						ps=caschk;
					}
					else if(CodeTable[h2].y==h1){
						h3=1;
						pc=CodeTable[h2+1].y-1;
					}
					else{
						h2+=2;
					}
				}
				break;
			case Cas:
				break;
			case F1u:
				h1=Stack[t-1].i;
				if(h1<=Stack[t].i){
					Stack[Stack[t-2].i].i=h1;
				}
				else{
					t=t-3;
					pc=y-1;
				}
				break;
			case F2u:
				h2=Stack[t-2].i;
				h1=Stack[h2].i+1;
				if(h1<=Stack[t].i){
					Stack[h2].i=h1;
					pc=y-1;
				}
				else
					t-=3;
				break;
			case F1d:
				h1=Stack[t-1].i;
				if(h1>=Stack[t].i)
					Stack[Stack[t-2].i].i=h1;
				else{
					pc=y-1;
					t-=3;
				}
				break;
			case F2d:
				h2=Stack[t-2].i;
				h1=Stack[h2].i-1;
				if(h1>=Stack[t].i){
					Stack[h2].i=h1;
					pc=y-1;
				}
				else
					t-=3;
				break;
			case Mks:
				h1=blockTable[table[y].ref].vsize;
				if(t+h1>STACK_MAX)
					ps=stkchk;
				else{
					t+=5;
					Stack[t-1].i=h1-1;
					Stack[t].i=y;
				}
				break;
			case Cal:
				h1=t-y-5;
				h2=Stack[h1+4].i;
				h3=table[h2].lev;
				display[h3+1]=h1;
				h4=Stack[h1+3].i+h1;
				Stack[h1+1].i=pc;
				Stack[h1+2].i=display[h3];
				Stack[h1+3].i=b;
				for(h3=t+1;h3<h4;h3++)
					initStack(h3);
				b=h1;
				t=h4;
				pc=table[h2].adr-1;
				//输出运行栈
				break;
			default:
				printf("function num error!\n");
				break;
			}
			break;
		}
	case 2:{
			switch(f){
			case Idx:
				h1=y;
				h2=0;//arrayTable[h1].low
				h3=Stack[t].i;
				if(h3<h2)
					ps=inxchk;
				else if(h3>arrayTable[h1].size)
					ps=inxchk;
				else{
					t--;
					Stack[t].i=Stack[t].i+h3;
				}
				break;
			case Ixx:
				h1=y;
				h2=0;//arrayTable[h1].low
				h3=Stack[t].i;
				if(h3<=h2)
					ps=inxchk;
				else if(h3>arrayTable[h1].size)
					ps=inxchk;
				else{
					t--;
					Stack[t].i=Stack[t].i+h3;
				}
				break;
			case Ldb:
				h1=Stack[t].i;
				t-=1;
				h2=y+t;
				if(h2>=STACK_MAX)
					ps=stkchk;
				else{
					while(t<h2){
						t++;
						Stack[t]=Stack[h1];
						h1++;
					}
				}
				break;
			case Cpb:
				h1=Stack[t-1].i;
				h2=Stack[t].i;
				h3=h1+y;
				while(h1<h3){
					Stack[h1]=Stack[h2];
					h1++;
					h2++;
				}
				t-=2;
				break;
			case Ldc:
				t++;
				if(t>STACK_MAX)
					ps=stkchk;
				else
					Stack[t].i=y;
				break;
			case Ldr:
				t++;
				if(t>STACK_MAX)
					ps=stkchk;
				else
					Stack[t].r=realConstTable[y];
				break;
			case Flt://实型转化
				h1=t-y;
				Stack[h1].r=Stack[h1].i;
				Stack[h1].i=0;
				break;
			case Red:
				switch(y){
				case 1:
					red=scanf("%d",&Stack[Stack[t].i].i);
					if(red)
						t--;
					else
						ps=redchk;
					break;
				case 2:
					red=scanf("%lf",&Stack[Stack[t].i].r);
					if(red)
						t--;
					else
						ps=redchk;
					break;
				case 3:
					red=scanf("%c",&Stack[Stack[t].i].i);
					if(red)
						t--;
					else
						ps=redchk;
					break;
				default:
					ps=redchk;
					printf("read type error\n");
					break;
				}
				break;
			case Wrs://
				printf("%s",stringTable[y]);
				break;
			case Wrw:
				switch(y){
				case 1:printf("%d",Stack[t].i); break;
				case 2:printf("%f",Stack[t].r); break;
				case 3:printf("%c",Stack[t].i); break;
				default:
					printf("Wrong wrw number!\n"); break;
				}
				t--;
				break;
			default:
				printf("function num error!\n");
				break;
			}
			break;
		}
	case 3:{
			switch(f){
			case Wru://给定域宽
				break;
			case Hlt:
				ps=fin;
				break;
			case Exp:
				t=b-1;
				pc=Stack[b+1].i;
				b=Stack[b+3].i;
				break;
			case Exf:
				t=b;
				pc=Stack[b+1].i;
				b=Stack[b+3].i;
				break;
			case Ldt:
				Stack[t]=Stack[Stack[t].i];
				break;
			case Not:
				Stack[t].i=!Stack[t].i;
				break;
			case Mus:
				Stack[t].i=-Stack[t].i;
				Stack[t].r=-Stack[t].r;
				break;
			case Wrr://写实数
				break;
			case Sto:
				Stack[Stack[t-1].i]=Stack[t];
				t-=2;
				break;
			case Eqr:
				t--;
				Stack[t].i=(Stack[t].r == Stack[t+1].r);
				break;
			default:
				printf("function num error!\n");
				break;
			}
			break;
		}
	case 4:{
			switch(f){
			case Ner:
				t--;
				Stack[t].i=(Stack[t].r != Stack[t+1].r);
				break;
			case Lsr:
				t--;
				Stack[t].i=(Stack[t].r < Stack[t+1].r);
				break;
			case Ler:
				t--;
				Stack[t].i=(Stack[t].r <= Stack[t+1].r);
				break;
			case Gtr:
				t--;
				Stack[t].i=(Stack[t].r > Stack[t+1].r);
				break;
			case Ger:
				t--;
				Stack[t].i=(Stack[t].r >= Stack[t+1].r);
				break;
			case Eql:
				t--;
				Stack[t].i=(Stack[t].i == Stack[t+1].i);
				break;
			case Neq:
				t--;
				Stack[t].i=(Stack[t].i != Stack[t+1].i);
				break;
			case Lss:
				t--;
				Stack[t].i=(Stack[t].i < Stack[t+1].i);
				break;
			case Leq:
				t--;
				Stack[t].i=(Stack[t].i <= Stack[t+1].i);
				break;
			case Grt:
				t--;
				Stack[t].i=(Stack[t].i > Stack[t+1].i);
				break;
			default:
				printf("function num error!\n");
				break;
			}
			break;
		}
	case 5:{
			switch(f){
			case Geq:
				t--;
				Stack[t].i=(Stack[t].i >= Stack[t+1].i);
				break;
			case Orr:
				t--;
				Stack[t].i=(Stack[t].i || Stack[t+1].i);
				break;
			case Add:
				t--;
				Stack[t].i=(Stack[t].i + Stack[t+1].i);
				break;
			case Sub:
				t--;
				Stack[t].i=(Stack[t].i - Stack[t+1].i);
				break;
			case Adr:
				t--;
				Stack[t].r=(Stack[t].r + Stack[t+1].r);
				break;
			case Sur:
				t--;
				Stack[t].r=(Stack[t].r - Stack[t+1].r);
				break;
			case And:
				t--;
				Stack[t].i=(Stack[t].i && Stack[t+1].i);
				break;
			case Mul:
				t--;
				Stack[t].i=(Stack[t].i * Stack[t+1].i);
				break;
			case Div:
				t--;
				if(Stack[t+1].i==0)
					ps=divchk;
				else
					Stack[t].i=(Stack[t].i / Stack[t+1].i);
				break;
			case Mod:
				if(Stack[t+1].i==0)
					ps=divchk;
				else
					Stack[t].i=(Stack[t].i % Stack[t+1].i);
				break;
			default:
				printf("function num error!\n");
				break;
			}
			break;
		}
	case 6:{
			switch(f){
			case Mur:
				t--;
				Stack[t].r=(Stack[t].r * Stack[t+1].r);
				break;
			case Dir:
				t--;
				if(Stack[t+1].r==0)
					ps=divchk;
				else
					Stack[t].r=(Stack[t].r / Stack[t+1].r);
				break;
			case Rdl:
				//应该用不到
				break;
			case Wrl:
				printf("\n");
				break;
			default:
				printf("function num error!\n");
				break;
			}
			break;
		}
	default: 
		printf("function num error!\n");
		break;
	}	
}

void interpret(){
	Stack[1].i=0;
	Stack[2].i=0;
	Stack[3].i=-1;
	Stack[4].i=0;
	display[0]=0;
	display[1]=0;
	t=blockTable[0].vsize-1;
	b=0;
	pc=table[Stack[4].i].adr;
	ps=run;
	while(ps==run){
		inter();
		pc++;
		ocnt++;
	}
	if(ps!=fin){
		printf("****Interpret ended at %d",pc);
		printf(" Because of:");
		switch(ps){
		case caschk: printf("undefined case\n");break;
		case divchk: printf("division by 0\n");break;
		case inxchk: printf("invalid index\n");break;
		case stkchk: printf("storage overflow\n");break;
		case redchk: printf("reading error\n");break;
		default: printf("Unknown error\n");break;
		}
		h1=b;
		blkcnt=10;
		/*while(h1>=0){
			printf("\n");
			blkcnt=blkcnt-1;
			if(blkcnt==0)
				h1=0;	
			h2=Stack[h1+4].i;
			if(h1!=0)
				printf("%s call at",table[h2].name,Stack[h1+1].i);
			h2=blockTable[table[h2].ref].last;
			while(h2!=0){
				if(table[h2].identType == VARIABLE){
					if(table[h2].identType>=CHARTYPE && table[h2].identType<=REALTYPE){
						if(table[h2].normal)
							h3=h1+table[h2].adr;
						else
							h3=Stack[h1+table[h2].adr].i;
						switch(table[h2].dataType){
						case INTTYPE:printf("%d",Stack[t].i); break;
						case REALTYPE:printf("%f",Stack[t].r); break;
						case CHARTYPE:printf("%c",Stack[t].c); break;
						default:printf("Wrong type!\n"); break;
						}
					}
					h2=table[h2].link;
				}
			}
			h1=Stack[h1+3].i;
		}*/
	}
	printf("\n");
	printf("-----------------------------------\n");
	printf("%d steps\n",ocnt);
}