// CTicTacToeField.cpp : implementation file
//

#include "pch.h"
#include "TicTacToeTest.h"
#include "CTicTacToeField.h"

#define TICTACTOEFIELD_CLASSNAME L"TicTacToeField"
#define FIELDNUMBERSPACE 20

// CTicTacToeField

IMPLEMENT_DYNAMIC(CTicTacToeField, CWnd)

CTicTacToeField::CTicTacToeField()
{

	fieldSize = 8;
	fields = nullptr;
	InitializeFields();
	this->RegisterClass();
	nSelectedX = -1;
	nSelectedY = -1;
}

CTicTacToeField::~CTicTacToeField()
{
}


BEGIN_MESSAGE_MAP(CTicTacToeField, CWnd)
//6	ON_COMMAND(AFX_ID_PREVIEW_CLOSE, &CTicTacToeField::RegisterClass)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_DESTROY()
ON_WM_RBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CTicTacToeField message handlers




//void CTicTacToeField::RegisterClass()
//{
	// TODO: Add your command handler code here
//}


bool CTicTacToeField::RegisterClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	
	if (!(::GetClassInfo(hInst, TICTACTOEFIELD_CLASSNAME, &wndcls)))
	{
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = TICTACTOEFIELD_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}


void CTicTacToeField::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect rect;
	GetClientRect(&rect);

	CPen ourPen;
	HGDIOBJ oldPen;
	ourPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	oldPen = dc.SelectObject(ourPen);

	dc.SelectObject(oldPen);
	ourPen.DeleteObject();

	int hPartSize = (rect.right - FIELDNUMBERSPACE) / fieldSize;
	int vPartSize = (rect.bottom - FIELDNUMBERSPACE) / fieldSize;

	this->HighlightSelection(dc);
	
	dc.SetTextColor(RGB(255, 0, 0));
	dc.SetTextAlign(TA_TOP | TA_LEFT);
	dc.SetBkMode(TRANSPARENT);
	//dc.TextOutW(0, 0, L"TEST TEXT");
	for (int i = 0; i < fieldSize; i++)
	{
		CString str;
		str.Format(L"%d", i);
		dc.TextOutW(0, FIELDNUMBERSPACE / 2 + vPartSize / 2 + i * vPartSize, str);
		dc.TextOutW(FIELDNUMBERSPACE / 2 + hPartSize / 2 + i * hPartSize, 0, str);
	}

	

	for (int i = 0; i < fieldSize + 1; i++)
	{
		dc.MoveTo(FIELDNUMBERSPACE + i * hPartSize, FIELDNUMBERSPACE);
		dc.LineTo(FIELDNUMBERSPACE + i * hPartSize, rect.bottom);
		dc.MoveTo(FIELDNUMBERSPACE, i * vPartSize + FIELDNUMBERSPACE);
		dc.LineTo(rect.right, i * vPartSize + FIELDNUMBERSPACE);
	}

	if (fields != nullptr)
	{
		for (int x = 0; x < fieldSize; x++)
			for (int y = 0; y < fieldSize; y++)
			{
				if (fields[y][x] == 1)
					this->DrawX(dc, this->GetRectFromField(x, y));
				if (fields[y][x] == 2)
					this->DrawO(dc, this->GetRectFromField(x, y));
			}
	}


}


// Установка размера игрового поля
void CTicTacToeField::SetFieldSize(int size)
{
	
}


void CTicTacToeField::OnLButtonDown(UINT nFlags, CPoint point)
{
	int x, y;

	if (point.x > FIELDNUMBERSPACE && point.y > FIELDNUMBERSPACE)
	{
		this->GetFieldPosition(point, x, y);
		if (fields != nullptr)
		{
			fields[y][x] = 1;
			this->Invalidate();
		}
	}

	CWnd::OnMButtonDown(nFlags, point);
}


void CTicTacToeField::GetFieldPosition(CPoint point, int& xpos, int& ypos)
{
	CRect rect;
	GetClientRect(&rect);
	int hPartSize = (rect.right - FIELDNUMBERSPACE) / fieldSize;
	int vPartSize = (rect.bottom - FIELDNUMBERSPACE) / fieldSize;
	for (int i = 0; i < fieldSize; i++)
	{
		if ((point.x > FIELDNUMBERSPACE + i * hPartSize) && (point.x < FIELDNUMBERSPACE * (i + 1) * hPartSize))
			xpos = i;
		if ((point.y > FIELDNUMBERSPACE + i * vPartSize) && (point.y < FIELDNUMBERSPACE * (i + 1) * vPartSize))
			ypos = i;
	}
}
void CTicTacToeField::InitializeFields()
{
	if (fields == nullptr)
	{
		fields = new int* [fieldSize];
		for (int i = 0; i < fieldSize; i++)
			fields[i] = new int[fieldSize];
	}

	ClearFields();
}

