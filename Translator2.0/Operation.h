///////////////////////////////////////////////////////////////
//
#include <mmsystem.h>

class CTransWnd {
public:
	enum WndState {
		FOLD,
		UNFOLD,
		INFOLD,
		INUNFOLD,
		READYFOLD
	};
private:
	static char *animate[16];

public:
	static void TransWndInit();
	static void FoldWnd();
	static void UnfoldWnd();
	static void SetInTxt(const char* szText);
	static void SetOutTxt(const char* szText);
	static WndState GetDlgState();
	static void SetDlgState(WndState state);
	static void BeginTransAnimate();
	static void StopTransAnimate();
	static void Trans();
	static void BeginPronun();
	static void StopPronun();

private:
	static UINT FoldWndThread(LPVOID pParam);
	static UINT UnfoldWndThread(LPVOID pParam);
	static UINT TransAnimateThread(LPVOID pParam);
	static UINT TransThread(LPVOID pParam);
	static UINT PronunThread(LPVOID pParam);

private:
	static WndState nWndState;
/*
	static CWinThread *thrFoldWnd;
	static CWinThread *thrUnfoldWnd;
	static CWinThread *thrAnimate;
	static CWinThread *thrTrans;
	static CWinThread *thrPronun;
*/
	static bool whetherExitUnfoldThread;
	static bool whetherExitFoldThread;
	static bool whetherStopReadyFold;
	static bool whetherExitAnimateThread;
	static bool whetherOnTrans;
	static bool whetherOnPronun;
	static MCI_OPEN_PARMS mciOpen;

};

struct StuParam {
	void (*Function)();
	int Time;
};
//////////////////////////////////////////////////////////////////
void RunLater(void (*Function)(), int Time);
UINT RunLater(LPVOID pParam);
////////////////////////////////////////////////////////////////////