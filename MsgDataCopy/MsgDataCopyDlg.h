
// MsgDataCopyDlg.h: 头文件
//

#pragma once

typedef bool(*CallBack_Func)(CString&);
// CMsgDataCopyDlg 对话框
class CMsgDataCopyDlg : public CDialogEx
{
// 构造
public:
	CMsgDataCopyDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSGDATACOPY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitParam(const CString& strWindowText, const CString& strTargetWindowText);
	BOOL SendDataByWmCopyData(const CString& strText);
	afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT PocessData(WPARAM wparam, LPARAM lparam);      //处理接收的结果;
	BOOL StartListen();
	BOOL StopListen();
	afx_msg void OnBnClickedmodifywintext();

public:
	CallBack_Func m_pFunc;
	CString m_strWindowText;
	CString m_strTargetWindowText;
	CString m_strAcceptContent;
	CString m_strSendContent;
	CListBox m_listLog;
	BOOL m_bIsRunning;
	static const UINT_PTR TIMER_ID = 1;		//计时器ID
	afx_msg void OnBnClickedsend();
	afx_msg void OnBnClickedstartlisten();
	afx_msg void OnBnClickedstoplisten();
	afx_msg void OnLbnDblclklog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonNewinstance();
	CString m_cstrDelayTime;
	afx_msg void OnBnClickedButtonWhlesend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
