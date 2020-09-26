
// TranslatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Translator.h"
#include "TransDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "Trans.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransDlg dialog
CTask task;


CTransDlg::CTransDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_TRANS);
}

void CTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, inEdit);
}

BEGIN_MESSAGE_MAP(CTransDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TRANS, &CTransDlg::OnBnClickedButtonTrans)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_HOTKEY()
	ON_EN_CHANGE(IDC_EDIT_OUTPUT, &CTransDlg::OnEnChangeEditOutput)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTransDlg message handlers

BOOL CTransDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	RECT rcTemp;
	GetWindowRect(&rcTemp);
	int nTemp = GetSystemMetrics(SM_CXSCREEN);
	int nRcHeight = rcTemp.bottom - rcTemp.top;
	int nRcWidth = rcTemp.right - rcTemp.left;
	rcTemp.left = nTemp - nRcWidth;
	rcTemp.right = nTemp;
	rcTemp.top = 10;
	rcTemp.bottom = 10 + nRcHeight;
	MoveWindow(&rcTemp);

	

	CFont *fontInput = new CFont;
	CFont *fontOutput = new CFont;
	fontInput->CreatePointFont(160, _T("Microsoft YaHei"));
	fontOutput->CreatePointFont(120, _T("Microsogt YaHei"));
	CWnd *inputEdit = GetDlgItem(IDC_EDIT_INPUT);
	CWnd *outputEdit = GetDlgItem(IDC_EDIT_OUTPUT);
	inputEdit->SetFont(fontInput);
	outputEdit->SetFont(fontOutput);

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	if (!RegisterHotKey(m_hWnd, 1001, MOD_CONTROL | MOD_SHIFT, 0x49)) {
#ifdef DEBUG
		AfxMessageBox(_T("register Hotkey failed!"));
#endif // DEBUG
	}

	CBgThr::StartThread();
	WndInit::wheDlgInit = true;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTransDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTransDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTransDlg::OnBnClickedButtonTrans()
{
	// TODO: Add your control notification handler code here
	CEdit* editInput = (CEdit*)GetDlgItem(IDC_EDIT_INPUT);
	CString strEn;
	editInput->GetWindowTextW(strEn);
	if (_T("") != strEn) {
		task.PostTask(strEn);
	}

}



void CTransDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	PostQuitMessage(0);
	// TODO: Add your message handler code here
}



void CTransDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnClose();
	DestroyWindow();
}



CTask::CTask() {
	nGetPoint = 0;
	nPostPoint = 0;
}

bool CTask::GetTask(CString &strTask) {
	if (nGetPoint == nPostPoint) {
		return false;
	}
	strTask = TaskQueue[nGetPoint];
	++nGetPoint;
	if (9 < nGetPoint) {
		nGetPoint = 0;
	}
	return true;
}

bool CTask::PostTask(const WCHAR* szTask) {
	int nTemp = nPostPoint;
	++nTemp;
	if (9 < nTemp) {
		nTemp = 0;
	}
	if (nTemp == nGetPoint) {
		return false;
	}
	TaskQueue[nPostPoint] = szTask;
	++nPostPoint;
	if (9 < nPostPoint) {
		nPostPoint = 0;
	}
	return true;
}

void CTask::ClearTask() {
	nGetPoint = nPostPoint;
}

void CTransDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: Add your message handler code here and/or call default
	if (1001 == nHotKeyId) {
		task.ClearTask();
		this->SetForegroundWindow();
		CWnd *inputEdit = this->GetDlgItem(IDC_EDIT_INPUT);
		inputEdit->SetFocus();
		inputEdit->SetWindowText(_T(""));
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}



bool CBgThr::StartThread() {
	CWinThread *thrBg = AfxBeginThread(BgThread, NULL);
	return true;
}

