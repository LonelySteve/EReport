// SettingDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "EReport.xx.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// SettingDlg 对话框

IMPLEMENT_DYNAMIC(SettingDlg, CDialogEx)

SettingDlg::SettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTING_DIALOG, pParent)
	, m_tran_address(_T(""))
	, m_enable_tran(FALSE)
{

}

SettingDlg::~SettingDlg()
{
}

void SettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_tran_address);
	DDV_MaxChars(pDX, m_tran_address, 2048);
	DDX_Check(pDX, IDC_CHECK1, m_enable_tran);
	DDX_Control(pDX, IDC_CHECK1, m_checkbox_enable_tran);
	DDX_Control(pDX, IDC_EDIT1, m_edit_tran_address);
}


BEGIN_MESSAGE_MAP(SettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &SettingDlg::OnTranEnableBnClickedCheck)
END_MESSAGE_MAP()


// SettingDlg 消息处理程序


BOOL SettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 在此添加额外的初始化
	m_checkbox_enable_tran.SetCheck(m_enable_tran);
	if (!m_enable_tran)
	{
		// 禁用编辑框
		m_edit_tran_address.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void SettingDlg::OnTranEnableBnClickedCheck()
{
	if (m_enable_tran)
	{
		// 禁用编辑框
		m_edit_tran_address.EnableWindow(FALSE);
		m_enable_tran = FALSE;
	}
	else
	{
		// 启用编辑框
		m_edit_tran_address.EnableWindow(TRUE);
		m_enable_tran = TRUE;
	}
}
