#include "StdAfx.h"
#include "threadbmp.h"
# include "bmphide.h"

CThreadBmp::CThreadBmp(CWnd *parent)
{
	m_bRunning= false;
	m_nMetodo= 0;
	m_nAlgoritmo= 0;
	m_nOffset =0;
	m_bPar= false;
	m_pParent= parent;
	m_bShow= false;
	m_nFase= 0;
	m_bLBT= true;
}

CThreadBmp::~CThreadBmp(void)
{
	m_Event.ResetEvent();
	m_Thread.Kill();
}

UINT CThreadBmp::EventProc(LPVOID pProcParam) {

	CThreadBmp* pClass= reinterpret_cast<CThreadBmp*>(pProcParam);
	pClass->Init();
	return 0;
}

void CThreadBmp::Run() {

	m_bRunning= true;
	m_Thread.AddEvent(m_Event, EventProc, this);
	m_Thread.Start();
	m_Event.SetEvent();
}

void CThreadBmp::Kill() {

	if(!m_bRunning) 
		return;
	m_bRunning= false;
}

bool CThreadBmp::IsRunning() {

	return m_bRunning;
}

bool CThreadBmp::Init() {

	ASSERT(m_pParent);
	ASSERT(m_nAlgoritmo);
	ASSERT(!m_szBitmap.IsEmpty());
	CBmpHide bmphide;
	bmphide.SetParent(m_pParent);
	bmphide.SetLBT(m_bLBT);
	bool retorno=false;

	if( m_szAdd.IsEmpty()) { // extraer

		m_bShow=false;
		ASSERT(!m_szOriginal.IsEmpty());
		if(!bmphide.SetBitmap(m_szOriginal))
			return false;
		switch( m_nAlgoritmo) {
			case 1:
				bmphide.SetPassword(m_szContrasena);
				bmphide.SetTipo(1);
				break;
			case 2:
				bmphide.SetParametrosTipo2(m_nOffset, m_bPar);
				bmphide.SetTipo(2);
				break;
			case 3:
				bmphide.SetEcuacion(m_szEcuacion);
				bmphide.SetTipo(3);
				break;
		}
		retorno= bmphide.ExtractFile(m_szBitmap, m_szDestino);
	} else { // agregar

		if(!bmphide.SetBitmap(m_szBitmap))
			return false;
		bmphide.SetMarcar(m_nFase==1);
		switch( m_nAlgoritmo) {
			case 1:
				bmphide.SetPassword(m_szContrasena);
				bmphide.SetTipo(1);
				break;
			case 2:
				bmphide.SetParametrosTipo2(m_nOffset, m_bPar);
				bmphide.SetTipo(2);
				break;
			case 3:
				bmphide.SetEcuacion(m_szEcuacion);
				bmphide.SetTipo(3);
				break;
		}
		retorno = bmphide.AddFile(m_szAdd) && bmphide.WriteFile(m_szDestino);
	}
	if( m_bShow && retorno) {
		m_bShow= false;
		m_nFase=1;
		char temporal[MAX_PATH];
		GetTempPath( MAX_PATH, temporal);
		m_szDestino= temporal;
		m_szDestino+= "temporal.bmp";
		return Init();
	} else if( m_pParent && ::IsWindow(*m_pParent)) 
		m_pParent->SendMessage(WM_BITMAP, 2, retorno);
	return retorno;
}

void CThreadBmp::SetArchivos(CString bitmap, CString destino, CString add) {

	m_szBitmap= bitmap;
	m_szDestino= destino;
	m_szAdd= add;
}

void CThreadBmp::SetEcuacion(CString ecuacion) {

	m_nAlgoritmo= 3;
	m_szEcuacion= ecuacion;
}

void CThreadBmp::SetParImpar(bool par, int offset) {

	m_nAlgoritmo= 2;
	m_bPar= par;
	m_nOffset= offset;
}

void CThreadBmp::SetOriginalBitmap(CString bitmap) {

	m_szOriginal= bitmap;
}

void CThreadBmp::SetPassword(CString password) {

	m_szContrasena= password;
	m_nAlgoritmo= 1;
	
}

void CThreadBmp::SetMetodo(int metodo) {

	m_nMetodo= metodo;
}

void CThreadBmp::SetShowPixeles(bool show) {

	m_bShow= show;
}

void CThreadBmp::SetLBT(bool use) {

	m_bLBT= use;
}
