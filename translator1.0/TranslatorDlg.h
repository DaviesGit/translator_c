//////////////////////////////////////
//


void CALLBACK GetClipboardText(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime);

class CTransDlg : public CDialog {
	enum ClickType{ 
		CLOSEBTN,
		MOVEBAR,
		UNKNOWNTYPE

	};


///////////////////////////////////////
// function
public:
	void ClipboardTrans(const char* szEn);

protected:
	 int GetClickType(CPoint point);
	 virtual BOOL OnInitDialog();


protected:
	//{{AFX
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnClose();
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnTranslate();
		afx_msg void OnMciNotify();
		afx_msg void OnHotKey(WPARAM wParam, LPARAM lParam);
	//}}AFX
	
	DECLARE_MESSAGE_MAP()

};
