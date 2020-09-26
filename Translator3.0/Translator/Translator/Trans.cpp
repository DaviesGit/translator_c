///////////////////////////////////////////////////////////////////////////////
//Using google translate engine
//

#include "stdafx.h"
#include "Trans.h"
#include <afxinet.h>
#include <vector>

__int64 CTrans::nKey1 = 0;
__int64 CTrans::nKey2 = 0;
CString CTrans::strDir;
bool CTrans::wheInGetKey = false;
DWORD CTrans::connectTimeout = 2000;

bool CTrans::GetTransRes(const WCHAR* szEn, CString &strRes){
	bool wheStore;
	CString strFileName;
	CFile file;
	wheStore = WheWord(szEn);
	if (strDir.IsEmpty()) {
		if (!InitDir()) {
			return false;
		}
	}
	if (wheStore) {
		WCHAR wcFirst;
		if (96 < szEn[0]) {
			wcFirst = szEn[0] - 32;
		}
		else{
			wcFirst = szEn[0];
		}
		strFileName = strDir + _T("\\EnText\\") + wcFirst + _T("_EnText.txt");
		if (!file.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite)) {
			return false;
		}
		CStringA strATemp;
		int nCount = 0;
		char buffer[8192];
		do {
			nCount = file.Read(buffer, 8191);
			buffer[nCount] = 0;
			strATemp += buffer;
		} while (nCount > 0);
		CString strTemp;
		Decoding(strATemp, strTemp);
		int nLoca1;
		int nLoca2;
		CString strSub;
		strSub = _T("@@@@:") + (CString)szEn + _T(":####\r\n");
		nLoca1 = strTemp.Find(strSub);
		if (-1 != nLoca1) {
			nLoca2 = strTemp.Find(_T("####END####\r\n\r\n"), nLoca1);
			if (-1 == nLoca2) {
#ifdef DEBUG
				AfxMessageBox(_T("English Text parse error!"));
#endif // DEBUG
			}
			int nLen = strSub.GetLength();
			strRes = strTemp.Mid(nLoca1 + nLen, nLoca2 - nLoca1 - nLen);
			file.Close();
			return true;
		}
	}
	CString  strBaidTrans, strBaidWord, strBaidPronu, strBaidExchange, strBaidCollins, strBaidEnEn;
	CString strGoogTrans, strGoogWord, strGoogPronu, strGoogDefine;
	ThreadParam BaidParam, GoogParam;
	BaidParam.status = 0;
	BaidParam.szEn = szEn;
	BaidParam.strTrans = &strBaidTrans;
	BaidParam.strWord = &strBaidWord;
	BaidParam.strPronu = &strBaidPronu;
	BaidParam.strExchange = &strBaidExchange;
	BaidParam.strCollins = &strBaidCollins;
	BaidParam.strEnEn = &strBaidEnEn;

	GoogParam.status = 0;
	GoogParam.szEn = szEn;
	GoogParam.strTrans = &strGoogTrans;
	GoogParam.strWord = &strGoogWord;
	GoogParam.strPronu = &strGoogPronu;
	GoogParam.strDefine = &strGoogDefine;

	AfxBeginThread(BdTransThread, &BaidParam);
	AfxBeginThread(GgTransThread, &GoogParam);

	while (0 == BaidParam.status || 0 == GoogParam.status) {
		Sleep(100);
	}
	strRes.Empty();
	if (1 == BaidParam.status && 1 == GoogParam.status) {
		if (!strBaidPronu.IsEmpty() && !strGoogPronu.IsEmpty()) {
			strRes += _T(" (-:  ") + strGoogPronu + _T("  ") + strBaidPronu + _T("\r\n");
		}
		else if (!strBaidPronu.IsEmpty() && strGoogPronu.IsEmpty()) {
			strRes += _T(" (-:  ") + strBaidPronu + _T("\r\n");
		}
		else if (strBaidPronu.IsEmpty() && !strGoogPronu.IsEmpty()) {
			strRes += _T(" (-:  ") + strGoogPronu + _T("\r\n");
		}
		if (!strBaidExchange.IsEmpty()) {
			strRes += _T("  ") + strBaidExchange + _T("\r\n");
		}
		if (!strGoogWord.IsEmpty()) {
			strRes += strGoogWord + _T("\r\n");
		}
		if (!strBaidCollins.IsEmpty()) {
			strRes += strBaidCollins + _T("\r\n");
		}
		if (!strBaidEnEn.IsEmpty()) {
			strRes += strBaidEnEn + _T("\r\n");
		}
		if (!strGoogDefine.IsEmpty()) {
			strRes += strGoogDefine + _T("\r\n");
		}
		if (!strBaidWord.IsEmpty()) {
			strRes += strBaidWord + _T("\r\n");
		}
		if (!strGoogTrans.IsEmpty()) {
			strRes += strGoogTrans + _T("\r\n");
		}
		if (!strBaidTrans.IsEmpty()) {
			strRes += strBaidTrans + _T("\r\n");
		}
		if (wheStore) {
			CString strStore(_T("@@@@:") + (CString)szEn + _T(":####\r\n") + strRes + _T("####END####\r\n\r\n"));
			CStringA strAStore;
			Encoding(strStore, strAStore);
			file.SeekToEnd();
			file.Write(strAStore, strAStore.GetLength());
			file.Close();
		}
	}
	else if (1 == BaidParam.status && -1 == GoogParam.status) {
		strRes += _T(" (-:  ") + strBaidPronu + _T("\r\n");
		if (!strBaidExchange.IsEmpty()) {
			strRes += _T("  ") + strBaidExchange + _T("\r\n");
		}
		if (!strBaidCollins.IsEmpty()) {
			strRes += strBaidCollins + _T("\r\n");
		}
		if (!strBaidEnEn.IsEmpty()) {
			strRes += strBaidEnEn + _T("\r\n");
		}
		if (!strBaidWord.IsEmpty()) {
			strRes += strBaidWord + _T("\r\n");
		}
		if (!strBaidTrans.IsEmpty()) {
			strRes += strBaidTrans + _T("\r\n");
		}
	}
	else if (-1 == BaidParam.status && 1 == GoogParam.status) {
		strRes += _T(" (-:  ") + strGoogPronu + _T("\r\n");
		if (!strGoogWord.IsEmpty()) {
			strRes += strGoogWord + _T("\r\n");
		}
		if (!strGoogDefine.IsEmpty()) {
			strRes += strGoogDefine + _T("\r\n");
		}
		if (!strGoogTrans.IsEmpty()) {
			strRes += strGoogTrans + _T("\r\n");
		}

	}
	else {
		return false;
	}
	return true;
}

