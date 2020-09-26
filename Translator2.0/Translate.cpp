//
//

#include "stdafx.h"
#include "translate.h"
#include <afxinet.h>


/////////////////////////////////////////////////////////////////////////////
//CTranslate
//

CTranslate::CTranslate() {

}

CTranslate::~CTranslate() {

}

///////////////////////////////////////////////////////////////////////////
//dictResult 结尾有换行符
//transResult 只有一行
bool CTranslate::Translate(const char* szEn, CString &dictResult, CString &transResult) {

	int nStrLen = strlen(szEn);
	bool whetherStorage;
	whetherStorage = true;
	for (int n = 0; n < nStrLen; ++n) {
		if (123 > szEn[n] && 96 < szEn[n]) {
			
		} else {
			whetherStorage = false;
		}
	}

	CFile file;
	if (true == whetherStorage) {
		CString strFileName;
		char szExeDir[8192];
		int nTempDirLen = GetModuleFileName(NULL, szExeDir, 8191);
		szExeDir[nTempDirLen] = 0;
		strFileName = szExeDir;
		strFileName = strFileName.Left(strFileName.ReverseFind('\\')) + "\\" + "EnText";
		if (-1 == GetFileAttributes(strFileName)) {
			if (!CreateDirectory(strFileName,NULL)) {
#ifdef _DEBUG
				AfxMessageBox("Can not create directory!");
#endif
			}
		}
		char chTemp[2] = {szEn[0], 0};
		strFileName = strFileName + "//" + chTemp + "_EnText.txt";
		file.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
		
		char tempBuf[8192];
		int nReadLen;
		CString strEnText;
		do {
			nReadLen = file.Read(tempBuf, 8191);
			tempBuf[nReadLen] = 0;
			strEnText += tempBuf;
		} while (nReadLen);
		CString strSub = "\r\n";
		strSub += szEn;
		int nLocation = -1;
		do {
			++nLocation;
			nLocation = strEnText.Find(strSub, nLocation);
			if (-1 == nLocation) {
			 break;
			}
		} while ("\r\n" != strEnText.Mid(nLocation + strlen(szEn) + 2, 2));
		if (-1 != nLocation) {
			int nLocation0;
			int nLocation1;
			nLocation0 = strEnText.Find("\\\\{{DictResult", nLocation) + 17;
			nLocation1 = strEnText.Find("\\\\}}", nLocation0) -2;
			dictResult = strEnText.Mid(nLocation0, nLocation1 - nLocation0) + "\r\n";
			if ("\r\n" == dictResult) {
				dictResult = "";
			}
			nLocation0 = strEnText.Find("\\\\{{TransResult", nLocation) + 18;
			nLocation1 = strEnText.Find("\\\\}}", nLocation0) -2;
			transResult = strEnText.Mid(nLocation0, nLocation1 - nLocation0);
			file.Close();
			return true;
		}

	}


	CString strEnHtml;
	strEnHtml = szEn;
//	strEnHtml.TrimLeft();
//	strEnHtml.TrimRight();
	strEnHtml.Replace(" ", "+");
	CString strUrl = "http://fanyi.baidu.com/v2transapi?from=en&to=zh&query=" + strEnHtml + "&transtype=translang&simple_means_flag=3";
	CString strHtml;
	CInternetSession internetSession;
	CHttpFile *httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	char szTemp[8192];
	int nCount = 0;
	do {
		nCount = httpFile->Read(szTemp, 8191);
		szTemp[nCount] = 0;
		strHtml += szTemp;
	} while (nCount);

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
			strDictResult += "词典: ";
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
//////////////////////////////////////////////////
//storage
	if (true == whetherStorage) {
		CString strStorageText;
		strStorageText = "\r\n";
		strStorageText += szEn;
		strStorageText += "\r\n\\\\{{DictResult:\r\n";
		strStorageText += dictResult + "\\\\}}\r\n";
		strStorageText += "\\\\{{TransResult:\r\n";
		strStorageText += transResult + "\r\n\\\\}}\r\n";
		int nTemp = strStorageText.GetLength();
		file.SeekToEnd();
		file.Write(strStorageText, nTemp);
		file.Close();
	}

	return true;
}

