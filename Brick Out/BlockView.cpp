
// BlockView.cpp : CBlockView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CBlockView ����/�Ҹ�

CBlockView::CBlockView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	nX = 300;
	nY = 500;	
	nMx = nMy = STEP;
	Level = 100; // �������

	speed = 20;
	for (int i = 0; i < 4; i++) //��ϻ���
	{
		for (int j = 0; j < 10; j++)
		{
			//if (i <= 2)
				block[i][j] =  rand() % 2 +1 ;// ������ 1 or 2   2�� �ι��¾ƾ� ������
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CBlockView �׸���

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
			if (block[i][j] != 0) {                                //(i,j) ������ ���� ����
				if (block[i][j] == 1) {
					
					CBrush brush, *oldBrush;
					brush.CreateSolidBrush(RGB(252, 198,162));
					oldBrush = pDC->SelectObject(&brush);

					pDC->Rectangle(0 + j * 100, 0 + i * 75, 95 + j * 100, 70 + i * 75); // ���� :95 ����: 70 ���� :5 ����
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
					
					pDC->Rectangle(0 + j * 100, 0 + i * 75, 95 + j * 100, 70 + i * 75); // ���� :95 ����: 70 ���� :5
					pDC->SelectObject(oldBrush);
					brush.DeleteObject();

					CBlock[i][j].left = 0 + j * 100; //�������� ���� ����
					CBlock[i][j].top = 0 + i * 75;
					CBlock[i][j].right = 95 + j * 100;
					CBlock[i][j].bottom = 70 + i * 75;
					
				}
			}
		}

	}
	
	

	brush.CreateSolidBrush(RGB(136, 248, 157));
	oldBrush = pDC->SelectObject(&brush);

	pDC->Ellipse(nX - R, nY - R, nX + R, nY + R); //�� ����
	pDC->SelectObject(oldBrush);
	brush.DeleteObject();

	brush.CreateSolidBrush(RGB(164, 183, 220));
	oldBrush = pDC->SelectObject(&brush);
	
	pDC->Rectangle(m_ptStart.x - Level, 600, m_ptStart.x + Level, 615);//�� ����
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CBlockView �μ�

BOOL CBlockView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CBlockView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CBlockView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.

}


// CBlockView ����

#ifdef _DEBUG
void CBlockView::AssertValid() const
{
	CView::AssertValid();
}

void CBlockView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBlockDoc* CBlockView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBlockDoc)));
	return (CBlockDoc*)m_pDocument;
}
#endif //_DEBUG


// CBlockView �޽��� ó����


void CBlockView::OnTimer(UINT_PTR nIDEvent)
{
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nY + R <= m_Rect.bottom) // ���� ȭ�� �Ʒ��� �ȳ��� ���� ����
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
	

	// �ٿ� �ε�ġ�� �������� Ƣ���
	if (nY +R >= 600 && nX <=m_ptStart.x + Level && nX  >=m_ptStart.x - Level)
	{
		nMy = -nMy;
	}
	// �����ʿ� �ε�ġ�� �������� Ƣ���
	if (nX + R >= m_Rect.right)
	{
		nMx = -STEP;
	}

	// ���ʿ� �ε�ġ�� �Ʒ��ʿ� Ƣ���
	if (nY - R <= m_Rect.top) 
	{
		nMy = STEP;
	}
	// ���ʿ� �ε�ġ�� ���������� Ƣ���
	if (nX - R <= m_Rect.left) 
	{
		nMx = STEP;
	}
	// ȭ�� �Ʒ��� �������� ����
	if (nY + R >= m_Rect.bottom)
	{
		 AfxMessageBox(_T("You die!"));	 
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
		if (nY - R <= CBlock[i][j].bottom &&nY >= CBlock[i][j].bottom&& nY <= CBlock[i][j].bottom + 100 && (nX) <= CBlock[i][j].right && (nX) >= CBlock[i][j].left) //��� �Ʒ��� �浹
			//CBlock[nY / (75 - 5 * j) - 1][(nX / (95 + 5 * j))]== CBlock[i][j]&& (nY-R)<=CBlock[i][j].bottom
		{
			nMy = STEP; 
			block[i][j] = block[i][j]-1; //��� ���ֱ�
			if (block[i][j] == 0) {
				CBlock[i][j].bottom = 0;
				CBlock[i][j].left = 0;
				CBlock[i][j].right = 0;
				CBlock[i][j].top = 0;
			}
		}
		if (nY <= CBlock[i][j].bottom && nY >= CBlock[i][j].top && (nX) > CBlock[i][j].left - 100 &&(nX) <= CBlock[i][j].left && (nX + R) >= CBlock[i][j].left) //��� ���� ��
		{
			nMx = -STEP;
			block[i][j] = block[i][j] - 1; //��� ���ֱ�
			if (block[i][j] == 0) {
				CBlock[i][j].bottom = 0;
				CBlock[i][j].left = 0;
				CBlock[i][j].right = 0;
				CBlock[i][j].top = 0;
			}
		}
		
		if (nY <= CBlock[i][j].bottom && nY >= CBlock[i][j].top && (nX) >= CBlock[i][j].right-STEP && (nX) <= CBlock[i][j].right+100 && (nX -R) <= CBlock[i][j].right   ) //��� ���� ��
		{
			nMx = STEP;
			block[i][j] = block[i][j] - 1; //��� ���ֱ�
			if (block[i][j] == 0) {
				CBlock[i][j].bottom = 0;
				CBlock[i][j].left = 0;
				CBlock[i][j].right = 0;
				CBlock[i][j].top = 0;
			}
		}
		/*if ((nY + R) >= CBlock[i][j].top && nX <= CBlock[i][j].right && nX >= CBlock[i][j].left && nY <= CBlock[i][j].top  && nY >= CBlock[i][j].top + 75) //��� ����
		{
			nMy = -STEP;
			block[i][j] = 0;//��� ���ֱ�
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//��Ŭ�� �ϸ� ����
	SetTimer(1, speed, NULL);
	
	CView::OnLButtonDown(nFlags, point);
	
	
}





void CBlockView::OnDestroy()
{
	CView::OnDestroy();
	KillTimer(1);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CBlockView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	GetClientRect(&m_Rect);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CBlockView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	m_ptStart = point;
	
	Invalidate();
	CView::OnMouseMove(nFlags, point);
}


int CBlockView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	return 0;
}



void CBlockView::On32771() //easy
{
	Level = 100;
	speed = 20;
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CBlockView::middle() //����
{
	Level = 50;
	speed = 15;
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CBlockView::High() //�����
{
	Level = 30;
	speed = 8;
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CBlockView::On32774() //�����
{
	nMy = -STEP;
	nMx = STEP;
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

