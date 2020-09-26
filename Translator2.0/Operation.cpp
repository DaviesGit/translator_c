///////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "resource.h"
#include "translate.h"
#include <math.h>
#include <mmsystem.h>

#pragma comment(lib, "WINMM.LIB")


//////////////////////////////////////////////////////////////

CTransWnd::WndState CTransWnd::nWndState = UNFOLD;
MCI_OPEN_PARMS CTransWnd::mciOpen;
/*
CWinThread* CTransWnd::thrFoldWnd = NULL;
CWinThread* CTransWnd::thrUnfoldWnd = NULL;
CWinThread* CTransWnd::thrAnimate = NULL;
CWinThread* CTransWnd::thrTrans = NULL;
CWinThread* CTransWnd::thrPronun = NULL;
*/
bool CTransWnd::whetherExitFoldThread = true;
bool CTransWnd::whetherExitUnfoldThread = true;
bool CTransWnd::whetherStopReadyFold = false;
bool CTransWnd::whetherExitAnimateThread = true;
bool CTransWnd::whetherOnTrans = false;
bool CTransWnd::whetherOnPronun = false;

char* CTransWnd::animate[16] = {"                                    \r\n                                    \r\n                                    \r\n          rU7   ,jJ:   7uL          \r\n        :DDDDDrFDDDDM:DDDDDv        \r\n        ZDDDDDDDDDDDDDDDDDDD        \r\n         MDDDD iDDDDJ MDDDD.        \r\n           :.    ,.     ,.          \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n          :uJ:   vU7   iYL.         \r\n         DDDDDZ:DDDDDr8DDDD1        \r\n        ,DDDDDDDDDDDDDDDGMDD        \r\n         JDDDD7 DDDDD uDDDDi        \r\n           ,.    .,     ..          \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n           LU7   iuj,  .;Li         \r\n         LDDDDD:DDDDDP:DDDDD,       \r\n         DDDDDDDDDDDDD8Dq00Dj       \r\n         .DDDDD UDDDDi 0DDDX        \r\n           .,     ..     .          \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n           iJJ.  .vU;   .i;:        \r\n          DDDDDj7DDDDD :ON0ZN.      \r\n         7DDDDDDDDDDDDL2P5UFDr      \r\n          0DDDD,.DDDDZ .XqZNJ       \r\n            ..    .,     ..         \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n           ,Y2i   ;UL.    :i:       \r\n          NDDDDD:DDDDDF :kk1kS,     \r\n          DDDDDDDDDDDDD LqYYY07     \r\n          rDDDDS NDDDDi .jkFSY      \r\n            .,     ,.      .        \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n            r2v.  ,JJi     .::      \r\n          :DDDDDr1DDDDD   LYLYji    \r\n          PDDDDDDDDDDDDi :U7rrLU    \r\n           MDDDD :DDDDP   iLYYv.    \r\n             :.    ..        .      \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n            :uJi   7Uv       ...    \r\n           DDDDD8:DDDDDr   ,7rrrr   \r\n          .DDDDDDDDDDDDD   rri:i7:  \r\n           YDDDDL DDDDD.   .irri:   \r\n             ,.    .,.              \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n             LU7   :ju:        ..   \r\n           7DDDDD:MDDDDM     :::::, \r\n           DDDDDDDDDDDDD.    i::,:: \r\n           .DDDDD JDDDDL     .::::. \r\n             .,     .,              \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n             iJJ.  .7Ur          .  \r\n. ...       DDDDDU7DDDDD:      .... \r\n. ....     7DDDDDDDDDDDDk      .... \r\n. ...       PDDDD:.DDDDO        ... \r\n              ..    .,              \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n   ...       ,Y5i   iUY,            \r\n  i::::     PDDDDD:DDDDDP           \r\n  ::,::,    DDDDDDDDDDDDD           \r\n  ::::,     iDDDDk kDDDDi           \r\n              .,     :.             \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n     ,..      rUv.  .Yji            \r\n   :7irri   :DDDDDrUDDDDD           \r\n   vii:i7   SDDDDDDDDDMDDr          \r\n   .r;ri,    ODDDD :DDDDP           \r\n               ,.    ..             \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n      .:,     :Uji   7UL            \r\n    ,YjLJJ.  DDDDDM:DDDDD7          \r\n    rUr77Ur  DDDDDDDDDDDDD          \r\n     7YYYr   LDDDDY DDDDD.          \r\n       .       ..     ,.            \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n       ,ii.    LU7   :ju:           \r\n      1F11Xr rDDDDD:MDDDDD          \r\n     :0ULJUX DDDDDDDDDDDDD.         \r\n      r2S15, .DDDDD YDDDDY          \r\n        ..     .,     ..            \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n        ,ri.   ijY,  .vUr           \r\n       28PNZY DDDDD1rDDDDD:         \r\n      .DF1UkPrDDDDDDDDDDDDk         \r\n       rNqEP: SDDDD:.DDDDM          \r\n         ..     ..     ,            \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n         :77.  .L2;   i1Y,          \r\n        MDDDDrJDDDDD:DDDDDq         \r\n       iDOP08MDDDDDDDDDDDDD         \r\n        UMDDM.,DDDDP SDDDD;         \r\n          ..    .:     ,.           \r\n                                    \r\n                                    \r\n",
"                                    \r\n                                    \r\n                                    \r\n         .rYi   ;UL.  .Jui          \r\n        7DDDDDrDDDDD7uDDDDD         \r\n        DDG88DDDDDDDDDDDDDD7        \r\n        .MDDDS XDDDD.,DDDDq         \r\n           ,     :.    ..           \r\n                                    \r\n                                    \r\n"};





