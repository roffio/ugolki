
// TicTacToeTest.h: ãëàâíûé ôàéë çàãîëîâêà äëÿ ïðèëîæåíèÿ PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "âêëþ÷èòü pch.h äî âêëþ÷åíèÿ ýòîãî ôàéëà â PCH"
#endif

#include "resource.h"		// îñíîâíûå ñèìâîëû


// CTicTacToeTestApp:
// Ñâåäåíèÿ î ðåàëèçàöèè ýòîãî êëàññà: TicTacToeTest.cpp
//

class CTicTacToeTestApp : public CWinApp
{
public:
	CTicTacToeTestApp();

// Ïåðåîïðåäåëåíèå
public:
	virtual BOOL InitInstance();

// Ðåàëèçàöèÿ

	DECLARE_MESSAGE_MAP()
};

extern CTicTacToeTestApp theApp;
