#pragma once
#include <string>
typedef struct MemoryStruct {
	char *memory;
	size_t size;
} MemoryStruct;

size_t write_callback(char *contents, size_t size, size_t nmemb, void *userdata);

std::string UrlEncode(const std::string& str);