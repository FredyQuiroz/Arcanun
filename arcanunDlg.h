// arcanunDlg.h: archivo de encabezado
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include "bitmappicture.h"
# include "separador/checkboxseparator.h"
# include "separador/sectionseparator.h"
# include "threadbmp.h"
# include "HyperLink.h"


CString GetAppPath(const char *archivo);

class CarcanunDlg : public CDialog
{

public:
	CarcanunDlg(CWnd* pParent = NULL);	// Constructor estándar

// Datos del cuadro de diálogo
	enum { IDD = IDD_ARCANUN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON		m_hIcon;
	CString		m_szFileHelp;
	CWnd		*m_pOldFocus;
	void		SetHelp(CWnd *item, CString &texto, CString &ayuda);
	bool FillLista(CString bitmap);

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString	m_szBitmap;
	CString	m_szDestino;
	CEdit m_EArchivo;
	CButton m_Explorar;
	afx_msg void OnBnClickedBexplorar();
	CProgressCtrl m_EProgreso;
	CButton m_EMetodo;
	afx_msg void OnBnClickedRlbt();
	afx_msg void OnBnClickedRsimple();
	CEdit m_EContrasena;
	CEdit m_EECuacion;
	CEdit m_EOffset;
	CButton m_BPar;
	afx_msg void OnBnClickedCpar();
	afx_msg void OnEnChangeEcoffset();
	afx_msg void OnEnChangeEcuacion();
	CSectionSeparator m_EAleatorio;
	CSectionSeparator m_SGeometrico;
	CSectionSeparator m_EPar;
	CSeparator m_SFin;
	int		m_nMetodo;
	CBitmapPicture	m_EBitmap;
	CStatic m_SEcuacion;
	afx_msg void OnStnClickedSaleatorio();
	afx_msg void OnStnClickedSgeometrico();
	afx_msg void OnStnClickedSpar();
	CListCtrl m_LCDetalles;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBextraer();
	afx_msg void OnBnClickedCancel();
	int		m_nAlgoritmo;
	int		m_nDisponibles;
	int		m_nDisponiblesLBT;
	CButton		m_BRSimple;
	CThreadBmp *m_pThreadBitmap;
	afx_msg void OnDestroy();
	LRESULT OnBitmapMessage(WPARAM wParam, LPARAM lParam);
	CButton m_BShow;
	afx_msg void OnBnClickedCshow();
	bool	m_bExtraer;
	CHyperLink m_SAyuda;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuAyuda();
	afx_msg void OnTimer(UINT nIDEvent);
};
