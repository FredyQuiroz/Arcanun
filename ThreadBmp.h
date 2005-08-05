#pragma once
# include "workerthread.h"


class CThreadBmp
{
	CEvent			m_Event;
	static UINT		EventProc(LPVOID pProcParam);
	bool			Init();
	bool			m_bRunning;
	void			Kill();
	int				m_nMetodo;
	int				m_nAlgoritmo;
	CString			m_szDestino;
	CString			m_szBitmap;
	CString			m_szAdd;
	CString			m_szEcuacion;
	CString			m_szOriginal;
	int				m_nOffset;
	bool			m_bPar;
	CString			m_szContrasena;
	CWnd			*m_pParent;
	bool			m_bShow;
	int				m_nFase;
	bool			m_bLBT;
public:
	CThreadBmp(CWnd *parent=NULL);
	~CThreadBmp(void);
	CWorkerThread	m_Thread;
	bool			IsRunning();
	void			Run();
	void			SetArchivos(CString bitmap, CString destino, CString add="");
	void			SetEcuacion(CString ecuacion);
	void			SetParImpar(bool par, int offset);
	void			SetPassword(CString password);
	void			SetMetodo(int metodo);
	void			SetOriginalBitmap(CString bitmap);
	void			SetShowPixeles(bool show);
	void			SetLBT(bool use);
};
