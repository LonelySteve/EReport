#pragma once
//ʹ��ģ�鶨���ļ�
#define dllexp __declspec(dllexport) 
extern "C"	// ��C����ʽ��������
{
	//���ﲻ�����޷���������
	dllexp void __stdcall set();
	dllexp void __stdcall about();
	dllexp int __stdcall end();
	dllexp int __stdcall EventFun(char *qq, int msgtype, int msgctype, char *msgsource, char *dop, char *bep, char *msg, char *rawmsg, char *backptr);
	dllexp int __stdcall Message(char *qq, int type, char *rawmsg, char *cookie, char *sessionkey, char *clientkey);
	dllexp const char * __stdcall info();
}
