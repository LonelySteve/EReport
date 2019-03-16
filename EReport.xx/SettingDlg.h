#pragma once


// SettingDlg 对话框

class SettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SettingDlg)

public:
	SettingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SettingDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 转发接口地址
	CString m_tran_address;
	// 是否启用事件转发
	BOOL m_enable_tran;
	virtual BOOL OnInitDialog();
	CButton m_checkbox_enable_tran;
	afx_msg void OnTranEnableBnClickedCheck();
	CEdit m_edit_tran_address;
};
