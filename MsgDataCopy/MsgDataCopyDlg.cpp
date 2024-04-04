
// MsgDataCopyDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MsgDataCopy.h"
#include "MsgDataCopyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMsgDataCopyDlg 对话框

CMsgDataCopyDlg* g_pCMsgDataCopyDlg = NULL;
static UINT g_nIndex = 0;

#define WM_COM_DATA_POCESS  (WM_USER + 5001)

CMsgDataCopyDlg::CMsgDataCopyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MSGDATACOPY_DIALOG, pParent)
	, m_cstrDelayTime(_T(""))
	, m_bIsRunning(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFunc = NULL;
}

void CMsgDataCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, edit_WindowText, m_strWindowText);
	DDX_Text(pDX, edit_targetWindowText, m_strTargetWindowText);
	DDX_Text(pDX, edit_sendContent, m_strSendContent);
	DDX_Text(pDX, edit_acceptContent, m_strAcceptContent);
	DDX_Control(pDX, list_log, m_listLog);
	DDX_Text(pDX, IDC_EDIT_DelayTime, m_cstrDelayTime);
}

BEGIN_MESSAGE_MAP(CMsgDataCopyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(btn_modifyWinText, &CMsgDataCopyDlg::OnBnClickedmodifywintext)
	ON_BN_CLICKED(btn_send, &CMsgDataCopyDlg::OnBnClickedsend)
	ON_BN_CLICKED(btn_startListen, &CMsgDataCopyDlg::OnBnClickedstartlisten)
	ON_BN_CLICKED(btn_stopListen, &CMsgDataCopyDlg::OnBnClickedstoplisten)
	ON_LBN_DBLCLK(list_log, &CMsgDataCopyDlg::OnLbnDblclklog)
	ON_MESSAGE(WM_COPYDATA, &CMsgDataCopyDlg::OnCopyData)
	ON_BN_CLICKED(IDC_BUTTON_NewInstance, &CMsgDataCopyDlg::OnBnClickedButtonNewinstance)
	ON_BN_CLICKED(IDC_BUTTON_WhleSend, &CMsgDataCopyDlg::OnBnClickedButtonWhlesend)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMsgDataCopyDlg 消息处理程序

BOOL CMsgDataCopyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_listLog.ResetContent();
	g_pCMsgDataCopyDlg = this;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMsgDataCopyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMsgDataCopyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMsgDataCopyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMsgDataCopyDlg::InitParam(const CString& strWindowText, const CString& strTargetWindowText)
{
	m_strWindowText = strWindowText;
	m_strTargetWindowText = strTargetWindowText;
	this->SetWindowTextW(m_strWindowText);
	SetDlgItemText(edit_WindowText, m_strWindowText);
	SetDlgItemText(edit_targetWindowText, m_strTargetWindowText);
}

BOOL CMsgDataCopyDlg::SendDataByWmCopyData(const CString& strText)
{
	UpdateData(TRUE);
	CWnd* pDisplayWnd = CWnd::FindWindow(NULL, m_strTargetWindowText);//
	if (pDisplayWnd == NULL) {
		CString str = _T("未找到目标窗口：") + m_strTargetWindowText;
		KillTimer(TIMER_ID);
		m_bIsRunning = FALSE;
		GetDlgItem(IDC_BUTTON_WhleSend)->SetWindowText(_T("循环发送"));
		MessageBox(CString(str).GetBuffer());
		if (this->IsWindowVisible()) {
			m_listLog.AddString(str);
			AfxMessageBox(str, MB_TOPMOST);
		}
		return FALSE;
	}
	COPYDATASTRUCT cds;
	memset(&cds, 0, sizeof(cds));
	cds.dwData = 0;
	cds.cbData = (strText.GetLength() + 1) * sizeof(WCHAR); // +1 for the NULL
	//ansi转成unicode字符串发送给打标软件;
	//_bstr_t tmp = (const TCHAR*)strText;
	CString s = strText;
	cds.lpData = (LPVOID)(LPCWSTR)s.GetBuffer();
	::SendMessage(pDisplayWnd->GetSafeHwnd(), WM_COPYDATA, (WPARAM)0, (LPARAM)&cds);
	return TRUE;
}

LRESULT CMsgDataCopyDlg::OnCopyData(WPARAM wParam, LPARAM lParam)
{
	PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
	LPCWSTR szNewString = (LPCWSTR)(pcds->lpData);
	//_bstr_t tmp = szNewString;
	//const char* pData = (const char*)tmp;
	//CStringA strDataRcv(pData);
	m_strAcceptContent = CString(szNewString);
	//此处要PostMessage到本窗口处理m_strDataRcv，切记不能阻塞;
	if (NULL != this->GetSafeHwnd())
	{
		::PostMessage(this->m_hWnd, WM_COM_DATA_POCESS, 0, (LPARAM)m_strAcceptContent.GetBuffer());
	}
	if (this->IsWindowVisible()) {
		GetDlgItem(edit_acceptContent)->SetWindowText(m_strAcceptContent);
		m_listLog.AddString(TEXT("接收到数据：") + m_strAcceptContent);
		if (500 < m_listLog.GetCount())
			m_listLog.DeleteString(0);
	}
	return 1L;
}

LRESULT CMsgDataCopyDlg::PocessData(WPARAM wparam, LPARAM lparam)
{
	CString strRecv = CString((TCHAR*)lparam);
	if (m_pFunc) {
		m_pFunc(strRecv);
	}
	return 1L;
}

BOOL CMsgDataCopyDlg::StartListen()
{
	return TRUE;
}

BOOL CMsgDataCopyDlg::StopListen()
{
	return TRUE;
}

void CMsgDataCopyDlg::OnBnClickedmodifywintext()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	this->SetWindowText(m_strWindowText);
	if (this->IsWindowVisible()) {
		m_listLog.AddString(TEXT("修改本窗口标题为：") + m_strWindowText);
	}
}


