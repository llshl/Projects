
// BlockView.cpp : CBlockView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Block.h"
#endif

#include "BlockDoc.h"
#include "BlockView.h"
#define STEP 10
#define R 30
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBlockView

IMPLEMENT_DYNCREATE(CBlockView, CView)

BEGIN_MESSAGE_MAP(CBlockView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
//	ON_WM_RBUTTONDOWN()
//ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_SIZE()
ON_WM_MOUSEMOVE()
ON_WM_CREATE()
//ON_WM_PAINT()
ON_COMMAND(ID_32771, &CBlockView::On32771)
ON_COMMAND(ID_32772, &CBlockView::middle)
ON_COMMAND(ID_32773, &CBlockView::High)
ON_COMMAND(ID_32774, &CBlockView::On32774)
//ON_WM_PAINT()
END_MESSAGE_MAP()

// CBlockView 생성/소멸

CBlockView::CBlockView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	nX = 300;
	nY = 500;	
	nMx = nMy = STEP;
	Level = 100; // 막대길이

	speed = 20;
	for (int i = 0; i < 4; i++) //블록생성
	{
		for (int j = 0; j < 10; j++)
		{
			//if (i <= 2)
				block[i][j] =  rand() % 2 +1 ;// 랜덤값 1 or 2   2면 두번맞아야 없어짐
			//else
				//block[i][j] = 2;
		}

	}
}

CBlockView::~CBlockView()
{
}

BOOL CBlockView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CBlockView 그리기

void CBlockView::OnDraw(CDC* pDC)
{
	CBlockDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CBrush brush, *oldBrush;
	
	
	//pDC->Ellipse(m_ptStart.x-30,nY-60,m_ptStart.x+30,nY);
	//pDC->Ellipse(nX -R , nY-60 , nX + R, nY );
	
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (block[i][j] != 0) {                                //(i,j) 번지의 벽돌 생성
				if (block[i][j] == 1) {
					
					CBrush brush, *oldBrush;
					brush.CreateSolidBrush(RGB(252, 198,162));
					oldBrush = pDC->SelectObject(&brush);

					pDC->Rectangle(0 + j * 100, 0 + i * 75, 95 + j * 100, 70 + i * 75); // 가로 :95 세로: 70 공백 :5 벽돌
					pDC->SelectObject(oldBrush);
					brush.DeleteObject();

					CBlock[i][j].left = 0 + j * 100;
					CBlock[i][j].top = 0 + i * 75;
					CBlock[i][j].right = 95 + j * 100;
					CBlock[i][j].bottom = 70 + i * 75;
				}
				else if(block[i][j]==2)
				{
					
					CBrush brush, *oldBrush;
					brush.CreateSolidBrush(RGB(255,111, 97));
					oldBrush = pDC->SelectObject(&brush);
					
					pDC->Rectangle(0 + j * 100, 0 + i * 75, 95 + j * 100, 70 + i * 75); // 가로 :95 세로: 70 공백 :5
					pDC->SelectObject(oldBrush);
					brush.DeleteObject();

					CBlock[i][j].left = 0 + j * 100; //생성벽돌 정보 저장
					CBlock[i][j].top = 0 + i * 75;
					CBlock[i][j].right = 95 + j * 100;
					CBlock[i][j].bottom = 70 + i * 75;
					
				}
			}
		}

	}
	
	

	brush.CreateSolidBrush(RGB(136, 248, 157));
	oldBrush = pDC->SelectObject(&brush);

	pDC->Ellipse(nX - R, nY - R, nX + R, nY + R); //공 생성
	pDC->SelectObject(oldBrush);
	brush.DeleteObject();

	brush.CreateSolidBrush(RGB(164, 183, 220));
	oldBrush = pDC->SelectObject(&brush);
	
	pDC->Rectangle(m_ptStart.x - Level, 600, m_ptStart.x + Level, 615);//바 생성
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CBlockView 인쇄

BOOL CBlockView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CBlockView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CBlockView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.

}


// CBlockView 진단

#ifdef _DEBUG
void CBlockView::AssertValid() const
{
	CView::AssertValid();
}

void CBlockView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBlockDoc* CBlockView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBlockDoc)));
	return (CBlockDoc*)m_pDocument;
}
#endif //_DEBUG


// CBlockView 메시지 처리기