//////////////////////////////////////////////////////////////////////////////////////////
void CTransWnd::FoldWnd() {
	if (true == whetherStopReadyFold) {
		whetherStopReadyFold = false;
		return;
	}
	switch (nWndState) {
		case UNFOLD: {
			whetherExitUnfoldThread = true;
			whetherExitFoldThread = false;
			CWinThread *thrFoldWnd = AfxBeginThread(FoldWndThread, NULL);
#ifndef _DEBUG
			CloseHandle(thrFoldWnd);
#endif
		break;
		}
		case READYFOLD: {
			whetherExitUnfoldThread = true;
			whetherExitFoldThread = false;
			CWinThread *thrFoldWnd = AfxBeginThread(FoldWndThread, NULL);
#ifndef _DEBUG
			CloseHandle(thrFoldWnd);
#endif
		break;
		}
		case INUNFOLD: {
			whetherExitUnfoldThread = true;
			whetherExitFoldThread = false;
			CWinThread *thrFoldWnd = AfxBeginThread(FoldWndThread, NULL);
#ifndef _DEBUG
			CloseHandle(thrFoldWnd);
#endif
		break;
		}
		default: {

		}

	}
}
//缓动y = -(x-1)^2 + 1;
UINT CTransWnd::FoldWndThread(LPVOID lParam) {
	nWndState = INFOLD;
	CWnd *mainWnd = AfxGetMainWnd();
	CWnd *inputEdit = mainWnd->GetDlgItem(IDC_EDIT_INPUT);
	RECT rcTemp0;
	RECT rcTemp1;
	RECT rcTemp2;
	RECT rcTemp5;
	int nFoldX, nFoldY;

	mainWnd->GetWindowRect(&rcTemp0);
	mainWnd->GetClientRect(&rcTemp1);
	mainWnd->ClientToScreen(&rcTemp1);
	rcTemp2 = rcTemp0;
	nFoldX = (rcTemp1.right - rcTemp1.left) - 200;
	nFoldY = (rcTemp1.bottom - rcTemp1.top) - 32;
	rcTemp5.left = 0; rcTemp5.top = 0; rcTemp5.bottom = 32;

	for (int n = 0; n < 20; ++n) {
		if (true == whetherExitFoldThread) {
			nWndState = FOLD;
			return 0;
		}
		rcTemp2.right = (int)(rcTemp0.right - nFoldX * (-pow((n+1)*0.05-1,2) + 1));
		rcTemp5.right = (int)(rcTemp1.right - rcTemp2.left - nFoldX * (-pow((n+1)*0.05-1,2) + 1));
		rcTemp2.bottom = (int)(rcTemp0.bottom - nFoldY * (-pow((n+1)*0.05-1,2) + 1));
		mainWnd->MoveWindow(&rcTemp2);
		inputEdit->MoveWindow(&rcTemp5);
		Sleep(20);
	}


	nWndState = FOLD;
//	CloseHandle(thrFoldWnd);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void CTransWnd::UnfoldWnd() {
	switch (nWndState) {
		case FOLD: {
			whetherExitFoldThread = true;
			whetherExitUnfoldThread = false;
			CWinThread *thrUnfoldWnd = AfxBeginThread(UnfoldWndThread, NULL);
#ifndef _DEBUG
			CloseHandle(thrUnfoldWnd);
#endif
		break;
		}
		case INFOLD: {
			whetherExitFoldThread = true;
			whetherExitUnfoldThread = false;
			CWinThread *thrUnfoldWnd = AfxBeginThread(UnfoldWndThread, NULL);
#ifndef _DEBUG
			CloseHandle(thrUnfoldWnd);
#endif
		break;
		}
		case READYFOLD: {
			whetherStopReadyFold = true;
			nWndState = UNFOLD;
		break;
		}
		default:{

		}
	}
}