bool CTrans::GetPronu(const WCHAR* szEn, CString* strFile) {
	bool wheWord;
	CFile file;
	wheWord = WheWord(szEn);
	if (strDir.IsEmpty()) {
		if (!InitDir()) {
			return false;
		}
	}
	ThreadParam BaidParam, GoogParam, CollinsParam, CambridgeParam;
	CString BaidFileName, GoogFileName, CollinsFileName, CambridgeFileName;
	if (wheWord) {
		BaidFileName = strDir + _T("\\EnAudio\\") + szEn[0] + _T("\\") + szEn + _T(".Baidu.mp3");
		GoogFileName = strDir + _T("\\EnAudio\\") + szEn[0] + _T("\\") + szEn + _T(".Google.mp3");
		CollinsFileName = strDir + _T("\\EnAudio\\") + szEn[0] + _T("\\") + szEn + _T(".Collins.mp3");
		CambridgeFileName = strDir + _T("\\EnAudio\\") + szEn[0] + _T("\\") + szEn + _T(".Cambridge.mp3");
	}
	else {
		BaidFileName = strDir + _T("\\EnAudio\\") + _T("EnAudio.Baidu.mp3");
		GoogFileName = strDir + _T("\\EnAudio\\") + _T("EnAudio.Google.mp3");
	}
	BaidParam.status = 0;
	BaidParam.szEn = szEn;
	BaidParam.szFileName = BaidFileName;
	GoogParam.status = 0;
	GoogParam.szEn = szEn;
	GoogParam.szFileName = GoogFileName;
	CollinsParam.status = 0;
	CollinsParam.szEn = szEn;
	CollinsParam.szFileName = CollinsFileName;
	CambridgeParam.status = 0;
	CambridgeParam.szEn = szEn;
	CambridgeParam.szFileName = CambridgeFileName;

	if (wheWord) {
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(BaidFileName)) {
			AfxBeginThread(BaidPronuThread, &BaidParam);
		}
		else {
			BaidParam.status = 1;
		}
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(GoogFileName)) {
			AfxBeginThread(GoogPronuThread, &GoogParam);
		}
		else {
			GoogParam.status = 1;
		}
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(CollinsFileName)) {
			AfxBeginThread(CollinsPronuThread, &CollinsParam);
		}
		else {
			CollinsParam.status = 1;
		}
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(CambridgeFileName)) {
			AfxBeginThread(CambridgePronuThread, &CambridgeParam);
		}
		else {
			CambridgeParam.status = 1;
		}
	}
	else {
		AfxBeginThread(BaidPronuThread, &BaidParam);
		AfxBeginThread(GoogPronuThread, &GoogParam);
		CollinsParam.status = -1;
		CambridgeParam.status = -1;
	}

	while (0 == BaidParam.status || 0 == GoogParam.status || 0 == CollinsParam.status || 0 == CambridgeParam.status) {
		Sleep(100);
	}
	if (1 == BaidParam.status) {
		strFile[0] = BaidFileName;
	}
	else {
		strFile[0].Empty();
	}
	if (1 == GoogParam.status) {
		strFile[1] = GoogFileName;
	}
	else {
		strFile[1].Empty();
	}
	if (1 == CollinsParam.status) {
		strFile[2] = CollinsFileName;
	}
	else {
		strFile[2].Empty();
	}
	if (1 == CambridgeParam.status) {
		strFile[3] = CambridgeFileName;
	}
	else {
		strFile[3].Empty();
	}
	if (-1 == BaidParam.status && -1 == GoogParam.status && -1 == CollinsParam.status && -1 == CambridgeParam.status) {
		return false;
	}
	return true;
}

bool CTrans::ChToEn(const WCHAR* szCh, CString &strEn)
{
	int n;
	int nLen;
	CString strKey;
	CStringA strACh;
	CString strTempF;
	Encoding(szCh, strACh);
	CString strCh(strACh);
	if (!GetKey(strCh, strKey)) {
		return false;
	}
	CString strUrl;
	nLen = strACh.GetLength();
	strCh.Empty();
	for (n = 0; n < nLen; ++n) {
		strTempF.Format(_T("%x"), strACh[n]);
		strCh += _T("%") + strTempF.Mid(6);
	}


	strUrl.Format(_T("https://translate.google.com/translate_a/single?client=t&sl=zh-CN&tl=en&hl=en&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=btn&ssel=0&tsel=0&kc=0&tk=%s&q=%s"), strKey, strCh);
	CInternetSession internetSession;
	internetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, connectTimeout);
	internetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, connectTimeout);
	CHttpFile* httpFile;
	try {
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (NULL == httpFile) {
		return false;
	}
	CStringA strTemp;
	int nCount = 0;
	char buffer[8192];
	do {
		try {
			nCount = httpFile->Read(buffer, 8191);
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			return false;
		}
		buffer[nCount] = 0;
		strTemp += buffer;
	} while (nCount > 0);
	httpFile->Close();
	CString strHtml;
	if (!Decoding(strTemp, strHtml)) {
#ifdef DEBUG
		AfxMessageBox(_T("Decoding Failed!"));
#endif // DEBUG
		return false;
	}
	int nWordLoca[] = { 2, 1 };
	int nTransLoca[] = { 2, 5 };
	int nTemp[10];
	int nLen1;
	int n1;
	CString strWord;
	CString strTrans;
	CString strTemp0;
	CString strTemp1;
	CString strTemp2;
	CString strTemp3;
	CString strTemp4;
	nTemp[0] = 4;
	nTemp[1] = 0;
	nTemp[2] = 1;
	nTemp[3] = 3;
	if (ParseArray(strHtml, nTemp, strTemp0)) {
		strEn += _T("(-:    ") + strTemp0.Mid(1, strTemp0.GetLength() - 2) + _T("\r\n\r\n");
	}
	if (ParseArray(strHtml, nWordLoca, strTemp0) && _T("") != strTemp0) {
		nLen = ParseArray(strTemp0, NULL);
		for (n = 0; n < nLen; ++n) {
			nTemp[0] = 2;
			nTemp[1] = n;
			ParseArray(strTemp0, nTemp, strTemp1);
			nTemp[1] = 0;
			ParseArray(strTemp1, nTemp, strTemp2);
			strWord += strTemp2.Mid(1, strTemp2.GetLength() - 2) + _T(":\r\n");
			nTemp[1] = 2;
			ParseArray(strTemp1, nTemp, strTemp2);
			nLen1 = ParseArray(strTemp2, NULL);
			for (n1 = 0; n1 < nLen1; ++n1) {
				nTemp[0] = 2;
				nTemp[1] = n1;
				ParseArray(strTemp2, nTemp, strTemp3);
				nTemp[1] = 3;
				ParseArray(strTemp3, nTemp, strTemp4);
				if (_T('0') != strTemp4[0]) {
					strWord += _T("--- ");
				}
				else {
					strWord += strTemp4.Mid(2, 3) + _T(' ');
				}
				nTemp[1] = 0;
				ParseArray(strTemp3, nTemp, strTemp4);
				strWord += strTemp4.Mid(1, strTemp4.GetLength() - 2) + _T(" ");
				nTemp[1] = 1;
				ParseArray(strTemp3, nTemp, strTemp4);
				strTemp4 = strTemp4.Mid(1, strTemp4.GetLength() - 2);
				strTemp4.Remove(_T('"'));
				strTemp4.Replace(_T(","), _T(", "));
				strWord += strTemp4 + _T("\r\n");
			}
			strWord += _T("\r\n");
		}
	}
	if (ParseArray(strHtml, nTransLoca, strTemp0)) {
		nLen = ParseArray(strTemp0, NULL);
		if (1 == nLen) {
			nTemp[0] = 3;
			nTemp[1] = 0;
			nTemp[2] = 2;
			ParseArray(strTemp0, nTemp, strTemp1);
			nLen = ParseArray(strTemp1, NULL);
			for (n = 0; n < nLen; ++n) {
				nTemp[0] = 3;
				nTemp[1] = n;
				nTemp[2] = 0;
				ParseArray(strTemp1, nTemp, strTemp2);
				strTrans += _T("@T: ") + strTemp2.Mid(1, strTemp2.GetLength() - 2) + _T("\r\n");
			}
		}
		else {
			strTrans += _T("@T: ");
			for (n = 0; n < nLen; ++n) {
				nTemp[0] = 5;
				nTemp[1] = n;
				nTemp[2] = 2;
				nTemp[3] = 0;
				nTemp[4] = 0;
				ParseArray(strTemp0, nTemp, strTemp1);
				strTrans += strTemp1.Mid(1, strTemp1.GetLength() - 2);
			}
			strTrans += _T("\r\n");
		}
	}
	strEn += strWord + _T("\r\n") + strTrans;
	return true;
}

bool CTrans::WheWord(const WCHAR* szEn) {
	int n, nLen;
	nLen = (int)wcslen(szEn);
	if (123 > szEn[0] && 96 < szEn[0]) {
		for (n = 1; n < nLen; ++n) {
			if (!((123 > szEn[n] && 96 < szEn[n]) || _T('-') == szEn[n])) {
				return false;
			}
		}
	}
	else if (91 > szEn[0] && 64 < szEn[0]) {
		if (1 < nLen) {
			if (91 > szEn[1] && 64 < szEn[1]) {
				for (n = 2; n < nLen; ++n) {
					if (!(91 > szEn[n] && 64 < szEn[n])) {
						return false;
					}
				}
			}
			else if (123 > szEn[1] && 96 < szEn[1]) {
				for (n = 2; n < nLen; ++n) {
					for (n = 2; n < nLen; ++n) {
						if (!(123 > szEn[n] && 96 < szEn[n])) {
							return false;
						}
					}
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
	return true;
}

bool CTrans::Encoding(const WCHAR* szText, CStringA &strText)
{
	int nCount = WideCharToMultiByte(CP_UTF8, 0, szText, -1, (LPSTR)NULL, 0, NULL, NULL);
	char* buffer = new char[nCount];
	int n = WideCharToMultiByte(CP_UTF8, 0, szText, -1, buffer, nCount, NULL, NULL);
	if (n < nCount) {
#ifdef DEBUG
		AfxMessageBox(_T("Encoding failed!"));
#endif 
		return false;
	}
	strText = buffer;
	delete[] buffer;
	return true;
}

bool CTrans::Decoding(const char* szText, CString &strText)
{
	int nCount = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)szText, -1, (LPWSTR)NULL, 0);
	WCHAR* wChar = new WCHAR[nCount];
	int n = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)szText, -1, wChar, nCount);
	if (n < nCount) {
#ifdef DEBUG
		AfxMessageBox(_T("Decoding failed!"));
#endif 
		return false;
	}
	strText = wChar;
	delete[] wChar;
	return true;
}