UINT __cdecl CBgThr::BgThread(LPVOID pParam) {
	CString strTask;
	CString strTimes;
	for( ; ; ) {
		if (task.GetTask(strTask)) {
			bool wheCh = false;
			for (int n = 0; n < strTask.GetLength(); ++n) {
				if (127 < strTask[n]) {
					wheCh = true;
					break;
				}
			}
			if (wheCh) {
				CString strRes;
				if (!CTrans::ChToEn(strTask, strRes)) {
					AfxGetMainWnd()->GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(_T("internet error!"));
					continue;
				}
				AfxGetMainWnd()->GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(strRes);
			}
			else {
				TransParam trans, pronu;
				CString strPronu[4];
				trans.status = 0;
				trans.szEn = strTask;
				pronu.status = 0;
				pronu.szEn = strTask;

				if (CWords::GetWordTimes(strTask, strTimes)) {
					AfxGetMainWnd()->SetWindowTextW(_T("Word: ") + strTask + _T(" >times: ") + strTimes);
				}

				AfxBeginThread(CTransThread::TransThread, &trans);
				AfxBeginThread(CTransThread::PronuThread, &pronu);
				
				while (0 == trans.status || 0 == pronu.status) {
					Sleep(100);
				}

			}
		}
		else {
			if (!WndInit::wheIdle) {
				Sleep(100);
				continue;
			}
			if (WndInit::whePronu) {
				int n;
				int nRepeat = WndInit::nRepeatTime / 100;
				for (n = 0; n < nRepeat; ++n) {
					if (WndInit::wheIdle) {
						Sleep(100);
					}
					else {
						n = nRepeat;
					}
				}
				if (!WndInit::wheIdle) {
					continue;
				}
				if (!CWords::GetNextWord(strTask, strTimes)) {
					Sleep(100);
					continue;
				}
				AfxGetMainWnd()->SetWindowText(_T("Word: ") + strTask + _T(" >times: ") + strTimes);
				CString strRes;
				if (!CTrans::GetTransRes(strTask, strRes)) {
					AfxGetMainWnd()->GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(_T("internet error!"));
					continue;
				}
				AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)->SetWindowTextW(strTask);
				((CEdit*)(AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)))->SetSel(-1);
				AfxGetMainWnd()->GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(strRes);
				CString strPronu[4];
				if (!CTrans::GetPronu(strTask, strPronu)) {
					continue;
				}
				for (n = 0; n < 4; ++n) {
					static MCI_OPEN_PARMS mciOpen;
					mciOpen.lpstrDeviceType = _T("MPEGAudio");
					if (strPronu[n].IsEmpty()) {
						int n1 = -1;
						do {
							++n1;
							mciOpen.lpstrElementName = strPronu[n1];
						} while (strPronu[n1].IsEmpty() && 3 > n1);
					}
					else {
						mciOpen.lpstrElementName = strPronu[n];
					}
					if (!WndInit::wheIdle) {
						continue;
					}
					mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
					mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_WAIT, (DWORD)&mciOpen);
					mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
					Sleep(100);
				}
			}
			else {
				Sleep(100);
			}
		}
	}
	return 0;
}


bool CBgThr::GetLine(const WCHAR* szStr, CString &strLine,const int nLine) {
	int nLoca1;
	int nLoca2=-1;
	int nCount = 0;
	CString strStr = szStr;
	do {
		nLoca1 = nLoca2;
		++nLoca1;
		nLoca2 = strStr.Find(_T("\r\n"), nLoca1);
		if (-1 != nLoca2) {
			++nCount;
		}
		else {
			return false;
		}
	} while (nLine != nCount);
	strLine = strStr.Mid(nLoca1, nLoca2 - nLoca1);
	return true;
}

void CTransDlg::OnEnChangeEditOutput()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


