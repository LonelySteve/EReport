#include "stdafx.h"
#include <string>
#include "logging.h"
#include "mpq_api.h"

using std::string;

template<typename T>
T build_msg(T msg, LogLevel level)
{
	T msg_builder = "[EReport]";
	switch (level)
	{
	case INFO:
		msg_builder += "[INFO]";
		break;
	case DEBUG:
		msg_builder += "[DEBUG]";
		break;
	case WARNING:
		msg_builder += "[WARNING]";
		break;
	case ERR:
		msg_builder += "[ERROR]";
		break;
	default:
		break;
	}
	msg_builder += msg;
	return msg_builder;
}

void Log(string msg, LogLevel level)
{
	Api_OutPut((char*)build_msg(msg, level).c_str());
}

void Log(CString msg, LogLevel level)
{
	Api_OutPut(build_msg(msg, level).GetBuffer());
}