bool CTrans::BaidDecoding(const char* szText, CString &strText) {
	CStringA strAUnicode = szText;
	int nUnicodeStrALength = strAUnicode.GetLength();
	if (0 == nUnicodeStrALength) {
		return false;
	}
	int nPoint = 0;
	CStringA strATemp;
	strText.Empty();
	WCHAR wcTemp[2];
	wcTemp[1] = 0;
	do {
		strATemp = strAUnicode.Mid(nPoint, 1);
		if ("\\" == strATemp) {
			++nPoint;
			strATemp = strAUnicode.Mid(nPoint, 1);
			++nPoint;
			if ("u" == strATemp) {
				strATemp = strAUnicode.Mid(nPoint, 4);
				wcTemp[0] = (int)strtol(strATemp, (char**)NULL, 16);
				strText += wcTemp;
				nPoint += 4;
			}
			else {
				strText += _T("\\");
				strText += strATemp;
			}
		}
		else {
			++nPoint;
			strText += strATemp;
		}
	} while (nUnicodeStrALength > nPoint);
	return true;
}

bool CTrans::GetKey(const WCHAR* szEn, CString &strKey) {
	//initialize the nKey1 and nKey2
	if (wheInGetKey) {
		while (wheInGetKey) {
			Sleep(10);
		}
	}
	if (0 >= nKey1 || 0 >= nKey2) {
		wheInGetKey = true;
		CString strHtml;
		try {
			CInternetSession internetSession;
			internetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, connectTimeout);
			internetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, connectTimeout);
			CHttpFile *httpFile;

			httpFile = (CHttpFile*)internetSession.OpenURL(_T("http://translate.google.com"));
			if (NULL == httpFile) {
				wheInGetKey = false;
				return false;
			}
			char szTemp[8192];
			int nCount = 0;
			do {
				nCount = httpFile->Read(szTemp, 8191);
				szTemp[nCount] = 0;
				strHtml += szTemp;
			} while (nCount);
			httpFile->Close();
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			wheInGetKey = false;
			return false;
		}
		__int64 a;
		__int64 b;
		int nTemp1;
		int nTemp2;
		CStringA strTemp;
		//get key1
		nTemp2 = strHtml.Find(_T("+\\x27.\\x27+"), 0);
		if (-1 == nTemp2) {
			wheInGetKey = false;
			return false;
		}
		nTemp1 = strHtml.Find(_T("var a\\x3d"), 0);
		nTemp1 = strHtml.Find(_T("return "), nTemp1);
		strTemp = strHtml.Mid(nTemp1 + 7, nTemp2 - nTemp1 - 7);
		nKey1 = atoll(strTemp);
		//get key2
		nTemp1 = strHtml.Find(_T("var a\\x3d"), 0);
		if (-1 == nTemp1) {
			wheInGetKey = false;
			return false;
		}
		nTemp2 = strHtml.Find(_T(";"), nTemp1);
		strTemp = strHtml.Mid(nTemp1 + 9, nTemp2 - nTemp1 - 9);
		a = atoll(strTemp);

		nTemp1 = strHtml.Find(_T("var b\\x3d"), 0);
		if (-1 == nTemp1) {
			wheInGetKey = false;
			return false;
		}
		nTemp2 = strHtml.Find(_T(";"), nTemp1);
		strTemp = strHtml.Mid(nTemp1 + 9, nTemp2 - nTemp1 - 9);
		b = atoll(strTemp);
		nKey2 = a + b;
		if (100 >= nKey1 || 100 >= nKey2) {
#ifdef DEBUG
			AfxMessageBox(_T("Initialize failed!"));
#endif
			wheInGetKey = false;
			return false;
		}
		wheInGetKey = false;
	}
	int n;
	int nLen;

	__int64 a = nKey1;
	__int64 b = nKey1;

	CStringA strEn(szEn);
	nLen = strEn.GetLength();
	for (n = 0; n < nLen; ++n) {
		int nNum = strEn[n];
		if (0 > nNum) {
			nNum += 256;
		}
		a += nNum;
		a = GoogleHk(a, "+-a^+6");
	}
	a = GoogleHk(a, "+-3^+b+-f");
	a ^= nKey2;
	a %= 1000000;
	if (a < 10) {
#ifdef DEBUG
		AfxMessageBox(_T("Get Google Key failed"));
#endif 
		return false;
	}
	strKey.Format(_T("%d.%d"), (int)a, (int)((a ^ b) & 4294967295));
	return true;
}

__int64 CTrans::GoogleHk(__int64 a, const char* b) {
	int nLen = strlen(b);
	for (int c = 0; c < nLen - 2; c += 3) {
		__int64 d = b[c + 2];
		if ('a' <= d) {
			d = d - 87;
		}
		else {
			d = d - 48;
		}
		if ('+' == b[c + 1]) {
			d = a >> d;
		}
		else {
			d = a << d & 4294967295;
		}
		if ('+' == b[c]) {
			a = (a + d) & 4294967295;
		}
		else {
			a = a ^ d;
		}
	}
	return a;
}


bool CTrans::InitDir() {
	CString strModuleName;
	WCHAR szModuleName[8192];
	int nLen = GetModuleFileName(NULL, szModuleName, 8191);
	szModuleName[nLen] = 0;
	strModuleName = szModuleName;
	strDir = strModuleName.Left(strModuleName.ReverseFind('\\'));
	CString strTemp;
	strTemp = strDir + _T("\\EnText");
	if (-1 == GetFileAttributes(strTemp)) {
		if (!CreateDirectory(strTemp, NULL)) {
#ifdef DEBUG
			AfxMessageBox(_T("Can not Create dirctory!"));
#endif // DEBUG
			return false;
		}
	}
	strTemp = strDir + _T("\\EnAudio");
	if (-1 == GetFileAttributes(strTemp)) {
		if (!CreateDirectory(strTemp, NULL)) {
#ifdef DEBUG
			AfxMessageBox(_T("Can not Create dirctory!"));
#endif // DEBUG
			return false;
		}
	}
	for (int n = 0; n < 26; ++n) {
		WCHAR wChar = 65 + n;
		strTemp = strDir + _T("\\EnAudio\\") + wChar;
		if (-1 == GetFileAttributes(strTemp)) {
			if (!CreateDirectory(strTemp, NULL)) {
#ifdef DEBUG
				AfxMessageBox(_T("Can not Create dirctory!"));
#endif // DEBUG
				return false;
			}
		}
	}
	return true;
}

