
// TicTacToeTestDlg.cpp: ôàéë ðåàëèçàöèè
//

#include "pch.h"
#include "framework.h"
#include "TicTacToeTest.h"
#include "TicTacToeTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTicTacToeTestDlg::CTicTacToeTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TICTACTOETEST_DIALOG, pParent)
	, mFieldSize(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//  nSelectedX = 0;
	//  nSelectedY = 0;
}

void CTicTacToeTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TICTACTOEFIELD, tttField);
	DDX_Text(pDX, IDC_EDIT_FIELDSIZE, mFieldSize);
	DDX_Text(pDX, IDC_EDIT_FIELDSIZE, mFieldSize);
}

BEGIN_MESSAGE_MAP(CTicTacToeTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CTicTacToeTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_FIELDSIZE, &CTicTacToeTestDlg::OnBnClickedButtonChangeFieldsize)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_FIELDS, &CTicTacToeTestDlg::OnBnClickedButtonClearFields)
END_MESSAGE_MAP()



BOOL CTicTacToeTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		


	return TRUE;  
}



void CTicTacToeTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CTicTacToeTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTicTacToeTestDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CTicTacToeTestDlg::OnBnClickedButtonChangeFieldsize()
{
	UpdateData(TRUE);
	int size = _ttoi(mFieldSize);
	tttField.SetFieldSize(size);
}


void CTicTacToeTestDlg::OnBnClickedButtonClearFields()
{
	tttField.ClearFields();
	this->Invalidate();
}
