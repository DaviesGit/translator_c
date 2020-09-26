
// Translator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Translator.h"
#include "TransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTranslatorApp

BEGIN_MESSAGE_MAP(CTranslatorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTranslatorApp construction

CTranslatorApp::CTranslatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTranslatorApp object

CTranslatorApp theApp;


// CTranslatorApp initialization

BOOL CTranslatorApp::InitInstance()
{
	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CTransDlg* dlg = new CTransDlg();
	m_pMainWnd = dlg;
	dlg->Create(IDD_TRANS_DLG);
	dlg->ShowWindow(SW_SHOW);

	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return true;
}