bool CTrans::BdTrans(const WCHAR* szEn, CString &strTrans, CString &strWord, CString &strPronu, CString &strExchange, CString &strCollins, CString &strEnEn) {
	CString strSubRes;
	int nLocation[10];
	CString strLocation[10];
	CString strTemp1, strTemp2, strTemp3, strTemp4, strTemp5, strTemp6;
	CString strTempEnd;
	int nCount, nCount1, nCount2;
	int n, n1, n2;

	CString strUrl;
	strUrl.Format(_T("http://fanyi.baidu.com/v2transapi?from=en&to=zh&query=%s&transtype=translang&simple_means_flag=3"), szEn);
	CInternetSession internetSession;
	internetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, connectTimeout);
	internetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, connectTimeout);
	CHttpFile* httpFile;
	try {
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (NULL == httpFile) {
		return false;
	}
	CStringA strATemp;
	nCount = 0;
	char buffer[8192];
	do {
		try {
			nCount = httpFile->Read(buffer, 8191);
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			return false;
		}
		buffer[nCount] = 0;
		strATemp += buffer;
	} while (nCount > 0);
	httpFile->Close();
	if (-1 != strATemp.Find("<html>")) {
		return false;
	}
	CString strHtml;
	if (!BaidDecoding(strATemp, strHtml)) {
#ifdef DEBUG
		AfxMessageBox(_T("Decoding Failed!"));
#endif // DEBUG
		return false;
	}

	strLocation[0] = _T("5");
	strLocation[1] = _T("strans_result");
	strLocation[2] = _T("sdata");
	strLocation[3] = _T("n0");
	strLocation[4] = _T("sdst");
	if (ParseJson(strHtml, strLocation, strSubRes)) {
		strSubRes = strSubRes.Mid(1, strSubRes.GetLength() - 2);
		if (!strSubRes.IsEmpty()) {
			strTrans = strSubRes;
		}
	}

	strLocation[0] = _T("2");
	strLocation[1] = _T("dict_result");
	if (ParseObject(strHtml, strLocation, strSubRes)) {
#if 0==0 //just for collapse it ***this is get En-En resutlt;
		strLocation[0] = _T("3");
		strLocation[1] = _T("edict");
		strLocation[2] = _T("item");
		if (ParseObject(strSubRes, strLocation, strTemp1)) {
			strEnEn.Empty();
			nCount = ParseArray(strTemp1, NULL);
			for (n = 0; n < nCount; ++n) {
				nLocation[0] = 2;
				nLocation[1] = n;
				ParseArray(strTemp1, nLocation, strTemp2);
				strLocation[0] = _T("2");
				strLocation[1] = _T("pos");
				if (ParseObject(strTemp2, strLocation, strTemp3)) {
					strEnEn += _T("@") + strTemp3.Mid(1, strTemp3.GetLength() - 2) + _T(" :\r\n");
				}
				strLocation[1] = _T("tr_group");
				if (ParseObject(strTemp2, strLocation, strTemp3)) {
					nCount1 = ParseArray(strTemp3, NULL);
					for (n1 = 0; n1 < nCount1; ++n1) {
						nLocation[0] = 2;
						nLocation[1] = n1;
						ParseArray(strTemp3, nLocation, strTemp4);
						strLocation[0] = _T("3");
						strLocation[1] = _T("str");
						strLocation[2] = _T("n0");
						if (ParseJson(strTemp4, strLocation, strTemp5)) {
							strEnEn += _T("->En: ") + strTemp5.Mid(1, strTemp5.GetLength() - 2);
						}
						strLocation[0] = _T("2");
						strLocation[1] = _T("similar_word");
						if (ParseObject(strTemp4, strLocation, strTemp5)) {
							nCount2 = ParseArray(strTemp5, NULL);
							if (0 < nCount2) {
								for (n2 = 0; n2 < nCount2; ++n2) {
									nLocation[0] = 2;
									nLocation[1] = n2;
									ParseArray(strTemp5, nLocation, strTemp6);
									if (!strTemp6.IsEmpty()) {
										if (1 == nCount2) {
											strEnEn += _T("  ->S: ") + strTemp6.Mid(1, strTemp6.GetLength() - 2);
										}
										else if (0 == n2) {
											strEnEn += _T("  ->S: ") + strTemp6.Mid(1, strTemp6.GetLength() - 2) + _T(", ");
										}
										else if (n2 + 1 == nCount2) {
											strEnEn += strTemp6.Mid(1, strTemp6.GetLength() - 2);
										}
										else {
											strEnEn += strTemp6.Mid(1, strTemp6.GetLength() - 2) + _T(", ");
										}
									}
								}
							}
						}
						strEnEn += _T("\r\n");
						strLocation[0] = _T("2");
						strLocation[1] = _T("example");
						if (ParseObject(strTemp4, strLocation, strTemp5)) {
							nCount2 = ParseArray(strTemp5, NULL);
							if (0 < nCount2) {
								for (n2 = 0; n2 < nCount2; ++n2) {
									nLocation[0] = 2;
									nLocation[1] = n2;
									ParseArray(strTemp5, nLocation, strTemp6);
									if (!strTemp6.IsEmpty()) {
										strEnEn += _T("  e.g. ") + strTemp6.Mid(1, strTemp6.GetLength() - 2) + _T("\r\n");
									}
								}
							}
						}
					}
				}
			}
		}
#endif // 0==0 //just for collapse it ***this is get En-En resutlt;
#if 0==0 //just for collapse it ***this is get Collins resutlt;
		strLocation[0] = _T("3");
		strLocation[1] = _T("collins");
		strLocation[2] = _T("entry");
		if (ParseObject(strSubRes, strLocation, strTemp1)) {
			nCount = ParseArray(strTemp1, NULL);
			strCollins.Empty();
			for (n = 0; n < nCount; ++n) {
				nLocation[0] = 2;
				nLocation[1] = n;
				ParseArray(strTemp1, nLocation, strTemp2);
				strLocation[0] = _T("2");
				strLocation[1] = _T("type");
				if (ParseObject(strTemp2, strLocation, strTemp3)) {
					if (!(_T("\"mean\"") == strTemp3)) {
						continue;
					}
					strLocation[0] = _T("3");
					strLocation[1] = _T("svalue");
					strLocation[2] = _T("n0");
					ParseJson(strTemp2, strLocation, strTemp3);
					strLocation[0] = _T("4");
					strLocation[1] = _T("sposp");
					strLocation[2] = _T("n0");
					strLocation[3] = _T("slabel");
					if (!ParseJson(strTemp3, strLocation, strTempEnd)) {
						continue;
					}
					strCollins += _T("->") + strTempEnd.Mid(1, strTempEnd.GetLength() - 2) + _T(": ");
					strLocation[0] = _T("2");
					strLocation[1] = _T("tran");
					ParseObject(strTemp3, strLocation, strTempEnd);
					strCollins += strTempEnd.Mid(1, strTempEnd.GetLength() - 2) + _T(" ");
					strLocation[0] = _T("2");
					strLocation[1] = _T("def");
					ParseObject(strTemp3, strLocation, strTempEnd);
					strCollins += strTempEnd.Mid(1, strTempEnd.GetLength() - 2) + _T("\r\n");
				}
			}
		}
		strLocation[2] = _T("menus");
		if (ParseObject(strSubRes, strLocation, strTemp4)){
			strCollins.Empty();
			nCount1 = ParseArray(strTemp4, NULL);
			for (n1 = 0; n1 < nCount1; ++n1) {
				strLocation[0] = _T("3");
				strLocation[1].Format(_T("n%d"), n1);
				strLocation[2] = _T("sentry");
				if (ParseJson(strTemp4, strLocation, strTemp1)) {
					nCount = ParseArray(strTemp1, NULL);
					for (n = 0; n < nCount; ++n) {
						nLocation[0] = 2;
						nLocation[1] = n;
						ParseArray(strTemp1, nLocation, strTemp2);
						strLocation[0] = _T("2");
						strLocation[1] = _T("type");
						if (ParseObject(strTemp2, strLocation, strTemp3)) {
							if (!(_T("\"mean\"") == strTemp3)) {
								continue;
							}
							strLocation[0] = _T("3");
							strLocation[1] = _T("svalue");
							strLocation[2] = _T("n0");
							ParseJson(strTemp2, strLocation, strTemp3);
							strLocation[0] = _T("4");
							strLocation[1] = _T("sposp");
							strLocation[2] = _T("n0");
							strLocation[3] = _T("slabel");
							if (!ParseJson(strTemp3, strLocation, strTempEnd)) {
								continue;
							}
							strCollins += _T("->") + strTempEnd.Mid(1, strTempEnd.GetLength() - 2) + _T(": ");
							strLocation[0] = _T("2");
							strLocation[1] = _T("tran");
							ParseObject(strTemp3, strLocation, strTempEnd);
							strCollins += strTempEnd.Mid(1, strTempEnd.GetLength() - 2) + _T(" ");
							strLocation[0] = _T("2");
							strLocation[1] = _T("def");
							ParseObject(strTemp3, strLocation, strTempEnd);
							strCollins += strTempEnd.Mid(1, strTempEnd.GetLength() - 2) + _T("\r\n");
						}
					}
				}
			}
		}
		strCollins.Replace(_T("\\/"), _T("/"));
		strCollins.Replace(_T("<b>"), _T(""));
		strCollins.Replace(_T("</b>"), _T(""));
#endif // 0==0 //just for collapse it ***this is get Collins resutlt;


#if 0==0 //just for collapse it **get king soft means ,states, pronuciation
		strLocation[0] = _T("2");
		strLocation[1] = _T("simple_means");
		if (ParseObject(strSubRes, strLocation, strTemp1)) {
			strLocation[1] = _T("exchange");
			if (ParseObject(strTemp1, strLocation, strTemp2)) {
				strExchange.Empty();
				strLocation[1] = _T("word_done");
				if (ParseObject(strTemp2, strLocation, strTempEnd)) {
					strTempEnd.Remove(_T('"'));
					strTempEnd.Remove(_T('['));
					strTempEnd.Remove(_T(']'));
					if (!strTempEnd.IsEmpty()) {
						strExchange += _T("done:") + strTempEnd + _T(" ");
					}
				}
				strLocation[1] = _T("word_er");
				if (ParseObject(strTemp2, strLocation, strTempEnd)) {
					strTempEnd.Remove(_T('"'));
					strTempEnd.Remove(_T('['));
					strTempEnd.Remove(_T(']'));
					if (!strTempEnd.IsEmpty()) {
						strExchange += _T("er:") + strTempEnd + _T(" ");
					}
				}
				strLocation[1] = _T("word_est");
				if (ParseObject(strTemp2, strLocation, strTempEnd)) {
					strTempEnd.Remove(_T('"'));
					strTempEnd.Remove(_T('['));
					strTempEnd.Remove(_T(']'));
					if (!strTempEnd.IsEmpty()) {
						strExchange += _T("est:") + strTempEnd + _T(" ");
					}
				}
				strLocation[1] = _T("word_ing");
				if (ParseObject(strTemp2, strLocation, strTempEnd)) {
					strTempEnd.Remove(_T('"'));
					strTempEnd.Remove(_T('['));
					strTempEnd.Remove(_T(']'));
					if (!strTempEnd.IsEmpty()) {
						strExchange += _T("ing:") + strTempEnd + _T(" ");
					}
				}
				strLocation[1] = _T("word_past");
				if (ParseObject(strTemp2, strLocation, strTempEnd)) {
					strTempEnd.Remove(_T('"'));
					strTempEnd.Remove(_T('['));
					strTempEnd.Remove(_T(']'));
					if (!strTempEnd.IsEmpty()) {
						strExchange += _T("past:") + strTempEnd + _T(" ");
					}
				}
				strLocation[1] = _T("word_pl");
				if (ParseObject(strTemp2, strLocation, strTempEnd)) {
					strTempEnd.Remove(_T('"'));
					strTempEnd.Remove(_T('['));
					strTempEnd.Remove(_T(']'));
					if (!strTempEnd.IsEmpty()) {
						strExchange += _T("pl:") + strTempEnd + _T(" ");
					}
				}
				strLocation[1] = _T("word_third");
				if (ParseObject(strTemp2, strLocation, strTempEnd)) {
					strTempEnd.Remove(_T('"'));
					strTempEnd.Remove(_T('['));
					strTempEnd.Remove(_T(']'));
					if (!strTempEnd.IsEmpty()) {
						strExchange += _T("third:") + strTempEnd + _T(" ");
					}
				}
			}
			strLocation[0] = _T("3");
			strLocation[1] = _T("ssymbols");
			strLocation[2] = _T("n0");
			if (ParseJson(strTemp1, strLocation, strTemp2)) {
				strLocation[0] = _T("2");
				strLocation[1] = _T("ph_am");
				if (ParseObject(strTemp2, strLocation, strTempEnd)) {
					strPronu = strTempEnd.Mid(1, strTempEnd.GetLength() - 2);
				}
				strLocation[1] = _T("parts");
				if (ParseObject(strTemp2, strLocation, strTemp3)) {
					strWord.Empty();
					nCount = ParseArray(strTemp3, NULL);
					for (n = 0; n < nCount; ++n) {
						nLocation[0] = 2;
						nLocation[1] = n;
						ParseArray(strTemp3, nLocation, strTemp4);
						strLocation[0] = _T("2");
						strLocation[1] = _T("part");
						ParseObject(strTemp4, strLocation, strTempEnd);
						strWord += _T("->") + strTempEnd.Mid(1, strTempEnd.GetLength() - 2) + _T(" ");
						strLocation[0] = _T("2");
						strLocation[1] = _T("means");
						ParseObject(strTemp4, strLocation, strTempEnd);
						strTempEnd.Remove(_T('"'));
						strTempEnd.Remove(_T('['));
						strTempEnd.Remove(_T(']'));
						strWord += strTempEnd + _T("\r\n");
					}
				}
			}
		}

#endif // 0==0 //just for collapse it **get king soft means ,states, pronuciation

	}
	return true;
}

