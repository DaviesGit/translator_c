//
//

#include "stdafx.h"
#include "translate.h"
#include <MMSystem.h>

#define INTERNETERROR -1;
#define TRANSLATESUCCESSFUL 0;

/////////////////////////////////////////////////////////////////////////////
//CTranslate
//

CTranslate::CTranslate() {

}

CTranslate::~CTranslate() {

}

int CTranslate::Translate(const char* szEn, CString &dictResult, CString &transResult) {

	CString strEnHtml;
	strEnHtml = szEn;
	strEnHtml.TrimLeft();
	strEnHtml.TrimRight();
	strEnHtml.Replace(" ", "+");
	CString strUrl = "http://fanyi.baidu.com/v2transapi?from=en&to=zh&query=" + strEnHtml + "&transtype=translang&simple_means_flag=3";

	CString strHtml;

	CInternetSession internetSession;
	CHttpFile *httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	CString tempStr;
	while (httpFile->ReadString(tempStr)) {
		strHtml += tempStr;
	}
/*
	CFile file("C:\\Users\\Administrator\\Desktop\\test.html", CFile::modeRead);
	char* szTemp;
	szTemp = new char[file.GetLength() + 1];
	file.Read(szTemp, file.GetLength());
	szTemp[file.GetLength()] = 0;
	m_translateHtmlText = szTemp;
	delete szTemp; szTemp = NULL;
*/

	int nTempLocation0;
	int nTempLocation1;
	int nTempLocation2;
	nTempLocation0 = strHtml.Find("data\":[{\"dst", 0);
	if(-1 != nTempLocation0) {
		nTempLocation1 = strHtml.Find("\"", nTempLocation0+15);
		if(-1 != nTempLocation1) {
			CString strDictResult = strHtml.Mid(nTempLocation0 + 15, nTempLocation1 - nTempLocation0 -15);
			Decode(strDictResult, transResult);
		}
	}
	nTempLocation0 = strHtml.Find("part\":\"", 0);
	CString strDictResult;
	while (-1 != nTempLocation0) {
		nTempLocation0 += 7;
		if ('\"' == strHtml[nTempLocation0]) {
			strDictResult += "´Êµä: ";
			nTempLocation1 = nTempLocation0;
			nTempLocation0 += 11;
		} else {
			nTempLocation1 = strHtml.Find("\"", nTempLocation0);
			strDictResult += strHtml.Mid(nTempLocation0, nTempLocation1 - nTempLocation0) + " ";
			nTempLocation0 = nTempLocation1 + 11;
		}
		nTempLocation1 += 10;
		nTempLocation2 = nTempLocation1;
		do {
		nTempLocation2 = strHtml.Find("[", nTempLocation2 + 1);
		nTempLocation1 = strHtml.Find("]", nTempLocation1 + 1);
		} while (nTempLocation2 < nTempLocation1 && -1 != nTempLocation2);
		strDictResult += strHtml.Mid(nTempLocation0, nTempLocation1 - nTempLocation0);
		strDictResult += "\r\n";
		nTempLocation0 = strHtml.Find("part\":\"", nTempLocation0);
	}
	strDictResult.Remove('\"');
	strDictResult.Replace(",", "; ");
	Decode(strDictResult, dictResult);


	return TRANSLATESUCCESSFUL;
}

extern bool whetherInPronunciation;
extern MCI_OPEN_PARMS mciOpen;
bool CTranslate::GetPronunciation(const char* szEn, CString &strAudioPath) {
	
	if (true == whetherInPronunciation) {
		mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
		whetherInPronunciation = false;
		return false;
	}

	int nTemp = strlen(szEn);
	CString strFileName = szEn;
	for (int n = 0; n < nTemp; ++n) {
		if (97 > szEn[n]) {
			strFileName = "EnAudio";
			break;
		}
		if (122 < szEn[n]) {
			strFileName = "EnAudio";
			break;
		}
	}

	
	CFile file;
	char szExeDir[8192];
	int tempDirLen = GetModuleFileName(NULL, szExeDir, 8191);
	CString strDirTemp = szExeDir;
	strDirTemp = strDirTemp.Left(strDirTemp.ReverseFind('\\')) + "\\" + "EnAudio\\";
	if (-1 == GetFileAttributes(strDirTemp)) {
		if (!CreateDirectory(strDirTemp, NULL)) {
			AfxMessageBox("Can't Create Directory!");
			return false;
		}
	}
	
	if ("EnAudio" != strFileName) {
		if (-1 != GetFileAttributes(strDirTemp + strFileName + ".mp3")) {
			strAudioPath = strDirTemp + strFileName + ".mp3";
			return true;
		}
	}

	CString strUrl = "http://fanyi.baidu.com/gettts?lan=en&text=";
	strUrl += szEn;
	strUrl += "&source=web";
	CInternetSession internetSession;
	CHttpFile *httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);

	if (!file.Open(strDirTemp + strFileName + ".mp3", CFile::modeCreate | CFile::modeWrite)) {
		AfxMessageBox("Can't open the file for writing!");
		return false;
	}

	char tempData[8192];
	int tempDataLen;
	do {
	tempDataLen = httpFile->Read(tempData, 8192);
	file.Write(tempData, tempDataLen);
	} while (0 != tempDataLen);
	file.Close();

	strAudioPath = strDirTemp + strFileName + ".mp3";
	
	return true;
}


bool CTranslate::Decode(const char* szUnicode, CString &strGb2312) {
	CString strUnicode = szUnicode;
	int nUnicodeStrLength = strUnicode.GetLength();
	if (0 == nUnicodeStrLength) {
		return false;
	}
	int nPoint = 0;
	CString strTemp;
	strGb2312.Empty();
	WCHAR wcTemp;
	char szTemp[3];
	szTemp[2] = 0;
	do {
		strTemp = strUnicode.Mid(nPoint, 1);
		if ("\\" == strTemp) {
			++nPoint;
			strTemp = strUnicode.Mid(nPoint, 1);
			++nPoint;
			if ("u" == strTemp) {
				strTemp = strUnicode.Mid(nPoint, 4);
				wcTemp = strtol(strTemp, (char**)NULL, 16);
				WideCharToMultiByte(CP_ACP,0,&wcTemp,1,szTemp,3,NULL,NULL); 
				strGb2312 += szTemp;
				nPoint += 4;
			} else {
				strGb2312 += "\\" + strTemp;
			}
		} else {
			++nPoint;
			strGb2312 += strTemp;
		}
	} while (nUnicodeStrLength >= nPoint);

	return true;
}



