// Translator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Translator.h"
#include "translatordlg.h"
#include "InputEdit.h"

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
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	
	CTransDlg *transDlg = new CTransDlg;
	m_pMainWnd = transDlg;
	transDlg->Create(IDD_TRANSDLG);
	transDlg->ShowWindow(SW_SHOW);
	transDlg->SetTimer(564, 500, GetClipboardText);
	transDlg->SetWindowText("Translator");

	if (!RegisterHotKey(transDlg->m_hWnd, 1001, MOD_CONTROL | MOD_SHIFT, 0x49)) {
		AfxMessageBox("register hot key failed!");
	}


	CInputEdit *inputEdit = new CInputEdit;
	CRect rcInputEdit(10, 50, 280, 80);
	inputEdit->Create(ES_AUTOHSCROLL  | WS_CHILD | WS_VISIBLE, rcInputEdit, m_pMainWnd, 1234);
	inputEdit->ShowWindow(SW_SHOW);
	inputEdit->SetFocus();

	CFont *fontInputEdit = new CFont;
	fontInputEdit->CreatePointFont(160, "Microsoft YaHei");
	inputEdit->SetFont(fontInputEdit);
	
	CFont *fontOutputEdit = new CFont;
	fontOutputEdit->CreatePointFont(120, "Microsoft YaHei");
	m_pMainWnd->GetDlgItem(IDC_EDIT_OUTPUT)->SetFont(fontOutputEdit);

	return true;
}
