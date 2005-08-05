// arcanun.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "arcanun.h"
#include "arcanunDlg.h"
# include "SplashScreenEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CarcanunApp

BEGIN_MESSAGE_MAP(CarcanunApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Construcción de CarcanunApp

CarcanunApp::CarcanunApp()
{
	// TODO: agregar aquí el código de construcción,
	// Colocar toda la inicialización importante en InitInstance
}


// El único objeto CarcanunApp

CarcanunApp theApp;


// Inicialización de CarcanunApp

BOOL CarcanunApp::InitInstance()
{
	// Windows XP requiere InitCommonControls() si un manifiesto de 
	// aplicación especifica el uso de ComCtl32.dll versión 6 o posterior para habilitar
	// estilos visuales. De lo contrario, se generará un error al crear ventanas.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	SetRegistryKey("JuHU");


	CarcanunDlg dlg;
	m_pMainWnd = &dlg;

	CString szTemp= "m";
	CSplashScreenEx *pSplash=new CSplashScreenEx();

	pSplash->Create(m_pMainWnd,"",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW,false);
	pSplash->SetBitmap("logo.bmp",255,0,255);
	pSplash->SetTextFont("Impact",100,CSS_TEXT_BOLD);
	pSplash->SetTextRect(CRect(10,300,395,350));
	pSplash->SetTextColor(RGB(0,255,0));
	pSplash->SetTextFormat(DT_VCENTER | DT_LEFT);
	pSplash->Show();
	for( int i=0; i<35; i++) {
		pSplash->SetText(szTemp);
		szTemp+="m";
		Sleep(50);
	}
	pSplash->Hide();

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	return FALSE;
}