BEGIN_MESSAGE_MAP(CInEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()


void CInEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	
	switch (nChar) {
	case 1: //control + a
		if (255 == (unsigned short int)GetKeyState(VK_CONTROL) / 256) {
			((CEdit*)(AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)))->SetSel(0, -1);
			return;
		}
		break;
	case 3: //control + c change time
		if (255 == (unsigned short int)GetKeyState(VK_CONTROL) / 256)
		{
			CString strText;
			AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strText);
			int nReapt = _wtoi(strText);
			if (100 > nReapt) {
				AfxGetMainWnd()->SetWindowTextW(_T("input value not invalid!"));
			}
			else {
				WndInit::nRepeatTime = nReapt;
				AfxGetMainWnd()->SetWindowTextW(_T("set repeat time:") + strText + _T("ms"));
			}
			return;
		}
		break;
	case 2: //control + b begin
		break;
	case 20: //control + t termination
		break;
	case 8: //control + H hide
		if (255 == (unsigned short int)GetKeyState(VK_CONTROL) / 256) {
			if (WndInit::wheAutoHide) {
				WndInit::wheAutoHide = false;
				AfxGetMainWnd()->SetWindowTextW(_T("not automatic hide"));
			}
			else {
				WndInit::wheAutoHide = true;
				AfxGetMainWnd()->SetWindowTextW(_T("automatic hide"));
			}
			return;
		} 
		break;
	case 14: //control + n
		break;
	case 16: //control + p pronunciatoin
		if (255 == (unsigned short int)GetKeyState(VK_CONTROL) / 256){
			if (WndInit::whePronu) {
				WndInit::whePronu = false;
				AfxGetMainWnd()->SetWindowTextW(_T("stop pronunciation!"));
			}
			else {
				WndInit::whePronu = true;
				AfxGetMainWnd()->SetWindowTextW(_T("begin pronunciation!"));
			}
			return;
		}
		break;
	case 18: //control + r remove
		if (255 == (unsigned short int)GetKeyState(VK_CONTROL) / 256) {
			CString strWndText;
			AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strWndText);
			if (!strWndText.IsEmpty()) {
				CWords::RemoveWord(strWndText);
			}
			return;
		}
		break;
	case 19: //control + s save
		if (255 == (unsigned short int)GetKeyState(VK_CONTROL) / 256) {
			CString strWndText;
			AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strWndText);
			if (!strWndText.IsEmpty()) {
				CWords::PostWord(strWndText);
			}
			return;
		}
		break;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


bool CWords::GetNextWord(CString &strWord, CString &strTimes) {
	CFile file;
	UINT nIndexLen;
	UINT nAmount, nLoca, nLoca0, nLen;
	if (strPath.IsEmpty()) {
		InitPath();
	}
	if (!file.Open(strPath + _T("\\Index"), CFile::modeRead)) {
		return false;
	}
	nIndexLen = (int)file.GetLength();
	unsigned char *index = new unsigned char[nIndexLen];
	file.Read(index, nIndexLen);
	file.Close();
	nAmount = index[0] * (256 * 256 * 256) + index[1] * (256 * 256) + index[2] * (256) + index[3];
	srand((UINT)time(NULL));
	if (8 >= nIndexLen) {
		return false;
	}
	do {
		nLoca = (rand() + 1) * 8;
	} while (nLoca >= nIndexLen);
	nLoca0 = index[nLoca++] * (256 * 256 * 256);
	nLoca0 += index[nLoca++] * (256 * 256);
	nLoca0 += index[nLoca++] * (256);
	nLoca0 += index[nLoca++];
	nLen = index[nLoca++] * (256 * 256 * 256);
	nLen += index[nLoca++] * (256 * 256);
	nLen += index[nLoca++] * (256);
	nLen += index[nLoca++];
	if (!file.Open(strPath + _T("\\Words.txt"), CFile::modeRead)) {
		delete[] index;
		return false;
	}
	file.Seek(nLoca0, CFile::begin);
	char *buffer = new char[nLen + 1];
	file.Read(buffer, nLen);
	buffer[nLen] = 0;
	strWord = buffer;
	file.Seek(nLoca0 + nLen + 1, CFile::begin);
	file.Read(buffer, 1);
	buffer[1] = 0;
	strTimes = buffer;
	file.Close();
	delete[] index;
	delete[] buffer;
	return true;
}