void CTicTacToeField::DestroyFields()
{
	if (fields != nullptr)
	{
		for (int i = 0; i < fieldSize; i++)
			delete[] fields[i];
		delete[] fields;
		fields = nullptr;
	}
}

void CTicTacToeField::ClearFields()
{
	if (fields != nullptr)
	{
		for (int row = 0; row < fieldSize; row++)
		{
			for (int col = 0; col < fieldSize; col++)
			{
				fields[row][col] = 0;
			}
		}
	}
}

void CTicTacToeField::OnDestroy()
{
	CWnd::OnDestroy();
	DestroyFields();
}


void CTicTacToeField::OnRButtonDown(UINT nFlags, CPoint point)
{
	int x, y;

	if (point.x > FIELDNUMBERSPACE && point.y > FIELDNUMBERSPACE)
	{
		this->GetFieldPosition(point, x, y);
		if (fields != nullptr)
		{
			fields[y][x] = 2;
			this->Invalidate();
		}
	}

	CWnd::OnMButtonDown(nFlags, point);
}


CRect CTicTacToeField::GetRectFromField(int x, int y)
{
	CRect rect;
	CRect rectWnd;
	GetClientRect(&rectWnd);
	int hPartSize = (rectWnd.right - FIELDNUMBERSPACE) / fieldSize;
	int vPartSize = (rectWnd.bottom - FIELDNUMBERSPACE) / fieldSize;

	rect.left = FIELDNUMBERSPACE + x * hPartSize + 1;
	rect.top = FIELDNUMBERSPACE + y * vPartSize + 1;
	rect.right = FIELDNUMBERSPACE + ((x + 1) * hPartSize) - 1;
	rect.bottom = FIELDNUMBERSPACE + ((y + 1) * vPartSize) - 1;

	return rect;
}


void CTicTacToeField::DrawX(CPaintDC& dc, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HGDIOBJ oldPen = dc.SelectObject(pen);
	HGDIOBJ oldBrush = dc.SelectObject(CreateSolidBrush(RGB(0,0,0)));

	dc.Ellipse(rect.left + 4, rect.top + 4, rect.right - 4, rect.bottom - 4);
	oldBrush = dc.SelectObject(CreateSolidBrush(RGB(150, 150, 150)));
	dc.Ellipse(rect.left + 6, rect.top + 6, rect.right - 6, rect.bottom - 6);
	oldBrush = dc.SelectObject(CreateSolidBrush(RGB(0, 0, 0)));
	dc.Ellipse(rect.left + 10, rect.top + 10, rect.right - 10, rect.bottom - 10);

	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);
	pen.DeleteObject();
}

void CTicTacToeField::DrawO(CPaintDC& dc, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HGDIOBJ oldPen = dc.SelectObject(pen);
	HGDIOBJ oldBrush = dc.SelectObject(GetStockObject(HOLLOW_BRUSH));

	dc.Ellipse(rect.left + 4, rect.top + 4, rect.right - 4, rect.bottom - 4);
	oldBrush = dc.SelectObject(CreateSolidBrush(RGB(200, 200, 200)));
	dc.Ellipse(rect.left + 5, rect.top + 5, rect.right - 5, rect.bottom - 5);
	dc.Ellipse(rect.left + 10, rect.top + 10, rect.right - 10, rect.bottom - 10);

	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);
	pen.DeleteObject();
}


void CTicTacToeField::OnMouseMove(UINT nFlags, CPoint point)
{
	int x, y;

	GetFieldPosition(point, x, y);
	if ((nSelectedX != x) || (nSelectedY != y))
	{
		nSelectedX = x;
		nSelectedY = y;
		this->Invalidate();
	}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = this->m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = HOVER_DEFAULT;

	TrackMouseEvent(&tme);

	CWnd::OnMouseMove(nFlags, point);
}


void CTicTacToeField::HighlightSelection(CPaintDC& dc)
{
	if ((nSelectedX < 0) || (nSelectedY < 0))
		return;

	CRect rect = GetRectFromField(nSelectedX, nSelectedY);
	CBrush brush;
	brush.CreateSolidBrush(RGB(125, 249, 255));
	HGDIOBJ oldBrush = dc.SelectObject(brush);
	dc.Rectangle(rect);
	dc.SelectObject(oldBrush);
	brush.DeleteObject();
}


void CTicTacToeField::OnMouseLeave()
{
	nSelectedX = -1;
	nSelectedY = -1;
	this->Invalidate();

	CWnd::OnMouseLeave();
}
