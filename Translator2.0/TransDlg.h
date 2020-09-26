/////////////////////////////////////////////////////////
//




///////////////////////////////////////////////////////////////


class CTransDlg : public CDialog {

private:
	void SetDlgState();
	bool WhetherEnTxt(const char* Text);
	bool GetClipboard(CString &strClipboard);

public:
	CTransDlg();

protected:

	afx_msg void OnClose();
	afx_msg void OnTrans();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHotkey(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	
	DECLARE_MESSAGE_MAP()
public:
	static CString strLastClipboard;

};