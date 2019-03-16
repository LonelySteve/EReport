#include "stdafx.h"
#include "network.h"
#include "mpq_api.h"

// ���д�ص��������ܻᱻ��ε������������д�� https://blog.csdn.net/shang_cm/article/details/77938309
size_t write_callback(char *contents, size_t size, size_t nmemb, void *userdata)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userdata;
	// ���ӷ����������ڴ��С
	char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL)
	{
		/* out of memory! */
		Api_OutPut((char *)"������Ӧ���ݳ����ڴ治�㣡");
		return 0;
	}

	mem->memory = ptr;
	// ����������Ч���ݽ�β����ʼ��д���µ�����
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0; // ʼ�ձ������һλΪ��ֹ��

	return realsize;
}

unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}