#pragma once
//使用模块定义文件
#define dllexp __declspec(dllexport) 
extern "C"	// 以C的形式导出函数
{
	//这里不声明无法正常导出
	dllexp void __stdcall set();
	dllexp void __stdcall about();
	dllexp int __stdcall end();
	dllexp int __stdcall EventFun(char *qq, int msgtype, int msgctype, char *msgsource, char *dop, char *bep, char *msg, char *rawmsg, char *backptr);
	dllexp int __stdcall Message(char *qq, int type, char *rawmsg, char *cookie, char *sessionkey, char *clientkey);
	dllexp const char * __stdcall info();
}