bool CWords::GetWordTimes(const WCHAR* szWord, CString &strTimes) {
	int nLoca;
	CStringA strAWord;
	CFile file;
	if (strPath.IsEmpty()) {
		InitPath();
	}
	if (!file.Open(strPath + _T("\\Words.txt"), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite)) {
		return false;
	}
	CStringA strATemp;
	int nCount = 0;
	char szTemp[8192];
	do {
		nCount = file.Read(szTemp, 8191);
		szTemp[nCount] = 0;
		strATemp += szTemp;
	} while (nCount);
	strAWord = "@:";
	strAWord += szWord;
	strAWord += "#";
	nLoca = strATemp.Find(strAWord);
	if (-1 == nLoca) {
		file.Close();
		strTimes = _T("0");
		return true;
	}
	file.Seek(nLoca + strAWord.GetLength(), CFile::begin);
	file.Read(szTemp, 1);
	file.Close();
	szTemp[1] = 0;
	strTimes = szTemp;
	return true;
}

bool CWords::PostWord(const WCHAR* szWord) {
	int nLoca;
	int nTimes;
	char times[2];
	CFile file;
	if (strPath.IsEmpty()) {
		InitPath();
	}
	if (!file.Open(strPath + _T("\\Words.txt"), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite)) {
		return false;
	}
	CStringA strATemp;
	int nCount = 0;
	char szTemp[8192];
	do {
		nCount = file.Read(szTemp, 8191);
		szTemp[nCount] = 0;
		strATemp += szTemp;
	} while (nCount);
	CStringA strAWord;
	strAWord = "@:";
	strAWord += szWord;
	strAWord += "#";
	
	nLoca = strATemp.Find(strAWord);
	if (-1 != nLoca) {
		nLoca += strAWord.GetLength();
		file.Seek(nLoca,CFile::begin);
		nTimes = atoi(strATemp.Mid(nLoca, 1));
		if (9 > nTimes) {
			++nTimes;
		}
		_itoa_s(nTimes, times, 10);
		AfxGetMainWnd()->SetWindowTextW((CString)szWord + _T(" ->times: ") + times);
		file.Write(times, 1);
		file.Close();
		RefreshIndex();
		return true;
	}
	else {
		strAWord += "1#\r\n";
		file.Write(strAWord, strAWord.GetLength());
		file.Close();
		AfxGetMainWnd()->SetWindowTextW((CString)szWord + _T(" ->times: 1"));
		RefreshIndex();
		return true;
	}
}

bool CWords::RemoveWord(const WCHAR* szWord) {

	int nLoca;
	int nTimes;
	int nStrLen;
	int nNewLen;
	CStringA strTimes;
	char times[2];
	CStringA strAWord;
	CFile file;
	if (strPath.IsEmpty()) {
		InitPath();
	}
	if (!file.Open(strPath + _T("\\Words.txt"), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite)) {
		return false;
	}
	CStringA strATemp;
	int nCount = 0;
	char szTemp[8192];
	do {
		nCount = file.Read(szTemp, 8191);
		szTemp[nCount] = 0;
		strATemp += szTemp;
	} while (nCount);
	strAWord = "@:";
	strAWord += szWord;
	strAWord += "#";
	nLoca = strATemp.Find(strAWord);
	if (-1 == nLoca) {
		file.Close();
		AfxGetMainWnd()->SetWindowTextW(_T("Not have this word!"));
		return false;
	}
	nStrLen = strAWord.GetLength();
	nLoca += nStrLen;
	strTimes = strATemp.Mid(nLoca, 1);
	nTimes = atoi(strTimes);
	if (1 >= nTimes) {
		nLoca -= nStrLen;
		file.Seek(nLoca, CFile::begin);
		nNewLen = strATemp.GetLength() - nStrLen - 4;
		strATemp = strATemp.Right(strATemp.GetLength() - nLoca - nStrLen - 4);
		file.Seek(nLoca, CFile::begin);
		file.Write(strATemp, strATemp.GetLength());
		file.SetLength(nNewLen);
		file.Close();
		AfxGetMainWnd()->SetWindowTextW((CString)szWord + _T(" ->Removed"));
		RefreshIndex();
	}
	else {
		--nTimes;
		_itoa_s(nTimes, times, 10);
		file.Seek(nLoca, CFile::begin);
		file.Write(times, 1);
		file.Close();
		AfxGetMainWnd()->SetWindowTextW((CString)szWord + _T(" ->times: ") + times);
		RefreshIndex();
	}
	return true;
}

