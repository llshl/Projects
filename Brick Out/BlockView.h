
// BlockView.h : CBlockView 클래스의 인터페이스
//

#pragma once


class CBlockView : public CView
{
protected: // serialization에서만 만들어집니다.
	CBlockView();
	DECLARE_DYNCREATE(CBlockView)

// 특성입니다.
public:
	CBlockDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CBlockView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int nX;
	int nY;
	int nMx;
	int nMy;
	CRect m_Rect;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CPoint m_ptStart;
	CPoint m_ptEnd;
	int block[4][10];
	CRect CBlock[4][10];
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnPaint();
	afx_msg void On32771();
	int Level;
	afx_msg void middle();
	afx_msg void High();
	int speed;
	afx_msg void On32774();
//	afx_msg void OnPaint();
	CBitmap  m_Bitmap;
};

#ifndef _DEBUG  // BlockView.cpp의 디버그 버전
inline CBlockDoc* CBlockView::GetDocument() const
   { return reinterpret_cast<CBlockDoc*>(m_pDocument); }
#endif