void CBlockView::OnTimer(UINT_PTR nIDEvent)
{
	
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nY + R <= m_Rect.bottom) // 공이 화면 아래로 안내려 가면 진행
	{

		nX += nMx;
		nY = nY + nMy;
	}
	else
	{
		nX = 600;
		nY = 500;
		nMy = 0;
		nMx = 0;
	}
	

	// 바에 부딪치면 위쪽으로 튀기게
	if (nY +R >= 600 && nX <=m_ptStart.x + Level && nX  >=m_ptStart.x - Level)
	{
		nMy = -nMy;
	}
	// 오른쪽에 부딪치면 왼쪽으로 튀기게
	if (nX + R >= m_Rect.right)
	{
		nMx = -STEP;
	}

	// 위쪽에 부딪치면 아래쪽에 튀기게
	if (nY - R <= m_Rect.top) 
	{
		nMy = STEP;
	}
	// 왼쪽에 부딪치며 오른쪽으로 튀기게
	if (nX - R <= m_Rect.left) 
	{
		nMx = STEP;
	}
	// 화면 아래로 내려가면 종료
	if (nY + R >= m_Rect.bottom)
	{
		 AfxMessageBox(_T("You die!"));	 
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
		if (nY - R <= CBlock[i][j].bottom &&nY >= CBlock[i][j].bottom&& nY <= CBlock[i][j].bottom + 100 && (nX) <= CBlock[i][j].right && (nX) >= CBlock[i][j].left) //블록 아랫면 충돌
			//CBlock[nY / (75 - 5 * j) - 1][(nX / (95 + 5 * j))]== CBlock[i][j]&& (nY-R)<=CBlock[i][j].bottom
		{
			nMy = STEP; 
			block[i][j] = block[i][j]-1; //블록 없애기
			if (block[i][j] == 0) {
				CBlock[i][j].bottom = 0;
				CBlock[i][j].left = 0;
				CBlock[i][j].right = 0;
				CBlock[i][j].top = 0;
			}
		}
		if (nY <= CBlock[i][j].bottom && nY >= CBlock[i][j].top && (nX) > CBlock[i][j].left - 100 &&(nX) <= CBlock[i][j].left && (nX + R) >= CBlock[i][j].left) //블록 왼쪽 면
		{
			nMx = -STEP;
			block[i][j] = block[i][j] - 1; //블록 없애기
			if (block[i][j] == 0) {
				CBlock[i][j].bottom = 0;
				CBlock[i][j].left = 0;
				CBlock[i][j].right = 0;
				CBlock[i][j].top = 0;
			}
		}
		
		if (nY <= CBlock[i][j].bottom && nY >= CBlock[i][j].top && (nX) >= CBlock[i][j].right-STEP && (nX) <= CBlock[i][j].right+100 && (nX -R) <= CBlock[i][j].right   ) //블록 오른 면
		{
			nMx = STEP;
			block[i][j] = block[i][j] - 1; //블록 없애기
			if (block[i][j] == 0) {
				CBlock[i][j].bottom = 0;
				CBlock[i][j].left = 0;
				CBlock[i][j].right = 0;
				CBlock[i][j].top = 0;
			}
		}
		/*if ((nY + R) >= CBlock[i][j].top && nX <= CBlock[i][j].right && nX >= CBlock[i][j].left && nY <= CBlock[i][j].top  && nY >= CBlock[i][j].top + 75) //블록 윗면
		{
			nMy = -STEP;
			block[i][j] = 0;//블록 없애기
			CBlock[i][j].bottom = 0;
			CBlock[i][j].left = 0;
			CBlock[i][j].right = 0;
			CBlock[i][j].top = 0;
		}*/
		}
	}
	//KillTimer(1);
	

	Invalidate();
	//SetTimer(1, speed, NULL);
	CView::OnTimer(nIDEvent);
}


void CBlockView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//좌클릭 하면 시작
	SetTimer(1, speed, NULL);
	
	CView::OnLButtonDown(nFlags, point);
	
	
}





void CBlockView::OnDestroy()
{
	CView::OnDestroy();
	KillTimer(1);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CBlockView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	GetClientRect(&m_Rect);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CBlockView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	m_ptStart = point;
	
	Invalidate();
	CView::OnMouseMove(nFlags, point);
}


int CBlockView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	return 0;
}



void CBlockView::On32771() //easy
{
	Level = 100;
	speed = 20;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CBlockView::middle() //보통
{
	Level = 50;
	speed = 15;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CBlockView::High() //어려움
{
	Level = 30;
	speed = 8;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CBlockView::On32774() //재시작
{
	nMy = -STEP;
	nMx = STEP;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