UINT CTransWnd::UnfoldWndThread(LPVOID lParam) {
	nWndState = INUNFOLD;
	CWnd *mainWnd = AfxGetMainWnd();
	CWnd *inputEdit = mainWnd->GetDlgItem(IDC_EDIT_INPUT);
	RECT rcTemp0;
	RECT rcTemp1;
	RECT rcTemp2;
	RECT rcTemp5;
	int nUnfoldX, nUnfoldY;

	mainWnd->GetWindowRect(&rcTemp0);
	mainWnd->GetClientRect(&rcTemp1);
	mainWnd->ClientToScreen(&rcTemp1);
	rcTemp2 = rcTemp0;
	nUnfoldX = 300 - (rcTemp1.right - rcTemp1.left);
	nUnfoldY = 200 - (rcTemp1.bottom - rcTemp1.top);
	rcTemp5.left = 0; rcTemp5.top = 0; rcTemp5.bottom = 32;

	for (int n = 0; n < 20; ++n) {
		if (true == whetherExitUnfoldThread) {
			nWndState = UNFOLD;
			return 0;
		}
		rcTemp2.right = (int)(rcTemp0.right + nUnfoldX * (-pow((n+1)*0.05-1,2) + 1));
		rcTemp5.right = (int)(rcTemp1.right - rcTemp2.left + nUnfoldX * (-pow((n+1)*0.05-1,2) + 1));
		rcTemp2.bottom = (int)(rcTemp0.bottom + nUnfoldY * (-pow((n+1)*0.05-1,2) + 1));
		mainWnd->MoveWindow(&rcTemp2);
		inputEdit->MoveWindow(&rcTemp5);
		Sleep(20);
	}

	nWndState = UNFOLD;
//	CloseHandle(thrUnfoldWnd);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////





void CTransWnd::SetInTxt(const char* szText) {
	AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(szText);
}

void CTransWnd::SetOutTxt(const char* szText) {
	AfxGetMainWnd()->GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText(szText);
}

void CTransWnd::TransWndInit() {
	SetInTxt("Enter the Content there!");
	SetOutTxt("↑↑↑Enter the contents above↑↑↑\r\n\r\nNote: You can use CTRL+SHIFT+I to enter text quickly!\r\n\r\nMade by Davies");
}

CTransWnd::WndState CTransWnd::GetDlgState() {
	return nWndState;
}

void CTransWnd::SetDlgState(WndState state) {
	nWndState = state;
}

void CTransWnd::BeginTransAnimate() {
	if (true == whetherExitAnimateThread) {
		whetherExitAnimateThread = false;
		CWinThread *thrAnimate = AfxBeginThread(TransAnimateThread, NULL);
#ifndef _DEBUG
		CloseHandle(thrAnimate);
#endif
	}
}

void CTransWnd::StopTransAnimate() {
	whetherExitAnimateThread = true;
}

UINT CTransWnd::TransAnimateThread(LPVOID pParam) {
	int n = 0;
	while (!whetherExitAnimateThread) {
		SetOutTxt(animate[n]);
		if (14 < n) {
			n = 0;
		} else {
			++n;
		}
		Sleep(50);
	}
//	CloseHandle(thrAnimate);
	return 0;
}

void CTransWnd::Trans() {
	if (false == whetherOnTrans) {
		CWinThread *thrTrans = AfxBeginThread(TransThread, NULL);
#ifndef _DEBUG
		CloseHandle(thrTrans);
#endif
	}
}

UINT CTransWnd::TransThread(LPVOID pParam) {
	whetherOnTrans = true;
	bool whetherChText = false;
	BeginTransAnimate();
	CString strText;
	AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strText);
	int nLength = strText.GetLength();
	for (int n = 0; n < nLength; ++n) {
		if (0 > strText[n]) {
			whetherChText = true;
			break;
		}
	}
	if (whetherChText) {
		CString strDictResult;
		CString strTransResult;
		if (!CTranslate::ChToEn(strText, strTransResult, strDictResult)) {
			StopTransAnimate();
			SetOutTxt("Sorry!\r\n  translation failed!");
			goto end;
		}
		StopTransAnimate();		
		CString strResult;
		if ("" != strDictResult) {
			strResult = "  @Dictionary@:\r\n" + strDictResult + "\r\n\r\n";
		}
		if ("" != strTransResult) {
			strResult += "  @Translation@:\r\n" + strTransResult;
		}
		if ("" == strResult) {
			SetOutTxt("啊噢! 翻译好像出了一点问题喔!");
		} else {
			SetOutTxt(strResult);
		}
	} else {
		CString strTransResult;
		CString strDictResult;
		if (!CTranslate::Translate(strText, strDictResult, strTransResult)) {
			StopTransAnimate();
			SetOutTxt("Sorry!\r\n  translation failed!");
			goto end;
		}
		StopTransAnimate();
		CString strResult;
		if ("" != strDictResult && "\r\n" != strDictResult) {
			strResult = "  @Dictionary@:\r\n" + strDictResult + "\r\n";
		}
		if ("" != strTransResult) {
			strResult += "  @Translation@:\r\n" + strTransResult;
		}
		if ("" == strResult) {
			SetOutTxt("啊噢! 翻译好像出了一点问题喔!");
		} else {
			SetOutTxt(strResult);
		}
	}
end:
	whetherOnTrans = false;
	UnfoldWnd();
	return 0;
}

