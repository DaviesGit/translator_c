//
//

#include <afxinet.h>
#include <fstream>

class CTranslate {

public:
	CTranslate();
	~CTranslate();

	static int Translate(const char* strEn, CString &dictResult, CString &transResult);

	static bool GetPronunciation(const char* szEn, CString &strAudioPath);

private:
	static bool Decode(const char* szUnicode, CString &gb2312Str);
};






