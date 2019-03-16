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
	// ��ʼ��Curl
	curl_global_init(CURL_GLOBAL_ALL);
	// ���������� Ĭ��ת���ӿ�Ϊ http://127.0.0.1:8050
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
	MessageBox(0, "�¼�ת���ϱ����@JLoeve���ο���mypcqq c++ sdk��ʵ��", "��ʾ", 0);
}

dllexp const char * __stdcall info()
{
	return "�¼�ת���ϱ����";
}

dllexp int __stdcall end()
{
	// д������
	WritePrivateProfileString("EReport", "address", tran_address.GetString(), ".\\Set.ini");
	WritePrivateProfileString("EReport", "enable", enable_status ? "1" : "0", ".\\Set.ini");
	// �ͷ�curlȫ����Դ
	curl_global_cleanup();
	Log(CString("��ж���¼��ϱ������"));
	// �ͷ���������Դ
	tran_address.ReleaseBuffer();
	// ���ط�0�ɹ� ��ɲ����ػ�Ϊ��
	return 1;
}

/**
/* @brief EventFun
/* @param qq ��QQ��¼���������ʶ�����ĸ�Q
/* @param msgtype ��ϢΨһ��ʶ-1 δ�����¼� 1 ������Ϣ 2,Ⱥ��Ϣ 3,��������Ϣ 4,��ʱ�Ự 1001,����Ӻ��� 1002,��������״̬�ı� 1003 ��ɾ������ 1004 ǩ����� 2001 ĳ���������Ⱥ 2002 ĳ�˱��������Ⱥ 2003 �ұ��������Ⱥ 2005 ĳ�˱���׼������Ⱥ 2006 ĳ���˳�Ⱥ  2007 ĳ�˱������Ƴ�Ⱥ 2008 ĳȺ����ɢ 2009 ĳ�˳�Ϊ����Ա 2010 ĳ�˱�ȡ������Ա 2011 Ⱥ��Ƭ�䶯 2012 Ⱥ���䶯//��δ���� 2013 Ⱥ����䶯
/* @param msgctype �������롢��������Ⱥ�¼��¸�ֵΪ1ʱ������Ϊ������Ա
/* @param msgsource ��Ϣ��Դͷ  Ⱥ��,����QQ,������ID,��ʱ�Ự����QQ��
/* @param dop ��������������Ϣ�Ķ��� T��ʱΪT�˵Ĺ���ԱQQ
/* @param bep ��������������Ϣ�Ķ��� T��ʱΪ��T�����QQ
/* @param msg �������������Ϣ����  ������ȺʱΪ��Ⱥ����,��Ӻ���Ϊ������Ϣ,T��֮���Ϊ��
/* @param rawmsg �������ܺ�ķ���ֽ����ݻ�json�ṹ��Ϣ
/* @param backptr ��Ϣ�ش�ָ�롣 ����������ķ��ظ����ı���Ϣ  �ܾ���������ʱ��Ϊ�ܾ����� ��ʽ:��д���ڴ�("����",��_��Ϣ�ش��ı�ָ��_Out)
/* @return �ú���������Ӧ���ָ����ĻỰ��ϵͳ�¼�
*/
dllexp int __stdcall EventFun(char *qq, int msgtype, int msgctype, char *msgsource, char *dop, char *bep, char *msg, char *rawmsg, char *backptr)
{
	CString tip_str;
	tip_str.Format("ת���¼���%s", tran_address);
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
			// ʹ�� POST �ύ��
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
				tmp.Format("ת����ַ��Ӧ����������Ϣ��%s", err_msg);
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
					Log(CString("����ת����ַ��Ӧ��Json�ı�ʧ�ܣ�"), ERR);
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
					Log(CString("û�л�ȡ����Ч�ķ���ֵ��"), WARNING);
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
		Log(CString("û�������¼��ϱ����EReport��"), WARNING);
		return 0;
	}


	//����ֵ-1 ���յ���Ϣ���ܾ�����
	//����0 û���յ���Ϣ�򲻱�����
	//����1 ���������,����ִ���������
	//����2 ���������,������Ϣ���ٴ����������

	//���ⷵ��ֵ:
	//0 ����/ȡ�����¼�(�类��Ӻ��� �������Ⱥ
	//10 ͬ��/��׼���¼�(�类��Ӻ��� �������Ⱥ
	//20 ��ͬ��/�ܾ����¼�(�类��Ӻ��� �������Ⱥ
	//30 ����ͬ����¼�(�����ڱ���Ӻ���
	return -1;
}
///�ú����յ�����ͨ����Ϣ  ����ֵ-1 ���յ���Ϣ���ܾ�����   ����0 û���յ���Ϣ�򲻱����� ����1 ���������,����ִ��������� ����2 ���������,������Ϣ���ٴ����������
/// ����п��ܵ�¼���QQ  ����Ϣ�����Դ�QQ
///1 UDP�յ���ԭʼ��Ϣ 2 UDP���͵���Ϣ
///����QQApi_Tea���ܵ�ͨ��ԭ��
///����QQ��صĸ�����ҳ�ӿڲ���,��ֱ���ύ�����¼
/// ͨ�Ű����õļ�����Կ
///��¼��ҳ�����õ���Կ'
dllexp int __stdcall Message(char *qq, int type, char *rawmsg, char *cookie, char *sessionkey, char *clientkey)
{

	//����ֵ-1 ���յ���Ϣ���ܾ�����
	//����0 û���յ���Ϣ�򲻱�����
	//����1 ���������,����ִ���������
	//����2 ���������,������Ϣ���ٴ���������
	return 2;
}
