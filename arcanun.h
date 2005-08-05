// arcanun.h: archivo de encabezado principal para la aplicación PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Símbolos principales


// CarcanunApp:
// Consulte la sección arcanun.cpp para obtener información sobre la implementación de esta clase
//

class CarcanunApp : public CWinApp
{
public:
	CarcanunApp();

// Reemplazos
	public:
	virtual BOOL InitInstance();

// Implementación

	DECLARE_MESSAGE_MAP()
};

extern CarcanunApp theApp;
