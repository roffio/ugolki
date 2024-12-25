#pragma once
#include "CTicTacToeField.h"


class CTicTacToeTestDlg : public CDialogEx
{

public:
	CTicTacToeTestDlg(CWnd* pParent = nullptr);	// ñòàíäàðòíûé êîíñòðóêòîð


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TICTACTOETEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ïîääåðæêà DDX/DDV


protected:
	HICON m_hIcon;
	CTicTacToeField tttField;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonChangeFieldsize();
private:
	CString mFieldSize;
public:
	afx_msg void OnBnClickedButtonClearFields();
private:
//	int nSelectedX;
//	int nSelectedY;
};
