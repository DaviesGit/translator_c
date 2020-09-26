//////////////////////////////
//

#include "stdafx.h"
#include "translatordlg.h"
#include "resource.h"
#include "OtherCoreFile\translate.h"
#include "pronunciation.h"
#include <MMSystem.h>



BEGIN_MESSAGE_MAP(CTransDlg, CDialog)
	//{{AFX
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TRANSLATE, OnTranslate)
	ON_MESSAGE(MM_MCINOTIFY, OnMciNotify)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)

	//}}AFX
END_MESSAGE_MAP()


void CTransDlg::OnPaint() {
	CDialog::OnPaint();
	
}

BOOL CTransDlg::OnEraseBkgnd(CDC* pDC) {

	CBitmap bmBack;
	CDC m_dcBack;
	bmBack.LoadBitmap(IDB_BACKGROUND);
	m_dcBack.CreateCompatibleDC(pDC);

	m_dcBack.SelectObject(&bmBack);
	pDC->BitBlt(0,0,300,400,&m_dcBack,0,0,SRCCOPY);
	
	return true;
}

void CTransDlg::OnClose() {
	
	PostQuitMessage(0);
}

void CTransDlg::OnLButtonUp(UINT nFlags, CPoint point) {
	switch (GetClickType(point)) {
		case  CLOSEBTN: {
			PostQuitMessage(0);
		break;
		}

	}
}

void CTransDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	switch (GetClickType(point)) {
		case MOVEBAR: {
			DefWindowProc(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		break;
		}

	}
}


int CTransDlg::GetClickType(CPoint point) {
	if (point.x > 253 && point.y < 34) {
		return CLOSEBTN;
	}
	if (point.x < 253 && point.y < 34) {
		return MOVEBAR;
	}
	return UNKNOWNTYPE;
}



HBRUSH CTransDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
/*
	if(pWnd->GetDlgCtrlID() == 13545) {
		pDC->SetBkMode(TRANSPARENT);


		static int a = 0;
		++a;
		char *szCount = new char[10];
		itoa(a, szCount, 10);
		CString strCount = "Ctrl:";
		strCount += szCount;
		GetDlgItem(IDC_EDIT3)->SetWindowText(strCount);
		CString strTemp;
		GetDlgItem(IDC_EDIT4)->GetWindowText(strTemp);
		strCount = strTemp + strCount + "\r\n";
		GetDlgItem(IDC_EDIT4)->SetWindowText(strCount);

		static int aa = 0;
		++aa;
		if(1 == aa % 3) {
			CRect rcInputDlg;
			pWnd->GetWindowRect(rcInputDlg);
			ScreenToClient(rcInputDlg);
			InvalidateRect(rcInputDlg);
		}

		return (HBRUSH)::GetStockObject(NULL_BRUSH);
		return (HBRUSH)brBkgnd;


	}
	
*/


	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
} 


void CTransDlg::OnTranslate() {
	CString strInputEdit;
	GetDlgItem(1234)->GetWindowText(strInputEdit);
	if(0 == strInputEdit.GetLength()) {
		MessageBox("Enter the context! please!");
		return;
	}
	CString strDicResult;
	CString strTransResult;
	CTranslate::Translate(strInputEdit, strDicResult, strTransResult);
	GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText("dictionary:\r\n" + strDicResult + "\r\n" + "translate:\r\n" + strTransResult);
	CString strPathTemp;
	if (CTranslate::GetPronunciation(strInputEdit, strPathTemp)) {
		Pronunciation(strPathTemp);
	}

} 

static CString strLastClipboard = "";

void CALLBACK GetClipboardText(HWND hWnd, UINT nMsg, UINT Timerid, DWORD dwTime) {
	if (OpenClipboard(NULL)) {
		HANDLE hData = GetClipboardData(CF_TEXT);
		char* buffer = (char*)GlobalLock(hData);
		CString strClipboardText = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
		
		if (strLastClipboard == strClipboardText) {
			return;	
		} else {
			strLastClipboard = strClipboardText;
		}


		char *szTemp = strClipboardText.GetBuffer(0);
		int nTemp = strClipboardText.GetLength();
		for (int n = 0; n < nTemp; ++n) {
			if (0 > szTemp[n]) {
				return;
			}
		}

		if ("" != strClipboardText) {
			((CTransDlg*)AfxGetMainWnd())->ClipboardTrans(strClipboardText);
		}
	}
}


void CTransDlg::ClipboardTrans(const char* szEn) {
	AfxGetMainWnd()->GetDlgItem(1234)->SetWindowText(szEn);
	OnTranslate();

}


BOOL CTransDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	RECT rcTemp;
	this->GetWindowRect(&rcTemp);
	int nTemp = GetSystemMetrics(SM_CXSCREEN)- rcTemp.left - 400;
	rcTemp.left += nTemp;
	rcTemp.right += nTemp;
	this->MoveWindow(&rcTemp);
	return true;
}

extern MCI_OPEN_PARMS mciOpen;
extern bool whetherInPronunciation;
void CTransDlg::OnMciNotify() {
	if (true == whetherInPronunciation) {
		mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
		whetherInPronunciation = false;
	}
}

void CTransDlg::OnHotKey(WPARAM wParam, LPARAM lParam) {
	if (1001 == wParam) {
	this->SetForegroundWindow();
	this->GetDlgItem(1234)->SetFocus();
	this->GetDlgItem(1234)->SetWindowText("");
	}
}