bool CTrans::GgTrans(const WCHAR* szEn, CString &strTrans, CString &strWord, CString &strPronu, CString &strDefine) {
	int n;
	if (strDir.IsEmpty()) {
		if (!InitDir()) {
			return false;
		}
	}
	CString strKey;
	if (!GetKey(szEn, strKey)) {
		return false;
	}
	CString strUrl;
	strUrl.Format(_T("https://translate.google.com/translate_a/single?client=t&sl=en&tl=zh-CN&hl=en&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=bh&ssel=0&tsel=0&kc=0&tk=%s&q=%s"), strKey, szEn);

	CStringA strATemp;
	int nCount = 0;
	try {
		CInternetSession internetSession;
		internetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, connectTimeout);
		internetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, connectTimeout);
		CHttpFile* httpFile;
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
		if (NULL == httpFile) {
			return false;
		}
		char buffer[8192];
		do {
			nCount = httpFile->Read(buffer, 8191);
			buffer[nCount] = 0;
			strATemp += buffer;
		} while (nCount > 0);
		httpFile->Close();

	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (-1 != strATemp.Find("<html>")) {
		return false;
	}
	CString strHtml;
	if (!Decoding(strATemp, strHtml)) {
#ifdef DEBUG
		AfxMessageBox(_T("Decoding Failed!"));
#endif // DEBUG
		return false;
	}
	int nWordLoca[] = { 2, 1 };
	int nTransLoca[] = { 2, 5 };
	int nTemp[10];
	int nLen;
	int nLen1;
	int n1;
	CString strTemp0;
	CString strTemp1;
	CString strTemp2;
	CString strTemp3;
	CString strTemp4;
	nTemp[0] = 4;
	nTemp[1] = 0;
	nTemp[2] = 1;
	nTemp[3] = 3;
	if (ParseArray(strHtml, nTemp, strTemp0)) {
		strPronu = strTemp0.Mid(1, strTemp0.GetLength() - 2);
	}
	if (ParseArray(strHtml, nWordLoca, strTemp0) && _T("") != strTemp0) {
		nLen = ParseArray(strTemp0, NULL);
		for (n = 0; n < nLen; ++n) {
			nTemp[0] = 2;
			nTemp[1] = n;
			ParseArray(strTemp0, nTemp, strTemp1);
			nTemp[1] = 0;
			ParseArray(strTemp1, nTemp, strTemp2);
			strWord += strTemp2.Mid(1, strTemp2.GetLength() - 2) + _T(":\r\n");
			nTemp[1] = 2;
			ParseArray(strTemp1, nTemp, strTemp2);
			nLen1 = ParseArray(strTemp2, NULL);
			for (n1 = 0; n1 < nLen1; ++n1) {
				nTemp[0] = 2;
				nTemp[1] = n1;
				ParseArray(strTemp2, nTemp, strTemp3);
				nTemp[1] = 3;
				ParseArray(strTemp3, nTemp, strTemp4);
				if (_T('0') != strTemp4[0]) {
					strWord += _T("--- ");
				}
				else {
					strWord += strTemp4.Mid(2, 3) + _T(' ');
				}
				nTemp[1] = 0;
				ParseArray(strTemp3, nTemp, strTemp4);
				strWord += strTemp4.Mid(1, strTemp4.GetLength() - 2) + _T(" ");
				nTemp[1] = 1;
				ParseArray(strTemp3, nTemp, strTemp4);
				strTemp4 = strTemp4.Mid(1, strTemp4.GetLength() - 2);
				strTemp4.Remove(_T('"'));
				strTemp4.Replace(_T(","), _T(", "));
				strWord += strTemp4 + _T("\r\n");
			}
		}
	}
	if (ParseArray(strHtml, nTransLoca, strTemp0)) {
		nLen = ParseArray(strTemp0, NULL);
		if (1 == nLen) {
			nTemp[0] = 3;
			nTemp[1] = 0;
			nTemp[2] = 2;
			ParseArray(strTemp0, nTemp, strTemp1);
			nLen = ParseArray(strTemp1, NULL);
			for (n = 0; n < nLen; ++n) {
				nTemp[0] = 3;
				nTemp[1] = n;
				nTemp[2] = 0;
				ParseArray(strTemp1, nTemp, strTemp2);
				strTrans += _T("@T: ") + strTemp2.Mid(1, strTemp2.GetLength() - 2) + _T("\r\n");
			}
		}
		else {
			strTrans += _T("@T: ");
			for (n = 0; n < nLen; ++n) {
				nTemp[0] = 5;
				nTemp[1] = n;
				nTemp[2] = 2;
				nTemp[3] = 0;
				nTemp[4] = 0;
				ParseArray(strTemp0, nTemp, strTemp1);
				strTrans += strTemp1.Mid(1, strTemp1.GetLength() - 2);
			}
			strTrans += _T("\r\n");
		}
	}

	nTemp[0] = 2;
	nTemp[1] = 12;
	if (ParseArray(strHtml, nTemp, strTemp0)) {
		nCount = ParseArray(strTemp0, NULL);
		if (0 < nCount) {
			strDefine.Empty();
			for (n = 0; n < nCount; ++n) {
				nTemp[0] = 2;
				nTemp[1] = n;
				ParseArray(strTemp0, nTemp, strTemp1);
				nTemp[0] = 2;
				nTemp[1] = 0;
				ParseArray(strTemp1, nTemp, strTemp2);
				strDefine += strTemp2.Mid(1, strTemp2.GetLength() - 2) + _T(".\r\n");
				nTemp[0] = 4;
				nTemp[1] = 1;
				nTemp[2] = 0;
				nTemp[3] = 0;
				ParseArray(strTemp1, nTemp, strTemp2);
				strDefine += _T("  ") + strTemp2.Mid(1, strTemp2.GetLength() - 2) + _T("\r\n");
			}
		}
	}
	return true;
}

