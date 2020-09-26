// Translator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Translator.h"
#include "transdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTranslatorApp

BEGIN_MESSAGE_MAP(CTranslatorApp, CWinApp)
	//{{AFX_MSG_MAP(CTranslatorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTranslatorApp construction

CTranslatorApp::CTranslatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTranslatorApp object

CTranslatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTranslatorApp initialization

BOOL CTranslatorApp::InitInstance()
{

	CTransDlg *transDlg = new CTransDlg;
	m_pMainWnd = transDlg;
	transDlg->Create(IDD_DIALOG_TRANS);
	transDlg->ShowWindow(SW_SHOW);

	RECT rcTemp;
	transDlg->GetWindowRect(&rcTemp);
	int nTemp = GetSystemMetrics(SM_CXSCREEN);
	int nRcHeight = rcTemp.bottom - rcTemp.top;
	int nRcWidth = rcTemp.right - rcTemp.left;
	rcTemp.left = nTemp - 20 - nRcWidth;
	rcTemp.right = nTemp - 20;
	rcTemp.top = 10;
	rcTemp.bottom = 10 + nRcHeight;
	transDlg->MoveWindow(&rcTemp);
	transDlg->SetWindowText("Translator");
	HICON hIcon = LoadIcon(IDR_MAINFRAME);
	transDlg->SetIcon(hIcon, false);
	
	transDlg->SetTimer(1002, 500, NULL);
	::SetWindowPos(transDlg->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	CTransWnd::TransWndInit();

	CFont *fontInput = new CFont;
	CFont *fontOutput = new CFont;
	fontInput->CreatePointFont(160, "Microsoft YaHei");
	fontOutput->CreatePointFont(120, "Microsogt YaHei");
	CWnd *inputEdit = transDlg->GetDlgItem(IDC_EDIT_INPUT);
	CWnd *outputEdit = transDlg->GetDlgItem(IDC_EDIT_OUTPUT);
	inputEdit->SetFont(fontInput);
	outputEdit->SetFont(fontOutput);

	if (!RegisterHotKey(transDlg->m_hWnd, 1001, MOD_CONTROL | MOD_SHIFT, 0x49)) {
		CTransWnd::SetOutTxt("啊噢!注册快捷键失败啦!\r\n\r\n可能你已经打开了另外一个相同的窗口啦!\r\n\r\n\r\n\r\n\r\n              Made by Davies");
	}

	return true;
}