bool CTranslate::GetPronunciation(const char* szEn, CString &strAudioPath) {

	int nStrLen = strlen(szEn);
	if (1000 < nStrLen) {
		return false;
	}
	bool whetherEnWord = true;
	CString strFileName;
	strFileName = szEn;
	for (int n = 0; n < nStrLen; ++n) {
		if (123 > szEn[n] && 96 < szEn[n]) {

		} else {
			strFileName = "_EnAudio";
			whetherEnWord = false;
		}
	}

	char szExeDir[8192];
	int tempDirLen = GetModuleFileName(NULL, szExeDir, 8191);
	szExeDir[tempDirLen] = 0;
	CString strDirTemp = szExeDir;
	strDirTemp = strDirTemp.Left(strDirTemp.ReverseFind('\\')) + "\\" + "EnAudio\\";
	if (-1 == GetFileAttributes(strDirTemp)) {
		if (!CreateDirectory(strDirTemp, NULL)) {
#ifdef _DEBUG
			AfxMessageBox("Can't Create Directory!");
#endif
			return false;
		}
	}
	
	if (true == whetherEnWord) {
		if (-1 != GetFileAttributes(strDirTemp + strFileName + ".mp3")) {
			strAudioPath = strDirTemp + strFileName + ".mp3";
			return true;
		}
	}

	CFile file;	
	CString strUrl = "http://fanyi.baidu.com/gettts?lan=en&text=";
	strUrl += szEn;
	strUrl += "&source=web";
	CInternetSession internetSession;
	CHttpFile *httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);

	if (false == whetherEnWord) {
		int nCount = 0;
		char szCount[10];
		int nTemp;
		do {
			++nCount;
			itoa(nCount, szCount, 10);
			strAudioPath = strDirTemp + strFileName + szCount + ".mp3";
			nTemp = file.Open(strAudioPath, CFile::modeCreate | CFile::modeWrite);
		} while (!nTemp);
	} else {
		strAudioPath = strDirTemp + strFileName + ".mp3";
		if (!file.Open(strAudioPath, CFile::modeCreate | CFile::modeWrite)) {
#ifdef _DEBUG
			AfxMessageBox("Can not open Audio file for writing!");
#endif
		}
	}

	char tempData[8192];
	int tempDataLen;
	do {
	tempDataLen = httpFile->Read(tempData, 8192);
	file.Write(tempData, tempDataLen);
	} while (tempDataLen);
	if (0 == file.GetLength()) {
		return false;
	}
	file.Close();
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
				wcTemp = (int)strtol(strTemp, (char**)NULL, 16);
				WideCharToMultiByte(CP_ACP, 0, &wcTemp, 1, szTemp, 3, NULL, NULL); 
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

bool CTranslate::Code(const char* szGb2312, CString &strUnicode) {
	CString strGb2312 = szGb2312;
	const int nLength = strGb2312.GetLength();
	if (0 == nLength) {
		return false;
	}
	CString strTemp;
	strUnicode.Empty();
	int nPoint = 0;
	do {
		if (0 > strGb2312[nPoint]) {
			CString strCharTemp = strGb2312.Mid(nPoint, 2);
			char szWideCharTemp[2];
			MultiByteToWideChar(CP_ACP, 0, strCharTemp, 2, (unsigned short*)szWideCharTemp, 2);
			char szUTF8Char[8];
			int nLength = WideCharToMultiByte(CP_UTF8, 0, (unsigned short*)szWideCharTemp, 1, szUTF8Char, 8, NULL, NULL);
			for(int n = 0; n < nLength; ++n) {
				strTemp.Format("%X", szUTF8Char[n]);
				strUnicode += "%" + strTemp.Mid(6);
			}
			nPoint += 2;
		} else {
			strUnicode += szGb2312[nPoint];
			++nPoint;
		}
	} while (nLength > nPoint);
	return true;
}

bool CTranslate::ChToEn(const char* szCh, CString &strTransResult, CString &strDictResult) {
	CString strCh;
	CString strUnicodeCh;
	strCh = szCh;
	strCh.Replace(" ", "+");
	Code(strCh, strUnicodeCh);
	CString strUrl = "http://fanyi.baidu.com/v2transapi?from=zh&to=en&query=" + strUnicodeCh + "&transtype=translang&simple_means_flag=3";
	CInternetSession internetSession;
	CHttpFile *httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	CString strHtml;
	char szTemp[8192];
	int nCount = 0;
	do {
		nCount = httpFile->Read(szTemp, 8191);
		szTemp[nCount] = 0;
		strHtml += szTemp;
	} while (nCount);

	int nTempLocation0;
	int nTempLocation1;
	int nTempLocation2;
	nTempLocation0 = strHtml.Find("data\":[{\"dst", 0);
	if(-1 != nTempLocation0) {
		nTempLocation1 = strHtml.Find("\"", nTempLocation0+15);
		if(-1 != nTempLocation1) {
			strTransResult = strHtml.Mid(nTempLocation0 + 15, nTempLocation1 - nTempLocation0 -15);
		}
	}
	nTempLocation0 = strHtml.Find("word_means\":[\"", 0);
	if (-1 != nTempLocation0) {
		nTempLocation0 += 13;
		nTempLocation1 = nTempLocation0;
		nTempLocation2 = nTempLocation0;
		do {
			nTempLocation2 = strHtml.Find("[", nTempLocation2 + 1);
			nTempLocation1 = strHtml.Find("]", nTempLocation1 + 1);
		} while (nTempLocation2 < nTempLocation1 && -1 != nTempLocation2);
		strDictResult += strHtml.Mid(nTempLocation0, nTempLocation1 - nTempLocation0);
		strDictResult.Remove('\"');
		strDictResult.Replace(",", "; ");
	}
	return true;
}
