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
	hel = LoadLibrary(L"Message.DLL");
	if (hel == NULL)
	{
		MessageBox(0, L"����Message.DLLʧ�ܣ�", L"����", 0);
		return;
	}
	Api_OutPut_Func_Pointer = GetProcAddress(hel, "Api_OutPut");
	if (Api_OutPut_Func_Pointer == NULL)
	{
		MessageBox(0, L"��ȡApi_OutPut������ַʧ�ܣ�", L"����", 0);
		return;
	}
}