void CMsgDataCopyDlg::OnBnClickedsend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	SendDataByWmCopyData(m_strSendContent);
	if (this->IsWindowVisible())
	{
		m_listLog.AddString(_T("点击发送，内容：") + m_strSendContent);
	}
}


void CMsgDataCopyDlg::OnBnClickedstartlisten()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL b = StartListen();
	if (this->IsWindowVisible()) {
		GetDlgItem(btn_startListen)->EnableWindow(FALSE);
		GetDlgItem(btn_stopListen)->EnableWindow(TRUE);
		m_listLog.AddString(_T("开始监听") + b ? TEXT("成功") : TEXT("失败"));
	}
}


void CMsgDataCopyDlg::OnBnClickedstoplisten()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL b = StopListen();
	if (this->IsWindowVisible()) {
		GetDlgItem(btn_startListen)->EnableWindow(TRUE);
		GetDlgItem(btn_stopListen)->EnableWindow(FALSE);
		m_listLog.AddString(TEXT("停止监听 ") + b ? TEXT("成功") : TEXT("失败"));
	}
}


void CMsgDataCopyDlg::OnLbnDblclklog()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listLog.ResetContent();
}


BOOL CMsgDataCopyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam)
		{
		case VK_RETURN: //回车;
		case VK_ESCAPE: //ESC;
		case VK_SPACE:
			return TRUE;
		}
	}
	if ((WM_SYSKEYDOWN == pMsg->message) // WM_SYSKEYDOWN  表示ALT键按下
		&& (VK_F4 == pMsg->wParam))
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMsgDataCopyDlg::OnBnClickedButtonNewinstance()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CWnd* p_cWind = new CMsgDataCopyDlg(AfxGetMainWnd());
	(CMsgDataCopyDlg*)p_cWind->ShowWindow(SW_SHOW);*/

	/*CMsgDataCopyDlg* p_cWind = new CMsgDataCopyDlg();
	p_cWind->ShowWindow(SW_SHOW);*/

	CMsgDataCopyDlg* p_cWind = new CMsgDataCopyDlg;
	p_cWind->Create(IDD_MSGDATACOPY_DIALOG);
	p_cWind->ShowWindow(SW_SHOW);
}


void CMsgDataCopyDlg::OnBnClickedButtonWhlesend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bIsRunning)
	{
		KillTimer(TIMER_ID);
		m_bIsRunning = FALSE;
		GetDlgItem(IDC_BUTTON_WhleSend)->SetWindowText(_T("循环发送"));
	}
	else
	{
		CString cstrTime = m_cstrDelayTime;
		UINT iDelayTime = 0;
		if (_stscanf_s(cstrTime, _T("%u"), &iDelayTime) != 1)
		{
			GetDlgItem(IDC_EDIT_DelayTime)->SetWindowText(_T(""));
			AfxMessageBox(_T("设置循环时间失败，设置发送时间可能不是整数"));
			return;
		}
		SetTimer(TIMER_ID, iDelayTime, NULL);
		m_bIsRunning = TRUE;
		GetDlgItem(IDC_BUTTON_WhleSend)->SetWindowText(_T("停止"));
	}
	UpdateData(FALSE);
}


void CMsgDataCopyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData();
	if (nIDEvent == TIMER_ID)
	{
		bool bSendSuc = SendDataByWmCopyData(m_strSendContent);
		if (!bSendSuc)
		{
			KillTimer(TIMER_ID);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