bool CWords::RefreshIndex() {
	int nLoca1, nLoca2, nTemp1, nTemp2, nTemp3, nTemp4, nTemp5, nTemp6;
	int nCount, n;
	int nTimes;
	char *buffer = new char[1024 * 1024];
	int nBufferLoca;
	CFile file;
	if (strPath.IsEmpty()) {
		InitPath();
	}
	if (!file.Open(strPath + _T("\\Words.txt"), CFile::modeRead)) {
		delete[] buffer;
		return false;
	}
	CStringA strATemp;
	char szTemp[8192];
	do {
		nCount = file.Read(szTemp, 8191);
		szTemp[nCount] = 0;
		strATemp += szTemp;
	} while (nCount);
	file.Close();
	nLoca2 = 0;
	nCount = 0;
	nBufferLoca = 8;
	for( ; ; ) {
		nLoca1 = strATemp.Find("@:", nLoca2);
		if (-1 == nLoca1) {
			break;
		}
		nLoca2 = strATemp.Find("#", nLoca1 + 2);
		nTimes = atoi(strATemp.Mid(nLoca2 + 1, 1));
		nCount += nTimes;
		nTemp1 = nLoca1 + 2;
		nTemp2 = nLoca2 - nTemp1;
		for (n = 0; n < nTimes; ++n) {
			nTemp3 = nTemp1 / (256 * 256 * 256);
			nTemp4 = nTemp1 % (256 * 256 * 256);
			nTemp5 = nTemp4 % (256 * 256);
			nTemp6 = nTemp5 % (256);
			nTemp4 = nTemp4 / (256 * 256);
			nTemp5 = nTemp5 / (256);
			buffer[nBufferLoca++] = (char)(nTemp3);
			buffer[nBufferLoca++] = (char)(nTemp4);
			buffer[nBufferLoca++] = (char)(nTemp5);
			buffer[nBufferLoca++] = (char)(nTemp6);
			nTemp3 = nTemp2 / (256 * 256 * 256);
			nTemp4 = nTemp2 % (256 * 256 * 256);
			nTemp5 = nTemp4 % (256 * 256);
			nTemp6 = nTemp5 % (256);
			nTemp4 = nTemp4 / (256 * 256);
			nTemp5 = nTemp5 / (256);
			buffer[nBufferLoca++] = (char)(nTemp3);
			buffer[nBufferLoca++] = (char)(nTemp4);
			buffer[nBufferLoca++] = (char)(nTemp5);
			buffer[nBufferLoca++] = (char)(nTemp6);
		}
	}
	nTemp3 = nCount / (256 * 256 * 256);
	nTemp4 = nCount % (256 * 256 * 256);
	nTemp5 = nTemp4 % (256 * 256);
	nTemp6 = nTemp5 % (256);
	nTemp4 = nTemp4 / (256 * 256);
	nTemp5 = nTemp5 / (256);

	buffer[0] = (char)nTemp3;
	buffer[1] = (char)nTemp4;
	buffer[2] = (char)nTemp5;
	buffer[3] = (char)nTemp6;
	buffer[4] = 0;
	buffer[5] = 0;
	buffer[6] = 0;
	buffer[7] = 0;
	if (!file.Open(strPath + _T("\\Index"), CFile::modeCreate | CFile::modeWrite)) {
		delete[] buffer;
		return false;
	}
	file.Write(buffer, nBufferLoca);
	file.Close();
	delete[] buffer;
	return true;
}
CString CWords::strPath;
bool CWords::InitPath() {
	WCHAR szModuleName[8192];
	GetModuleFileName(NULL, szModuleName, 8191);
	strPath = szModuleName;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	return true;
}



void CTransDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	if (!nState) {
		WndInit::wheIdle = true;
		if (WndInit::wheAutoHide) {
			RECT rcTemp;
			GetWindowRect(&rcTemp);
			int nTemp = GetSystemMetrics(SM_CXSCREEN);
			int nRcWidth = rcTemp.right - rcTemp.left;
			if (rcTemp.left <= nTemp - nRcWidth) {
				rcTemp.left = nTemp - 150;
				rcTemp.right = nTemp + nRcWidth - 150;
				MoveWindow(&rcTemp);
			}
			SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			OutputDebugStringA("0\r\n");
		}
	}
	else if (1 == nState) {
			WndInit::wheIdle = false;
		OutputDebugStringA("1\r\n");
		if (WndInit::wheAutoHide) {
			RECT rcTemp;
			GetWindowRect(&rcTemp);
			int nTemp = GetSystemMetrics(SM_CXSCREEN);
			int nRcWidth = rcTemp.right - rcTemp.left;
			rcTemp.left = nTemp - nRcWidth;
			rcTemp.right = nTemp;
			MoveWindow(&rcTemp);
		}
	}
	else if (2 == nState) {
		WndInit::wheIdle = false;
		OutputDebugStringA("2\r\n");
	}
	// TODO: Add your message handler code here
}


void CTransDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if (!WndInit::wheDlgInit) {
		return;
	}
	CRect rectIn, rectOut;
	GetDlgItem(IDC_EDIT_INPUT)->GetClientRect(&rectIn);
	rectIn.right = cx;
	GetDlgItem(IDC_EDIT_INPUT)->MoveWindow(&rectIn);
	GetDlgItem(IDC_EDIT_OUTPUT)->GetClientRect(&rectOut);
	rectOut.top = rectIn.bottom;
	rectOut.right = cx;
	rectOut.bottom = cy;
	GetDlgItem(IDC_EDIT_OUTPUT)->MoveWindow(&rectOut);
}



bool WndInit::wheAutoHide = true;
bool WndInit::whePronu = true;
bool WndInit::wheIdle = true;
bool WndInit::wheDlgInit = false;
bool WndInit::wheSetWndPos = false;
int WndInit::nRepeatTime = 8000;



UINT __cdecl CTransThread::TransThread(LPVOID pParam) {
	TransParam* param = (TransParam*)pParam;
	param->status = 0;
	CString strRes;
	if (!CTrans::GetTransRes(param->szEn, strRes)) {
		AfxGetMainWnd()->GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(_T("internet error!"));
		param->status = -1;
		return 0;
	}
	AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)->SetWindowTextW(param->szEn);
	((CEdit*)(AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)))->SetSel(-1);
	AfxGetMainWnd()->GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(strRes);
	param->status = 1;
	return 0;
}
UINT __cdecl CTransThread::PronuThread(LPVOID pParam) {
	TransParam* param = (TransParam*)pParam;
	param->status = 0;
	CString strPronu[4];
	if (!CTrans::GetPronu(param->szEn, strPronu)) {
		param->status = -1;
		return 0;
	}
	for (int n = 0; n < 4; ++n) {
		static MCI_OPEN_PARMS mciOpen;
		mciOpen.lpstrDeviceType = _T("MPEGAudio");
		if (strPronu[n].IsEmpty()) {
			int n1 = -1;
			do {
				++n1;
				mciOpen.lpstrElementName = strPronu[n1];
			} while (strPronu[n1].IsEmpty() && 3 > n1);
		}
		else {
			mciOpen.lpstrElementName = strPronu[n];
		}
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
		mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_WAIT, (DWORD)&mciOpen);
		mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
		Sleep(100);
	}
	param->status = 1;
	return 0;
}