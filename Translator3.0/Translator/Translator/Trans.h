///////////////////////////////////////////////////////////////////////////////
//Using google translate engine
//

#include <atlstr.h>

///////////////////
//Ctrans

class CTrans
{
public:
	static bool GetTransRes(const WCHAR* szEn, CString &strRes);
	static bool GetPronu(const WCHAR* szEn, CString* strFile);
	static bool ChToEn(const WCHAR* szCh, CString &strEn);

private:
	static bool BdTrans(const WCHAR* szEn, CString &strTrans, CString &strWord, CString &strExchange, CString &strPronu, CString &strCollins, CString &strEnEn);
	static bool GgTrans(const WCHAR* szEn, CString &strTrans, CString &strWord, CString &strPronu, CString &strDefine);

	static bool CollinsPronu(const WCHAR* szEn, const WCHAR* szFileName);
	static bool CambridgePronu(const WCHAR* szEn, const WCHAR* szFileName);
	static bool BaidPronu(const WCHAR* szEn, const WCHAR* szFileName);
	static bool GoogPronu(const WCHAR* szEn, const WCHAR* szFileName);

	static UINT __cdecl BdTransThread(LPVOID pParam);
	static UINT __cdecl GgTransThread(LPVOID pParam);
	static UINT __cdecl CollinsPronuThread(LPVOID pParam);
	static UINT __cdecl CambridgePronuThread(LPVOID pParam);
	static UINT __cdecl BaidPronuThread(LPVOID pParam);
	static UINT __cdecl GoogPronuThread(LPVOID pParam);

	static bool WheWord(const WCHAR* szEn);
	static bool Encoding(const WCHAR* szText, CStringA &strText);
	static bool BaidDecoding(const char* szText, CString &strText);
	static bool Decoding(const char* szText, CString &strText);
	static bool GetKey(const WCHAR* szEn, CString &strKey);
	static bool ParseJson(const WCHAR* szJson, CString* strLocation, CString &strSub);
	static bool ParseObject(const WCHAR* szObject, CString* strLocation, CString &strSub);
	static bool ParseArray(const WCHAR* szArray, int* nLocation, CString &strSub);
	static int ParseArray(const WCHAR* szArray, int* nLocation);
	static int FindMatchBrace(CString &strTemp, const int nLoca1);
	static int FindMatchQuote(CString &strTemp, const int nLoca1);
	static __int64 GoogleHk(__int64 a, const char* b);
	static bool InitDir();

	static __int64 nKey1;
	static __int64 nKey2;
	static CString strDir;
	static bool wheInGetKey;
	static DWORD connectTimeout;
};

struct ThreadParam {
	int status;
	const WCHAR *szEn, *szFileName;
	CString *strTrans, *strWord, *strExchange, *strPronu, *strCollins, *strEnEn, *strDefine;

};

