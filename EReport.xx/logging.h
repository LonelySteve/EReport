#pragma once
 
enum LogLevel
{
	INFO,
	DEBUG,
	WARNING,
	ERR
};

void Log(std::string msg, LogLevel level = INFO);
void Log(CString msg, LogLevel level = INFO);