void CTransWnd::BeginPronun() {
	StopPronun();
	CWinThread *thrPronun = AfxBeginThread(PronunThread, NULL);
#ifndef _DEBUG
	CloseHandle(thrPronun);
#endif
}

UINT CTransWnd::PronunThread(LPVOID pParam) {
	Sleep(200);
	whetherOnPronun = true;
	CString szEn;
	CString strFileName;
	{
		AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(szEn);
		if (!CTranslate::GetPronunciation(szEn, strFileName)) {
			goto end;
		}
	}
	{	
		mciOpen.lpstrDeviceType = "MPEGAudio";
		mciOpen.lpstrElementName = strFileName;
		MCIERROR mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
		if (mciError) {
			char buf[1024];
			mciGetErrorString(mciError, buf, 1024);
//			AfxMessageBox(buf);
			goto end;
		}
		mciError = mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciOpen);
		if (mciError) {
			char buf[1024];
			mciGetErrorString(mciError, buf, 1024);
//			AfxMessageBox(buf);
			goto end;
		}
		MCI_STATUS_PARMS statusParms;
		statusParms.dwItem = MCI_STATUS_MODE;

		do {
			Sleep(100);
			if (!mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&statusParms)) {
				if (525 == statusParms.dwReturn) {
					goto end;
				}
			} else {
				goto end;
			}
		} while (whetherOnPronun);

	}

end:
	mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
	whetherOnPronun = false;
//	CloseHandle(thrPronun);
	return 0;
}

void CTransWnd::StopPronun() {
	whetherOnPronun = false;
}

////////////////////////////////////////////////////////////////////////////////
void RunLater(void (*Function)(), int Time) {
	StuParam *stuParam = new StuParam;
	stuParam->Function = Function;
	stuParam->Time = Time;
	CWinThread *thrRunLater = AfxBeginThread(RunLater, stuParam);
#ifndef _DEBUG
	CloseHandle(thrRunLater);
#endif
}

UINT RunLater(LPVOID pParam) {
	StuParam *stuParam = (StuParam*)pParam;
	void (*Function)() = stuParam->Function;
	Sleep(stuParam->Time);
	Function();
	return 0;
}

