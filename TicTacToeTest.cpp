
#include "pch.h"
#include "framework.h"
#include "TicTacToeTest.h"
#include "TicTacToeTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTicTacToeTestApp

BEGIN_MESSAGE_MAP(CTicTacToeTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ñîçäàíèå CTicTacToeTestApp

CTicTacToeTestApp::CTicTacToeTestApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}

CTicTacToeTestApp theApp;

BOOL CTicTacToeTestApp::InitInstance()
{

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();


	CShellManager *pShellManager = new CShellManager;


	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));


	SetRegistryKey(_T("Ëîêàëüíûå ïðèëîæåíèÿ, ñîçäàííûå ñ ïîìîùüþ ìàñòåðà ïðèëîæåíèé"));

	CTicTacToeTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		
	}
	else if (nResponse == IDCANCEL)
	{
		
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Предупреждение. Не удалось создать диалоговое окно, поэтому работа приложения неожиданно завершена .\n");
		TRACE(traceAppMsg, 0, "Предупреждение.При использовании элементов управления MFC для диалогового окна невозможно #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return FALSE;
}

