#pragma once


// CTicTacToeField

class CTicTacToeField : public CWnd
{
	DECLARE_DYNAMIC(CTicTacToeField)

public:
	CTicTacToeField();
	virtual ~CTicTacToeField();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void RegisterClass();
private:
	bool RegisterClass();
public:
	afx_msg void OnPaint();
private:
	int fieldSize;
public:
	// Установка размера игрового поля
	void SetFieldSize(int size);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	void GetFieldPosition(CPoint point, int& xpos, int& ypos);
	int** fields;
	void InitializeFields();
	void DestroyFields();
public:
	void ClearFields();
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
private:
	CRect GetRectFromField(int x, int y);
public:
	void DrawX(CPaintDC& dc, CRect rect);
	void DrawO(CPaintDC& dc, CRect rect);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	int nSelectedX;
	int nSelectedY;
	void HighlightSelection(CPaintDC& dc);
public:
	afx_msg void OnMouseLeave();
};


