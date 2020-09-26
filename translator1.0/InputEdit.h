////////////////////////////////////////
//

class CInputEdit : public CEdit {
	
	//{{AFX
		afx_msg void OnPaint();
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//}}AFX

	DECLARE_MESSAGE_MAP()
};