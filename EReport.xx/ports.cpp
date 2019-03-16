#include "stdafx.h"
#include "network.h"
#include "mpq_api.h"
#include "base64.h"
#include "ports.h"
#include "SettingDlg.h"
#include "logging.h"
#include <curl/curl.h>
#include <rapidjson/document.h>

#define MAX_PATHLENGTH 255

using namespace rapidjson;

CString tran_address;
BOOL enable_status;

void init()
{
	DPLS_INIT_LOADALLPORT();
	// 初始化Curl
	curl_global_init(CURL_GLOBAL_ALL);
	// 加载设置项 默认转发接口为 http://127.0.0.1:8050
	GetPrivateProfileString("EReport", "address", "http://127.0.0.1:8050", tran_address.GetBuffer(2048), 2048, ".\\Set.ini");
	enable_status = GetPrivateProfileInt("EReport", "enable", 1, ".\\Set.ini");
}

dllexp void __stdcall set()
{
	SettingDlg dlg;

	dlg.m_tran_address = tran_address;
	dlg.m_enable_tran = enable_status;
	dlg.DoModal();
	tran_address = dlg.m_tran_address;
	enable_status = dlg.m_enable_tran;
}

dllexp void __stdcall about()
{
	MessageBox(0, "事件转发上报插件@JLoeve，参考了mypcqq c++ sdk的实现", "提示", 0);
}

dllexp const char * __stdcall info()
{
	return "事件转发上报插件";
}

dllexp int __stdcall end()
{
	// 写入配置
	WritePrivateProfileString("EReport", "address", tran_address.GetString(), ".\\Set.ini");
	WritePrivateProfileString("EReport", "enable", enable_status ? "1" : "0", ".\\Set.ini");
	// 释放curl全局资源
	curl_global_cleanup();
	Log(CString("已卸载事件上报插件！"));
	// 释放设置项资源
	tran_address.ReleaseBuffer();
	// 返回非0成功 亦可不返回或为空
	return 1;
}

