#include "stdafx.h"
#include "mpq_api.h"

void *Api_OutPut_Func_Pointer; //����ȫ�ֵĺ���ָ��

int Api_OutPut(char * content)
{
	return ((int(__stdcall *)(char *))Api_OutPut_Func_Pointer)(content);
}

void DPLS_INIT_LOADALLPORT()
{
	HMODULE hel = NULL;
	hel = LoadLibrary("Message.DLL");
	if (hel == NULL)
	{
		MessageBox(0, "����Message.DLLʧ�ܣ�", "����", 0);
		return;
	}
	Api_OutPut_Func_Pointer = GetProcAddress(hel, "Api_OutPut");
	if (Api_OutPut_Func_Pointer == NULL)
	{
		MessageBox(0, "��ȡApi_OutPut������ַʧ�ܣ�", "����", 0);
		return;
	}
}