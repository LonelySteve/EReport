#include "stdafx.h"
#include "network.h"
#include "mpq_api.h"

// 这个写回调函数可能会被多次调用来完成数据写入 https://blog.csdn.net/shang_cm/article/details/77938309
size_t write_callback(char *contents, size_t size, size_t nmemb, void *userdata)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userdata;
	// 增加分配数据域内存大小
	char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL)
	{
		/* out of memory! */
		Api_OutPut((char *)"接受响应数据出错：内存不足！");
		return 0;
	}

	mem->memory = ptr;
	// 从数据域有效数据结尾处开始，写入新的内容
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0; // 始终保持最后一位为终止符

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