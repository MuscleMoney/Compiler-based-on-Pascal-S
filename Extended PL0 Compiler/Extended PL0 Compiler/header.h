#include "stdafx.h"


#define MAX_SYM_LENGTH 16  //���ų������ֵ
#define MAX_NUM_LENGTH 14  //���ֳ������ֵ
#define MAX_LINE_LENGTH 81 //�г������ֵ
#define MAX_STRING_LENGTH 50 //�ַ�����󳤶�
#define MAX_VAR_NUM 20//��������
#define CSMAX 30//�����Ŀ
#define CODE_MAX 800//Pָ������
#define STACK_MAX 1450//ջ����
#define PCODEF_MAX 64//P��������
#define TABLE_MAX 100//���ű�����
#define ARRAY_MAX 30//��������
#define BLOCK_MAX 20//������
#define STRING_MAX 600//�ַ�������
#define REAL_CONST_MAX 20//ʵ����������
#define LEVEL_MAX 7//������

enum  symbol{
	NUL,IDENT, NUM,//�գ���ʶ�������ִ�
	CHAR, INTEGER, REAL, STRING,//�ַ������ͣ�ʵ�ͣ��ַ���
	LPARENT, RPARENT, LBRACK, RBRACK,//�����ţ������ţ��������ţ���������
	COMMA, COLON, SEMICOLON, PERIOD,QUOTE, BECOMES,//����,ð�ţ��ֺţ���ţ���ֵ
	PLUS, MINUS, TIMES, DIV,//�Ӽ��˳�
	EQL, NEQ, GTR, GEQ, LSS, LEQ, //��ϵ����� ���ڣ������ڣ����ڣ����ڵ��ڣ�С�ڣ�С�ڵ���
	CONST, VAR, ARRAY, PROC, FUNC, //���������������飬���̣�����
	IF, THEN, ELSE, CASE, OF, FOR, TO, DOWNTO, DO,//������ͣ�����������of��for
	BEGIN, END, READ, WRITE }; //begin��end

enum IdentType{ NOTYPE, CONSTANT, VARIABLE,FUNCTION, PROCEDURE};
//   ��ʶ�����ͣ������ͣ� ������  ������    ������   ����
enum DataType{ NULTYPE, CHARTYPE, INTTYPE, REALTYPE,ARRAYTYPE};
//������������ �����ͣ� �ַ�       ����     ʵ��       ����

struct Table{//����������tc
	char name[MAX_SYM_LENGTH];//������
	IdentType identType;//��ʶ������
	DataType dataType;//��������
	int link;//ͬһ�ֳ�������һ����ʶ���ڷ��ű��е�λ��
	int ref;//����-�����С�����̺���-blocktableָ�룻
	int lev;//��̬���
	int adr;//����-����ջ��ַ�����̺���Ŀ������ڣ������ַ�������ֵ��ʵ������ָ�룻
	bool normal;//�β�Ϊfalse
};

struct BlockTable{//����������bc
	int last;//���һ����ʶ���ڷ��ű��е�λ��
	int lastpara;//���̻������һ�������ڷ��ű��е�λ��
	int psize;//������������Ϣ��ռ����ջ�Ĵ�С
	int vsize;//�ֲ�����������������Ϣ����ջ�еĴ洢��Ԫ��
};

struct ArrayTable{
	DataType dt;//��������
	int size;//�����С
};


enum Pcodef{//pָ��
	Lda,Lod,Ldi,Dis,e1,e2,e3,e4,Fct,Int,Jmp,Jpc,Swt,Cas,F1u,
	F2u,F1d,F2d,Mks,Cal,Idx,Ixx,Ldb,Cpb,Ldc,Ldr,
	Flt,Red,Wrs,Wrw,Wru,Hlt,Exp,Exf,Ldt,Not,Mus,
	Wrr,Sto,Eqr,Ner,Lsr,Ler,Gtr,Ger,Eql,Neq,Lss,
	Leq,Grt,Geq,Orr,Add,Sub,Adr,Sur,And,Mul,Div,
	Mod,Mur,Dir,Rdl,Wrl
};

struct Record{//ջ��Ԫ
	int i;
	double r;
};

struct Order{//ָ��
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



