#include "stdafx.h"
#include "mpq_api.h"

void *Api_Func_Pointers[1]; //����ȫ�ֵĺ���ָ������

int Api_OutPut(char * content)
{
	return ((int(__stdcall *)(char *))Api_Func_Pointers[0])(content);
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
	Api_Func_Pointers[0] = GetProcAddress(hel, "Api_OutPut");
	if (Api_Func_Pointers[0] == NULL)
	{
		MessageBox(0, "��ȡApi_OutPut������ַʧ�ܣ�", "����", 0);
		return;
	}
}