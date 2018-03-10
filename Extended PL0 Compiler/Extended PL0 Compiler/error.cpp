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

int errcnt;//错误数量统计

char errmsg[ERROR_TYPE_NUM][ERROR_MSG_LENGTH]={
	"Ident too long",//0 标识符长度过长 
	"Number too long",//1 数字长度过长
	"String too long",//2 字符串长度过长
	"String doesn't end",//3 字符串未结束
	"Illegal char",//4 非法字符
	"Constant form error",//5 常量格式错误
	"No num after dot",//6 小数点后无数字
	"Constant end error",//7 常量未正常结束
	"Constant char type error",//8 常量字符类型有误
	"Should be '='",//9 应是等号
	"Should be Ident",//10 应是标识符
	"Constdecl end error",//11 常量定义异常结束
	"Array var type error",//12 数组变量类型错误
	"Should be ']'",//13 应为右中括号
	"Array length should be unsigned int",//14 数组长度应是无符号整数
	"Should be '['",//15 应是左中括号
	"Var type error",//16 变量类型错误
	"Should be ':'",//17 应是冒号
	"Vardecl should be ended with ';'",//18 变量声明应以分号结尾
	"Vardecl end error",//19 变量声明非正常结束
	"Expression here should return unsigned int",//20 此处表达式应返回无符号整数
	"Should be ')'",//21 应是右括号
	"Factor sym type error",//22 因子符号类型错误
	"Should be 'then', if statement incomplete",//23 应是then，if语句不完整
	"Should be end",//24 应是end
	"Should be 'of'",//25 应是of,case 语句不完整
	"Should be end, Complex statement end error",//26 应是end, 复合语句未正常结束
	"Should be String or Expression, Write statement error",//27 应是字符串或表达式， 写语句错误
	"Should be 'do'",//28 应是do
	"Should be 'to' or 'downto', for statement error",//29 应是to或downto，for语句错误
	"Should be ':='",//30 应是赋值符号
	"Statement start error",//31 语句起始有误
	"Should be relation op",//32 应是关系运算符
	"Should be basic type",//33 应是基本类型
	"Header should be ended with';'",//34 头部分号结尾
	"Should be begin",//35 应是begin
	"Program ending error",//36 程序未结束
	"Should be '('",//37 应是左括号
	"Should be 'proc'",//38 应是过程
	"Should be 'func'",//39 应是函数
	"Header error",//40 函数或过程头部有误
	"Should be ';'",//41 应是分号
	"Char const illegal",//42 字符常量不合法
	"Array length can't be 0",//43 数组长度不能为零
	"Datatype error",//44 数据类型有误
	"Term times error",//45 项乘法有误
	"Term div error",//46 项除法有误
	"Ident Type error",//47 标识符类型有误
	"Array index should be unsigned int",//48 数组下标应为无符号整型
	"Too many paras",//49 参数数量过多
	"Para datatype error",//50 参数数据类型有误
	"Lack para",//51缺少参数
	"Can't assign Const",//52 不能对常量赋值
	"Array Ident lack index",//53 数组标识符缺少下标
	"Assign statement datatype error",//54 赋值语句数据类型错误
	"Can't assign this ident with index",//55 不能对非数组标识符进行下标赋值
	"Assign statement symbol error",//56 赋值语句字符类型有误
	"Case element datatype error",//57 case元素类型有误，应是整型或字符型
	"Ident doesn't exist",//58 标识符不存在
	"Read statement para should be variable",//59 读语句参数应是变量
	"Read statement datatype error",//60 读语句数据类型不支持
	"for circle variable should be inttype",//61 for循环变量应为整型
	"for circle variable can't be formal para",//62 for循环变量不能为形参
	"for circle expression should be inttype",//63 for循环表达式类型应为整型
	"Condition expression datatype error",//64 条件表达式两个表达式类型错误
	"Case table element datatype error",//65 情况表元素类型错误
	"Case table const already assigned",//66 情况表常量重复
	"Too many Ident",//67 符号表溢出
	"Ident assigned!",//68 标识符重复定义
	"Too many Block",//69 分程序溢出
	"Too many Array",//70 数组数量溢出
	"Too many Real Const",//71 实常量表溢出
	"Too many case",//72 case 过多
	"Too many strings",//73 字符串过多
	"No return statement",//74缺少返回值
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
		printf("↑\n");
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



