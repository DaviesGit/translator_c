/////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "transdlg.h"
#include "resource.h"
#include "translate.h"


/////////////////////////////////////////////////////////



BEGIN_MESSAGE_MAP(CTransDlg, CDialog) 

	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_TRANS, OnTrans)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_HOTKEY, OnHotkey)
	ON_WM_TIMER()
	

END_MESSAGE_MAP()



void CTransDlg::OnClose() {
	CDialog::OnClose();
	PostQuitMessage(0);
}


void CTransDlg::OnTrans() {
	static int nLastTransTime = 0;
	int nTransTime;
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	nTransTime = systemTime.wHour * 60 * 60 * 1000 + systemTime.wMinute * 60 * 1000 + systemTime.wSecond * 1000 + systemTime.wMilliseconds;
	if (1000 > (nTransTime - nLastTransTime)) {
		return;
	}
	CString strEn;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strEn);
	if ("" == strEn) {
		CTransWnd::SetOutTxt("你好像忘记输入内容了喔!");
		return;	
	}

	bool whetherChText = false;
	CString strText;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strText);
	int nLength = strText.GetLength();
	for (int n = 0; n < nLength; ++n) {
		if (0 > strText[n]) {
			whetherChText = true;
			break;
		}
	}

	nLastTransTime = nTransTime;
	CTransWnd::UnfoldWnd();
	CTransWnd::Trans();
	if (!whetherChText) {
		CTransWnd::BeginPronun();
	}
}

HBRUSH CTransDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {

	if (CTLCOLOR_EDIT == nCtlColor) {
		switch (pWnd->GetDlgCtrlID()) {
			case IDC_EDIT_INPUT: {
				pDC->SetBkColor(RGB(230, 230, 230));
				pDC->SetTextColor(RGB(60, 60, 60));
				HBRUSH brTemp = CreateSolidBrush(RGB(210, 210, 210));
				return brTemp;
			break;
			}
			case IDC_EDIT_OUTPUT: {

			break;
			}
			default: {
				
			}
		}
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTransDlg::OnHotkey(WPARAM wParam, LPARAM lParam) {
	if (1001 == wParam) {
		this->SetForegroundWindow();
		CWnd *inputEdit = this->GetDlgItem(IDC_EDIT_INPUT);
		inputEdit->SetFocus();
		inputEdit->SetWindowText("");
		CTransWnd::UnfoldWnd();
		CTransWnd::StopPronun();
	}
}

void CTransDlg::OnTimer(UINT nIDEvent) {
	SetDlgState();
	CString strClipboardText;
	if (!GetClipboard(strClipboardText)) {
		return;
	}
	if (!WhetherEnTxt(strClipboardText)) {
		return;
	}
	CTransWnd::SetInTxt(strClipboardText);
	OnTrans();
}


void CTransDlg::SetDlgState() {
	RECT rcTemp;
	GetWindowRect(&rcTemp);
	CPoint pointTemp;
	GetCursorPos(&pointTemp);
	if ((rcTemp.left < pointTemp.x) && (rcTemp.right >pointTemp.x) && (rcTemp.top < pointTemp.y) && (rcTemp.bottom > pointTemp.y)) {
		if ((CTransWnd::FOLD == CTransWnd::GetDlgState()) || (CTransWnd::INFOLD == CTransWnd::GetDlgState()) || (CTransWnd::READYFOLD == CTransWnd::GetDlgState())) {
				CTransWnd::UnfoldWnd();
		}
	} else {
		if (CTransWnd::UNFOLD == CTransWnd::GetDlgState()) {
			if (CTransWnd::READYFOLD != CTransWnd::GetDlgState()) {
				if (NULL == GetActiveWindow()) {
					RunLater(CTransWnd::FoldWnd, 3000);
					CTransWnd::SetDlgState(CTransWnd::READYFOLD);
				}
			}
		}
	}
}

bool CTransDlg::WhetherEnTxt(const char* text) {
	int nStrLen = strlen(text);
	for (int n = 0; n < nStrLen; ++n) {
		if (32 > text[n]) {
			return false;
		}
	}
	for (n = 0; n < nStrLen; ++n) {
		if (34 < text[n] && 39 > text[n]) {
			return false;
		}
		if (39 < text[n] && 44 > text[n]) {
			return false;
		}
		if (46 < text[n] && 63 > text[n]) {
			return false;
		}
		if (63 < text[n] && 65 > text[n]) {
			return false;
		}
		if (90 < text[n] && 97 > text[n]) {
			return false;
		}
		if (122 < text[n]) {
			return false;
		}
	}
	return true;
}

CString CTransDlg::strLastClipboard = "";
bool CTransDlg::GetClipboard(CString &strClipboard) {
	if (::OpenClipboard(NULL)) {
		HANDLE hData = GetClipboardData(CF_TEXT);
		char *szBuf = (char*)GlobalLock(hData);
		strClipboard = szBuf;
		GlobalUnlock(hData);
		CloseClipboard();
		if ("" == strClipboard) {
			return false;
		}
		if (strLastClipboard != strClipboard) {
			strLastClipboard = strClipboard;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}

	return true;
}

CTransDlg::CTransDlg() {
	GetClipboard(strLastClipboard);
}