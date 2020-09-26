////////////////////////////////////////////////
//

#include "stdafx.h"
#include "inputEdit.h"
#include "resource.h"
#include <stdlib.h>


////////////////////////////////////////
//

BEGIN_MESSAGE_MAP(CInputEdit, CEdit)
	//{{AFX
		ON_WM_PAINT()
		ON_WM_LBUTTONDOWN()
		ON_WM_CHAR()
		ON_WM_ERASEBKGND()
	//}}AFX
END_MESSAGE_MAP()

///////////////////////////////////////////////////
//

void CInputEdit::OnPaint() {

	CEdit::OnPaint();
}


void CInputEdit::OnLButtonDown(UINT nFlags, CPoint point) {
	CEdit::OnLButtonDown(nFlags, point);

}



void CInputEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CEdit::OnChar(nChar, nRepCnt, nFlags);
		
}

BOOL CInputEdit::OnEraseBkgnd(CDC* pDC) {

	return CEdit::OnEraseBkgnd(pDC);
}