bool CTrans::CollinsPronu(const WCHAR* szEn, const WCHAR* szFileName) {
	CString strKey;
	CString strUrl;
	CStringA strATemp;
	CString strHtml;
	int nLoca1, nLoca2;
	int n;
	int nCount;
	strUrl.Format(_T("http://www.collinsdictionary.com/dictionary/english/%s"), szEn);
	CInternetSession internetSession;
	internetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, connectTimeout);
	internetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, connectTimeout);
	CHttpFile* httpFile;
	try {
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (NULL == httpFile) {
		return false;
	}
	char buffer[8192];
	do {
		try {
			nCount = httpFile->Read(buffer, 8191);
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			return false;
		}
		buffer[nCount] = 0;
		strHtml += buffer;
	} while (nCount);
	nLoca1 = strHtml.Find(_T("hwd_sound sound audio_play_button icon-volume-up ptr"));
	if (-1 == nLoca1) {
		return false;
	}
	nLoca1 = strHtml.Find(_T("data-src-mp3"), nLoca1) + 14;
	if (-1 == nLoca1) {
		return false;
	}
	nLoca2 = strHtml.Find(_T('"'), nLoca1) - 1;
	strUrl.Format(_T("http://www.collinsdictionary.com%s"), strHtml.Mid(nLoca1, nLoca2 - nLoca1 + 1));
	try {
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (NULL == httpFile) {
		return false;
	}
	CFile file(szFileName, CFile::modeCreate | CFile::modeWrite);
	do {
		try {
			nCount = httpFile->Read(buffer, 8192);
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			return false;
		}
		file.Write(buffer, nCount);
	} while (nCount);
	file.Close();
	httpFile->Close();
	buffer[8191] = 0;
	strATemp = buffer;
	n = strATemp.Find("html");
	if (-1 != n) {
		_wremove(szFileName);
		return false;
	}
	return true;
}
bool CTrans::CambridgePronu(const WCHAR* szEn, const WCHAR* szFileName) {
	CString strKey;
	CString strUrl;
	CStringA strATemp;
	CString strHtml;
	int nLoca1, nLoca2;
	int n;
	int nCount;
	strUrl.Format(_T("http://dictionary.cambridge.org/us/dictionary/english/%s"), szEn);
	CInternetSession internetSession;
	internetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, connectTimeout);
	internetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, connectTimeout);
	CHttpFile* httpFile;
	try {
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (NULL == httpFile) {
		return false;
	}
	char buffer[8192];
	do {
		try {
			nCount = httpFile->Read(buffer, 8191);
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			return false;
		}
		buffer[nCount] = 0;
		strHtml += buffer;
	} while (nCount);
	nLoca1 = strHtml.Find(_T("<span class=\"region\">us</span>"));
	if (-1 == nLoca1) {
		return false;
	}
	nLoca1 = strHtml.Find(_T("data-src-mp3"), nLoca1) + 14;
	if (-1 == nLoca1) {
		return false;
	}
	nLoca2 = strHtml.Find(_T('"'), nLoca1) - 1;
	strUrl = strHtml.Mid(nLoca1, nLoca2 - nLoca1 + 1);
	try {
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (NULL == httpFile) {
		return false;
	}
	CFile file(szFileName, CFile::modeCreate | CFile::modeWrite);
	do {
		try {
			nCount = httpFile->Read(buffer, 8192);
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			return false;
		}
		file.Write(buffer, nCount);
	} while (nCount);
	file.Close();
	httpFile->Close();
	buffer[8191] = 0;
	strATemp = buffer;
	n = strATemp.Find("html");
	if (-1 != n) {
		_wremove(szFileName);
		return false;
	}
	return true;
}
bool CTrans::BaidPronu(const WCHAR* szEn, const WCHAR* szFileName) {
	CString strKey;
	CString strUrl;
	CStringA strATemp;
	int n;
	int nCount;
	strUrl.Format(_T("http://fanyi.baidu.com/gettts?lan=en&text=%s&source=web"), szEn);
	CInternetSession internetSession;
	internetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, connectTimeout);
	internetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, connectTimeout);
	CHttpFile* httpFile;
	try {
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (NULL == httpFile) {
		return false;
	}
	CFile file(szFileName, CFile::modeCreate | CFile::modeWrite);
	char buffer[8192];
	do {
		try {
			nCount = httpFile->Read(buffer, 8192);
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			return false;
		}
		file.Write(buffer, nCount);
	} while (nCount);
	file.Close();
	httpFile->Close();
	buffer[8191] = 0;
	strATemp = buffer;
	n = strATemp.Find("html");
	if (-1 != n) {
		_wremove(szFileName);
		return false;
	}
	return true;
}
bool CTrans::GoogPronu(const WCHAR* szEn, const WCHAR* szFileName) {
	CString strKey;
	CString strUrl;
	CStringA strATemp;
	int n;
	int nCount;
	if (!GetKey(szEn, strKey)) {
		return false;
	}
	strUrl.Format(_T("https://translate.google.com/translate_tts?ie=UTF-8&q=%s&tl=en&total=1&idx=0&textlen=%d&tk=%s&client=t&prev=input"), szEn, wcslen(szEn), strKey);
	CInternetSession internetSession;
	internetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, connectTimeout);
	internetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, connectTimeout);
	CHttpFile* httpFile;
	try {
		httpFile = (CHttpFile*)internetSession.OpenURL(strUrl);
	}
	catch (CInternetException* e) {
#ifdef DEBUG
		e->ReportError();
#endif // DEBUG
		e->Delete();
		return false;
	}
	if (NULL == httpFile) {
		return false;
	}
	CFile file(szFileName, CFile::modeCreate | CFile::modeWrite);
	char buffer[8192];
	do {
		try {
			nCount = httpFile->Read(buffer, 8192);
		}
		catch (CInternetException* e) {
#ifdef DEBUG
			e->ReportError();
#endif // DEBUG
			e->Delete();
			return false;
		}
		file.Write(buffer, nCount);
	} while (nCount);
	file.Close();
	httpFile->Close();
	buffer[8191] = 0;
	strATemp = buffer;
	n = strATemp.Find("html");
	if (-1 != n) {
		_wremove(szFileName);
		return false;
	}
	return true;
}