/**
/* @brief EventFun
/* @param qq 多QQ登录情况下用于识别是哪个Q
/* @param msgtype 信息唯一标识-1 未定义事件 1 好友信息 2,群信息 3,讨论组信息 4,临时会话 1001,被添加好友 1002,好友在线状态改变 1003 被删除好友 1004 签名变更 2001 某人申请加入群 2002 某人被邀请加入群 2003 我被邀请加入群 2005 某人被批准加入了群 2006 某人退出群  2007 某人被管理移除群 2008 某群被解散 2009 某人成为管理员 2010 某人被取消管理员 2011 群名片变动 2012 群名变动//暂未解析 2013 群公告变动
/* @param msgctype 对象申请、被邀请入群事件下该值为1时即对象为不良成员
/* @param msgsource 信息的源头  群号,好友QQ,讨论组ID,临时会话对象QQ等
/* @param dop 主动触发这条信息的对象 T人时为T人的管理员QQ
/* @param bep 被动接受这条信息的对象 T人时为被T对象的QQ
/* @param msg 视情况而定的信息内容  申请入群时为入群理由,添加好友为附加信息,T人之类的为空
/* @param rawmsg 经过解密后的封包字节数据或json结构信息
/* @param backptr 信息回传指针。 视情况而定的返回附加文本信息  拒绝好友申请时则为拒绝理由 方式:’写到内存("测试",参_信息回传文本指针_Out)
/* @return 该函数被动响应各种各样的会话、系统事件
*/
dllexp int __stdcall EventFun(char *qq, int msgtype, int msgctype, char *msgsource, char *dop, char *bep, char *msg, char *rawmsg, char *backptr)
{
	CString tip_str;
	tip_str.Format("转发事件至%s", tran_address);
	Log(tip_str);  // This one has magic, Don't Touch it!

	if (enable_status)
	{
		CURL *curl = nullptr;
		Document document;
		curl = curl_easy_init();
		if (curl != nullptr)
		{
			struct MemoryStruct chunk;

			chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
			chunk.size = 0;    /* no data at this point */

			struct curl_slist *header_chunk = NULL;
			// 使用 POST 提交表单
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			header_chunk = curl_slist_append(header_chunk, "Content-Type: application/x-www-form-urlencoded");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_chunk);
			curl_easy_setopt(curl, CURLOPT_URL, tran_address.GetString());
			/* example.com is redirected, so we tell libcurl to follow redirection */
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			/* set Referer: automatically when following redirects */
			curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);

			long data_length = strlen(qq) + strlen(msgsource) + strlen(dop) + strlen(bep) + strlen(msg) + strlen(rawmsg) + 1000;
			char * buffer = new char[data_length];
			sprintf_s(buffer, data_length, "qq=%s&msgtype=%d&msgctype=%d&msgsource=%s&dop=%s&bep=%s&msg=%s&rawmsg=%s",
				UrlEncode(qq).c_str(),
				msgtype,
				msgctype,
				UrlEncode(msgsource).c_str(),
				UrlEncode(dop).c_str(),
				UrlEncode(bep).c_str(),
				UrlEncode(base64_encode((const unsigned char *)msg, strlen(msg))).c_str(),
				UrlEncode(base64_encode((const unsigned char *)rawmsg, strlen(rawmsg))).c_str()
			);

			/* size of the POST data */
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(buffer));
			/* pass in a pointer to the data - libcurl will not copy */
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
			/* Perform the request, res will get the return code */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			/* we pass our 'chunk' struct to the callback function */
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
			/* get it! */
			auto res = curl_easy_perform(curl);
			/* Check for errors */
			if (res != CURLE_OK)
			{
				const char* err_msg = curl_easy_strerror(res);
				CString tmp;
				tmp.Format("转发地址响应出错！出错信息：%s", err_msg);
				Log(tmp, ERR);
			}
			else
			{
				/*
				 * Now, our chunk.memory points to a memory block that is chunk.size
				 * bytes big and contains the remote file.
				 *
				 * Do something nice with it!
				 */

				if (document.ParseInsitu(chunk.memory).HasParseError())
				{
					Log(CString("解析转发地址响应的Json文本失败！"), ERR);
				}
				else if (document["Ret"].IsNumber() && document["Ret"].IsInt())
				{
					auto ret_val = document["Ret"].GetInt();
					if (document.HasMember("Msg") && document["Msg"].IsString())
					{
						memcpy_s(msg, document["Msg"].GetStringLength(), document["Msg"].GetString(), strlen(msg));
					}
					Log(CString("OK!"));
					return ret_val;
				}
				else
				{
					Log(CString("没有获取的有效的返回值！"), WARNING);
				}
			}
			/* always cleanup */
			curl_easy_cleanup(curl);
			curl_slist_free_all(header_chunk);
			free(chunk.memory);
			delete buffer;
		}
	}
	else
	{
		Log(CString("没有启用事件上报插件EReport！"), WARNING);
		return 0;
	}


	//返回值-1 已收到信息但拒绝处理
	//返回0 没有收到信息或不被处理
	//返回1 被处理完毕,继续执行其他插件
	//返回2 被处理完毕,阻塞信息不再处理其他插件

	//特殊返回值:
	//0 忽略/取消该事件(如被添加好友 申请加入群
	//10 同意/批准该事件(如被添加好友 申请加入群
	//20 不同意/拒绝该事件(如被添加好友 申请加入群
	//30 单向同意该事件(仅用于被添加好友
	return -1;
}
///该函数收到所有通信信息  返回值-1 已收到信息但拒绝处理   返回0 没有收到信息或不被处理 返回1 被处理完毕,继续执行其他插件 返回2 被处理完毕,阻塞信息不再处理其他插件
/// 框架有可能登录多个QQ  该信息是来自此QQ
///1 UDP收到的原始信息 2 UDP发送的信息
///经过QQApi_Tea加密的通信原文
///用于QQ相关的各种网页接口操作,可直接提交替代登录
/// 通信包所用的加密秘钥
///登录网页服务用的秘钥'
dllexp int __stdcall Message(char *qq, int type, char *rawmsg, char *cookie, char *sessionkey, char *clientkey)
{

	//返回值-1 已收到信息但拒绝处理
	//返回0 没有收到信息或不被处理
	//返回1 被处理完毕,继续执行其他插件
	//返回2 被处理完毕,阻塞信息不再处理其他插
	return 2;
}
