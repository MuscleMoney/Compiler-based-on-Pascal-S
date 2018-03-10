#include "stdafx.h"


#define MAX_SYM_LENGTH 16  //符号长度最大值
#define MAX_NUM_LENGTH 14  //数字长度最大值
#define MAX_LINE_LENGTH 81 //行长度最大值
#define MAX_STRING_LENGTH 50 //字符串最大长度
#define MAX_VAR_NUM 20//最大变量数
#define CSMAX 30//情况数目
#define CODE_MAX 800//P指令上限
#define STACK_MAX 1450//栈上限
#define PCODEF_MAX 64//P函数上限
#define TABLE_MAX 100//符号表上限
#define ARRAY_MAX 30//数组上限
#define BLOCK_MAX 20//块上限
#define STRING_MAX 600//字符串上限
#define REAL_CONST_MAX 20//实常量表上限
#define LEVEL_MAX 7//层上限

enum  symbol{
	NUL,IDENT, NUM,//空，标识符，数字串
	CHAR, INTEGER, REAL, STRING,//字符，整型，实型，字符串
	LPARENT, RPARENT, LBRACK, RBRACK,//左括号，右括号，左中括号，右中括号
	COMMA, COLON, SEMICOLON, PERIOD,QUOTE, BECOMES,//逗号,冒号，分号，句号，赋值
	PLUS, MINUS, TIMES, DIV,//加减乘除
	EQL, NEQ, GTR, GEQ, LSS, LEQ, //关系运算符 等于，不等于，大于，大于等于，小于，小于等于
	CONST, VAR, ARRAY, PROC, FUNC, //常量，变量，数组，过程，程序
	IF, THEN, ELSE, CASE, OF, FOR, TO, DOWNTO, DO,//如果，就，否则，条件，of，for
	BEGIN, END, READ, WRITE }; //begin，end

enum IdentType{ NOTYPE, CONSTANT, VARIABLE,FUNCTION, PROCEDURE};
//   标识符类型：无类型， 常量，  变量，    函数，   过程
enum DataType{ NULTYPE, CHARTYPE, INTTYPE, REALTYPE,ARRAYTYPE};
//基本数据类型 无类型， 字符       整型     实数       数组

struct Table{//索引变量：tc
	char name[MAX_SYM_LENGTH];//符号名
	IdentType identType;//标识符类型
	DataType dataType;//数据类型
	int link;//同一分程序中上一个标识符在符号表中的位置
	int ref;//数组-数组大小；过程函数-blocktable指针；
	int lev;//静态层次
	int adr;//变量-运行栈地址；过程函数目标码入口；整数字符常量的值；实常量的指针；
	bool normal;//形参为false
};

struct BlockTable{//索引变量：bc
	int last;//最后一个标识符在符号表中的位置
	int lastpara;//过程或函数最后一个参数在符号表中的位置
	int psize;//参数和内务信息区占运行栈的大小
	int vsize;//局部变量参数及内需信息区在栈中的存储单元数
};

struct ArrayTable{
	DataType dt;//数据类型
	int size;//数组大小
};


enum Pcodef{//p指令
	Lda,Lod,Ldi,Dis,e1,e2,e3,e4,Fct,Int,Jmp,Jpc,Swt,Cas,F1u,
	F2u,F1d,F2d,Mks,Cal,Idx,Ixx,Ldb,Cpb,Ldc,Ldr,
	Flt,Red,Wrs,Wrw,Wru,Hlt,Exp,Exf,Ldt,Not,Mus,
	Wrr,Sto,Eqr,Ner,Lsr,Ler,Gtr,Ger,Eql,Neq,Lss,
	Leq,Grt,Geq,Orr,Add,Sub,Adr,Sur,And,Mul,Div,
	Mod,Mur,Dir,Rdl,Wrl
};

struct Record{//栈单元
	int i;
	double r;
};

struct Order{//指令
	Pcodef f;
	int x;
	int y;
};

struct CaseTab{
	int val;
	int lc;
};


struct Item{
	DataType dt;
	int ref;
};


//error.cpp
void error(int errnum);
void testerror();

//symbol.cpp
void getch(FILE *fin);
void getsym(FILE *fin);
void testgetsym(FILE *fin);
int strcmpa(char* Str1, char* Str2);

//getvalue.cpp
int getintvalue();
double getdecimalpart();
int getcharvalue(FILE* fin);

void testgetintvalue(FILE* fin);
void testgetdecimalpart(FILE* fin);
void testgetcharvalue(FILE* fin);

//decl.cpp
void constant(FILE* fin,Item *x);
void constdecl(FILE* fin);
void variable(FILE* fin);
void vardecl(FILE* fin);

void testconstdecl(FILE* fin);
void testvardecl(FILE* fin);

//expression.cpp
void expression(FILE* fin, Item* x);
void term(FILE* fin, Item* x);
void factor(FILE* fin, Item* x);

void testfactor(FILE* fin);
void testterm(FILE* fin);
void testexpression(FILE* fin);

//statement.cpp
void statement(FILE* fin);
void condition(FILE* fin);
void caseelement(FILE* fin);
void teststatement(FILE* fin);

//proc.cpp
void formalparapart(FILE* fin);
void procedure(FILE* fin);
void function(FILE* fin);
void block(FILE* fin);
void program(FILE* fin);

//skip.cpp
void skip(bool* skipitem, FILE* fin);

//table.cpp
extern void enter(char name[], IdentType idt, DataType dt,int adr);
extern void enter_b(char name[], IdentType idt);
extern int position(char name[]);
extern void entervar(char name[], DataType dt);
extern void enterBlock();
extern void enterArray(char name[], DataType dt, int size);
extern void enterString();
extern void printtable();

//code.cpp
void emit(Pcodef f);
void emit1(Pcodef f, int x);
void emit2(Pcodef f,int x,int y);
void printcode();

//interpret.cpp
void inter();
void interpret();



