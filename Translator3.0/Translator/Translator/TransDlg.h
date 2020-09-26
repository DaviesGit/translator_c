
// TranslatorDlg.h : header file
//

#pragma once
#include "afxwin.h"

class CInEdit : public CEdit {

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CTransThread {
public:
	static UINT __cdecl TransThread(LPVOID pParam);
	static UINT __cdecl PronuThread(LPVOID pParam);
};

struct TransParam {
	int status;
	const WCHAR *szEn;
};


// CTranslatorDlg dialog
class CTransDlg : public CDialogEx
{
// Construction
public:
	CTransDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TRANS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTrans();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnEnChangeEditOutput();
	CInEdit inEdit;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

class CTask {
public:
	CTask();
	bool GetTask(CString &strTask);
	bool PostTask(const WCHAR* szTask);
	void ClearTask();

private:
	CString TaskQueue[10];
	int nGetPoint;
	int nPostPoint;
};

class CBgThr {
public:
	static bool StartThread();

private:
	static UINT __cdecl BgThread(LPVOID pParam);
	static bool GetLine(const WCHAR* szStr, CString &strLine,const int nLine);
};

class CWords {
public:
	static bool GetNextWord(CString &strWord, CString &strTimes);
	static bool PostWord(const WCHAR* szWord);
	static bool RemoveWord(const WCHAR* szWord);
	static bool GetWordTimes(const WCHAR* szWord, CString &strTimes);
private:
	static bool RefreshIndex();
	static bool InitPath();
	
	static CString strPath;
};

struct WndInit {
	static bool whePronu, wheAutoHide, wheIdle, wheDlgInit, wheSetWndPos;
	static int nRepeatTime;
};