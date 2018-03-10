//finished
#include "stdafx.h"
#include "header.h"
#include <string.h>

#define ERROR_TYPE_NUM 80
#define ERROR_MSG_LENGTH 60

extern int cl;
extern int cc;
extern char tmpline[];
extern char tmpsym[];

int errcnt;//��������ͳ��

char errmsg[ERROR_TYPE_NUM][ERROR_MSG_LENGTH]={
	"Ident too long",//0 ��ʶ�����ȹ��� 
	"Number too long",//1 ���ֳ��ȹ���
	"String too long",//2 �ַ������ȹ���
	"String doesn't end",//3 �ַ���δ����
	"Illegal char",//4 �Ƿ��ַ�
	"Constant form error",//5 ������ʽ����
	"No num after dot",//6 С�����������
	"Constant end error",//7 ����δ��������
	"Constant char type error",//8 �����ַ���������
	"Should be '='",//9 Ӧ�ǵȺ�
	"Should be Ident",//10 Ӧ�Ǳ�ʶ��
	"Constdecl end error",//11 ���������쳣����
	"Array var type error",//12 ����������ʹ���
	"Should be ']'",//13 ӦΪ��������
	"Array length should be unsigned int",//14 ���鳤��Ӧ���޷�������
	"Should be '['",//15 Ӧ����������
	"Var type error",//16 �������ʹ���
	"Should be ':'",//17 Ӧ��ð��
	"Vardecl should be ended with ';'",//18 ��������Ӧ�ԷֺŽ�β
	"Vardecl end error",//19 ������������������
	"Expression here should return unsigned int",//20 �˴����ʽӦ�����޷�������
	"Should be ')'",//21 Ӧ��������
	"Factor sym type error",//22 ���ӷ������ʹ���
	"Should be 'then', if statement incomplete",//23 Ӧ��then��if��䲻����
	"Should be end",//24 Ӧ��end
	"Should be 'of'",//25 Ӧ��of,case ��䲻����
	"Should be end, Complex statement end error",//26 Ӧ��end, �������δ��������
	"Should be String or Expression, Write statement error",//27 Ӧ���ַ�������ʽ�� д������
	"Should be 'do'",//28 Ӧ��do
	"Should be 'to' or 'downto', for statement error",//29 Ӧ��to��downto��for������
	"Should be ':='",//30 Ӧ�Ǹ�ֵ����
	"Statement start error",//31 �����ʼ����
	"Should be relation op",//32 Ӧ�ǹ�ϵ�����
	"Should be basic type",//33 Ӧ�ǻ�������
	"Header should be ended with';'",//34 ͷ���ֺŽ�β
	"Should be begin",//35 Ӧ��begin
	"Program ending error",//36 ����δ����
	"Should be '('",//37 Ӧ��������
	"Should be 'proc'",//38 Ӧ�ǹ���
	"Should be 'func'",//39 Ӧ�Ǻ���
	"Header error",//40 ���������ͷ������
	"Should be ';'",//41 Ӧ�Ƿֺ�
	"Char const illegal",//42 �ַ��������Ϸ�
	"Array length can't be 0",//43 ���鳤�Ȳ���Ϊ��
	"Datatype error",//44 ������������
	"Term times error",//45 ��˷�����
	"Term div error",//46 ���������
	"Ident Type error",//47 ��ʶ����������
	"Array index should be unsigned int",//48 �����±�ӦΪ�޷�������
	"Too many paras",//49 ������������
	"Para datatype error",//50 ����������������
	"Lack para",//51ȱ�ٲ���
	"Can't assign Const",//52 ���ܶԳ�����ֵ
	"Array Ident lack index",//53 �����ʶ��ȱ���±�
	"Assign statement datatype error",//54 ��ֵ����������ʹ���
	"Can't assign this ident with index",//55 ���ܶԷ������ʶ�������±긳ֵ
	"Assign statement symbol error",//56 ��ֵ����ַ���������
	"Case element datatype error",//57 caseԪ����������Ӧ�����ͻ��ַ���
	"Ident doesn't exist",//58 ��ʶ��������
	"Read statement para should be variable",//59 ��������Ӧ�Ǳ���
	"Read statement datatype error",//60 ������������Ͳ�֧��
	"for circle variable should be inttype",//61 forѭ������ӦΪ����
	"for circle variable can't be formal para",//62 forѭ����������Ϊ�β�
	"for circle expression should be inttype",//63 forѭ�����ʽ����ӦΪ����
	"Condition expression datatype error",//64 �������ʽ�������ʽ���ʹ���
	"Case table element datatype error",//65 �����Ԫ�����ʹ���
	"Case table const already assigned",//66 ��������ظ�
	"Too many Ident",//67 ���ű����
	"Ident assigned!",//68 ��ʶ���ظ�����
	"Too many Block",//69 �ֳ������
	"Too many Array",//70 �����������
	"Too many Real Const",//71 ʵ���������
	"Too many case",//72 case ����
	"Too many strings",//73 �ַ�������
	"No return statement",//74ȱ�ٷ���ֵ
};
void error(int errnum){
	int i;
	if(errnum>ERROR_TYPE_NUM ||errnum<0){
		printf("errnum error!\n");
		return ;
	}
	else{
		if(errcnt == 0){
			printf("Errors:\n");
		}
		printf("Position:(%d) Errnum:%d Msg:%s\n",cl,errnum,errmsg[errnum]);
		printf("%s\n",tmpline);
		for(i=0;i<cc-strlen(tmpsym);i++){
			if(tmpline[i] =='\t')
				printf("\t");
			else
				printf(" ");
		}
		printf("��\n");
		errcnt++;
		return;
	}
}

void testerror(){
	error(1);
	cl=10;
	error(2);
	cl=15;
	error(3);
	cl=26;
	error(58);
	error(-1);
	return;
}



