///////////////////////////////////////
//

#include "stdafx.h"
#include "pronunciation.h"

#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

MCI_OPEN_PARMS mciOpen;
bool whetherInPronunciation = false;

void Pronunciation(const char* szFileName) {
	if (true == whetherInPronunciation) {
		mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
		whetherInPronunciation = false;
		return;
	}
	
	mciOpen.lpstrDeviceType = "MPEGAudio";
	mciOpen.lpstrElementName = szFileName;
	mciOpen.dwCallback = (DWORD)AfxGetMainWnd()->m_hWnd;
	MCIERROR mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
	if(mciError) {
		char buf[256];
		mciGetErrorString(mciError, buf, 256);
		AfxMessageBox(buf);
		return;
	}
	mciError = mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciOpen);
	if(mciError) {
		char buf[256];
		mciGetErrorString(mciError, buf, 256);
		AfxMessageBox(buf);
		return;
	} else {
		whetherInPronunciation = true;
	}
	
}