bool CTrans::ParseJson(const WCHAR* szJson, CString* strLocation, CString &strSub) {
	int nArrayLen = _wtoi(strLocation[0]);
	CString strTemp1 = szJson;
	CString strTemp2;
	CString strLoca0;
	int nLoca[2];
	nLoca[0] = 2;
	CString strLoca[2];
	strLoca[0].Format(_T("%d"), 2);
	for (int n = 1; n < nArrayLen; ++n) {
		if (_T('n') == strLocation[n][0]) {
			strLoca0 = strLocation[n].Right(strLocation[n].GetLength() - 1);
			nLoca[1] = _wtoi(strLoca0);
			if (!ParseArray(strTemp1, nLoca, strTemp2)) {
				return false;
			}
		}
		else if (_T('s') == strLocation[n][0]){
			strLoca0 = strLocation[n].Right(strLocation[n].GetLength() - 1);
			strLoca[1] = strLoca0;
			if (!ParseObject(strTemp1, strLoca, strTemp2)) {
				return false;
			}
		}
		else {
#ifdef DEBUG
			AfxMessageBox(_T("Location index string format error!"));
#endif // DEBUG
			return false;
		}
		strTemp1 = strTemp2;
	}
	strSub = strTemp1;
	return true;
}
/*
*strLocation[0] is array length;
*/
bool CTrans::ParseObject(const WCHAR* szObject, CString* strLocation, CString &strSub) {
	int nArrayLen = _wtoi(strLocation[0]);
	int n;
	bool wheContinue;
	CString strObject = szObject;
	int nStrLen;
	int nLoca1;
	int nLoca2;
	int nLoca3;
	int nLoca4;


	for (n = 1; n < nArrayLen; ++n) {
		nStrLen = strObject.GetLength();
		if (_T('{') != strObject[0] || _T('}') != strObject[nStrLen - 1]) {
#ifdef DEBUG
			OutputDebugString(_T("Json's Object Error"));
#endif // DEBUG
			return false;
		}
		if (_T('"') != strObject[1]) {
#ifdef DEBUG
			AfxMessageBox(_T("Json's Object Error"));
#endif // DEBUG
			return false;
		}
		nLoca4 = -1;
		wheContinue = true;
		do
		{
			nLoca1 = nLoca4 + 2;
			nLoca2 = FindMatchQuote(strObject, nLoca1);
			nLoca3 = nLoca2 + 2;
			if (_T('{') == strObject[nLoca3]) {
				nLoca4 = FindMatchBrace(strObject, nLoca3);
				//Check whither right;
				if (-1 == nLoca4) {
					return false;
				}
			}
			else if (_T('[') == strObject[nLoca3]) {
				nLoca4 = FindMatchBrace(strObject, nLoca3);
				//Check whither right;
				if (-1 == nLoca4) {
					return false;
				}
			}
			else if (_T('"') == strObject[nLoca3]) {
				nLoca4 = FindMatchQuote(strObject, nLoca3);
				if (-1 == nLoca4) {
					nLoca4 = strObject.Find(_T('}'), nLoca3) - 1;
				}
			}
			else {
				nLoca4 = strObject.Find(_T(','), nLoca3);
				if (-1 == nLoca4) {
					nLoca4 = strObject.Find(_T('}'), nLoca3) - 1;
				}
				else {
					--nLoca4;
				}
			}
			if (strLocation[n] == strObject.Mid(nLoca1 + 1, nLoca2 - nLoca1 - 1)) {
				wheContinue = false;
			}
			if (nLoca4 + 2 >= nStrLen && wheContinue) {
#ifdef DEBUG
				OutputDebugString(_T("Can not find the object element specified!"));
#endif // DEBUG
				return false;
			}
		} while (wheContinue);
		strObject = strObject.Mid(nLoca3, nLoca4 - nLoca3 + 1);
	}
	strSub = strObject;
	return true;
}

//////////////////////////
//nLocation the first Element store the array length;
bool CTrans::ParseArray(const WCHAR* szArray, int* nLocation, CString &strSub) {
	if (NULL == nLocation) {
		strSub = szArray;
		return true;
	}
	CString strArray = szArray;
	int nArrayLen = nLocation[0];
	int nStrLen;
	int nLoca1, nLoca2; //sub member location;
	int n;
	int nCount;
	bool wheContinue;

	for (n = 1; n < nArrayLen; ++n) {
		nStrLen = strArray.GetLength();
		if (_T('[') != strArray[0] || _T(']') != strArray[nStrLen - 1]) {
#ifdef DEBUG
			OutputDebugString(_T("Array format error!"));
#endif // DEBUG
			return false;
		}
		nLoca1 = 1;
		nCount = 0;
		do { //find sub member's location
			if (_T('"') == strArray[nLoca1]) {
				nLoca2 = FindMatchQuote(strArray, nLoca1);
			}
			else if (_T('[') == strArray[nLoca1]) {
				nLoca2 = FindMatchBrace(strArray, nLoca1);
			}
			else if (_T('{') == strArray[nLoca1]) {
				nLoca2 = FindMatchBrace(strArray, nLoca1);
			}
			else {
				nLoca2 = strArray.Find(_T(','), nLoca1);
				if (-1 == nLoca2) {
					nLoca2 = nStrLen - 2;
				}
				else {
					nLoca2 -= 1;
				}
			}

			wheContinue = true;
			if (-1 == nLoca2) {
#ifdef DEBUG
				AfxMessageBox(_T("Parsing Array failed!"));
#endif // DEBUG
				return false;
			}
			else {
				if (nLocation[n] == nCount) {
					wheContinue = false;
				}
				else {
					// check whether in the end of the array:
					if (nLoca2 + 2 >= nStrLen) {
#ifdef DEBUG
						OutputDebugStringW(_T("exceed Array bound!"));
#endif // DEBUG
						return false;
					}
					nLoca1 = nLoca2 + 2;
					++nCount;
				}
			}
		} while (wheContinue);
		strArray = strArray.Mid(nLoca1, nLoca2 - nLoca1 + 1);
	}
	strSub = strArray;
	return true;
}
int CTrans::ParseArray(const WCHAR* szArray, int* nLocation) {
	CString strArray;
	int nLoca1, nLoca2; // the array members location;
	int nCount = 0;
	int nStrLen;

	if (!ParseArray(szArray, nLocation, strArray)) {
#ifdef DEBUG
		AfxMessageBox(_T("Get sub Array failed"));
#endif // DEBUG
		return -1;
	}
	nStrLen = strArray.GetLength();

	if (_T('[') != strArray[0] || _T(']') != strArray[nStrLen - 1]) {
#ifdef DEBUG
		AfxMessageBox(_T("Array format error!"));
#endif // DEBUG
		return -1;
	}
	nLoca1 = 1;
	do {
		++nCount;
		if (_T('"') == strArray[nLoca1]) {
			nLoca2 = FindMatchQuote(strArray, nLoca1);
		}
		else if (_T('[') == strArray[nLoca1]) {
			nLoca2 = FindMatchBrace(strArray, nLoca1);
		}
		else if (_T('{') == strArray[nLoca1]) {
			nLoca2 = FindMatchBrace(strArray, nLoca1);
		}
		else {
			nLoca2 = strArray.Find(_T(','), nLoca1);
			if (-1 == nLoca2) {
				nLoca2 = nStrLen - 2;
			}
			else {
				nLoca2 -= 1;
			}
		}
		if (-1 == nLoca2) {
#ifdef DEBUG
			AfxMessageBox(_T("Parsing Array failed!"));
#endif // DEBUG
			return false;
		}
		else {
			// check whether in the end of the array:
			if (nLoca2 + 2 == nStrLen) {
				return nCount;
			}
			else {
				nLoca1 = nLoca2 + 2;
			}
		}
	} while (true);
}

