//
//



class CTranslate {

public:
	CTranslate();
	~CTranslate();

public:
	static bool Translate(const char* strEn, CString &dictResult, CString &transResult);
	static bool GetPronunciation(const char* szEn, CString &strAudioPath);
	static bool ChToEn(const char* szCh, CString &strTransResult, CString &strDictResult);

private:
	static bool Decode(const char* szUnicode, CString &gb2312Str);
	static bool Code(const char* szGb2312, CString &strUnicode);
};