int CTrans::FindMatchBrace(CString &strTemp, const int nLoca1) {
	WCHAR wcBraceLeft = strTemp[nLoca1];
	WCHAR wcBraceRight;
	if (_T('[') == wcBraceLeft) {
		wcBraceRight = _T(']');
	}
	else if (_T('{') == wcBraceLeft) {
		wcBraceRight = _T('}');
	}
	else {
#ifdef DEBUG
		AfxMessageBox(_T("Object or Array which is parsed format error!"));
#endif // DEBUG
		return -1;
	}

	int nLoca2;
	int nLoca3;
	int nLoca4;
	int nLocaT1;
	int nLocaT2;
	int nCount;
	std::vector<int> vecQuote;
	bool wheFind;
	bool wheContinue;
	int n;
	//nLoca1, nLoca2 is [] location;
	//nLoca3, nLoca4 is quote vector temp location;
	//nLocaT1 is vector Temp Location
	//nLocaT2 is [ temp location

	//if it's array;
	nLoca2 = nLoca1;
	nLocaT2 = nLoca2;
	nLoca3 = strTemp.Find(_T('"'), nLoca2);
	vecQuote.push_back(nLoca3);
	nLoca4 = nLoca3;
	wheFind = true;
	if (-1 != nLoca3) {
		do {
			nLoca4 = strTemp.Find(_T('"'), nLoca4 + 1);
			if (_T('\\') == strTemp[nLoca4 - 1]) {
				nLocaT1 = nLoca4 - 2;
				nCount = 0;
				while (_T('\\') == strTemp[nLoca4]) {
					++nCount;
					--nLocaT1;
				}
				if (1 == nCount % 2) {
					wheFind = false;
				}
			}
			else {
				wheFind = false;
			}
		} while (wheFind);
	}
	vecQuote.push_back(nLoca4);
	//find end of {[]} valid
	do {
		//find next ']}' valid
		do {
			nLoca2 = strTemp.Find(wcBraceRight, nLoca2 + 1);
			while (vecQuote.at(vecQuote.size() - 1) < nLoca2) {
				nLoca3 = strTemp.Find(_T('"'), nLoca4 + 1);
				if (-1 == nLoca3) {
					break;
				}
				wheFind = true;
				nLoca4 = nLoca3;
				do {
					nLoca4 = strTemp.Find(_T('"'), nLoca4 + 1);
					if (_T('\\') == strTemp[nLoca4 - 1]) {
						nLocaT1 = nLoca4 - 2;
						nCount = 0;
						while (_T('\\') == strTemp[nLoca4]) {
							++nCount;
							--nLocaT1;
						}
						if (1 == nCount % 2) {
							wheFind = false;
						}
					}
					else {
						wheFind = false;
					}
				} while (wheFind);
				vecQuote.push_back(nLoca3);
				vecQuote.push_back(nLoca4);
			}
			wheContinue = false;
			for (n = 0; (UINT)n < vecQuote.size(); n += 2) {
				if (vecQuote.at(n) < nLoca2 && vecQuote.at(n + 1) > nLoca2) {
					wheContinue = true;
				}
			}
		} while (wheContinue);
		//find next '[{' valid
		do {
			nLocaT2 = strTemp.Find(wcBraceLeft, nLocaT2 + 1);
			while (vecQuote.at(vecQuote.size() - 1) < nLocaT2) {
				nLoca3 = strTemp.Find(_T('"'), nLoca4 + 1);
				if (-1 == nLoca3) {
					break;
				}
				nLoca4 = nLoca3;
				wheFind = true;
				do {
					nLoca4 = strTemp.Find(_T('"'), nLoca4 + 1);
					if (_T('\\') == strTemp[nLoca4 - 1]) {
						nLocaT1 = nLoca4 - 2;
						nCount = 0;
						while (_T('\\') == strTemp[nLoca4]) {
							++nCount;
							--nLocaT1;
						}
						if (1 == nCount % 2) {
							wheFind = false;
						}
					}
					else {
						wheFind = false;
					}
				} while (wheFind);
				vecQuote.push_back(nLoca3);
				vecQuote.push_back(nLoca4);
			}
			wheContinue = false;
			for (n = 0; (UINT)n < vecQuote.size(); n += 2) {
				if (vecQuote.at(n) < nLocaT2 && vecQuote.at(n + 1) > nLocaT2) {
					wheContinue = true;
				}
			}
		} while (wheContinue);
		wheContinue = false;
		if (-1 == nLoca2) {
#ifdef DEBUG
			AfxMessageBox(_T("Parsing Json failed!"));
#endif // DEBUG
			return -1;
		}
		if (nLocaT2 < nLoca2 && -1 != nLocaT2) {
			wheContinue = true;
		}
	} while (wheContinue);
	return nLoca2;
}
int CTrans::FindMatchQuote(CString &strTemp, const int nLoca1) {
	int nLoca2;
	int nLocaT1;
	bool wheFind;
	int nCount;
	nLoca2 = nLoca1;
	wheFind = true;
	if (-1 != nLoca1) {
		do {
			nLoca2 = strTemp.Find(_T('"'), nLoca2 + 1);
			if (-1 == nLoca2) {
#ifdef DEBUG
				AfxMessageBox(_T("Format error! can't find next valid quote"));
#endif // DEBUG
				return -1;
			}
			if (_T('\\') == strTemp[nLoca2 - 1]) {
				nLocaT1 = nLoca2 - 2;
				nCount = 0;
				while (_T('\\') == strTemp[nLoca2]) {
					++nCount;
					--nLocaT1;
				}
				if (1 == nCount % 2) {
					wheFind = false;
				}
			}
			else {
				wheFind = false;
			}
		} while (wheFind);
	}
	return nLoca2;
}


UINT __cdecl CTrans::BdTransThread(LPVOID pParam) {
	ThreadParam* threadParam = (ThreadParam*)pParam;
	threadParam->status = 0;
	if (BdTrans(threadParam->szEn, *threadParam->strTrans, *threadParam->strWord, *threadParam->strPronu, *threadParam->strExchange, *threadParam->strCollins, *threadParam->strEnEn)) {
		threadParam->status = 1;
	}
	else {
		threadParam->status = -1;
	}
	return 0;
}
UINT __cdecl CTrans::GgTransThread(LPVOID pParam) {
	ThreadParam* threadParam = (ThreadParam*)pParam;
	threadParam->status = 0;
	if (GgTrans(threadParam->szEn, *threadParam->strTrans, *threadParam->strWord, *threadParam->strPronu, *threadParam->strDefine)) {
		threadParam->status = 1;
	}
	else {
		threadParam->status = -1;
	}
	return 0;
}
UINT __cdecl CTrans::CollinsPronuThread(LPVOID pParam) {
	ThreadParam* threadParam = (ThreadParam*)pParam;
	threadParam->status = 0;
	if (CollinsPronu(threadParam->szEn, threadParam->szFileName)) {
		threadParam->status = 1;
	}
	else {
		threadParam->status = -1;
	}
	return 0;
}
UINT __cdecl CTrans::CambridgePronuThread(LPVOID pParam) {
	ThreadParam* threadParam = (ThreadParam*)pParam;
	threadParam->status = 0;
	if (CambridgePronu(threadParam->szEn, threadParam->szFileName)) {
		threadParam->status = 1;
	}
	else {
		threadParam->status = -1;
	}
	return 0;
}
UINT __cdecl CTrans::BaidPronuThread(LPVOID pParam) {
	ThreadParam* threadParam = (ThreadParam*)pParam;
	threadParam->status = 0;
	if (BaidPronu(threadParam->szEn, threadParam->szFileName)) {
		threadParam->status = 1;
	}
	else {
		threadParam->status = -1;
	}
	return 0;
}
UINT __cdecl CTrans::GoogPronuThread(LPVOID pParam) {
	ThreadParam* threadParam = (ThreadParam*)pParam;
	threadParam->status = 0;
	if (GoogPronu(threadParam->szEn, threadParam->szFileName)) {
		threadParam->status = 1;
	}
	else {
		threadParam->status = -1;
	}
	return 0;
}

