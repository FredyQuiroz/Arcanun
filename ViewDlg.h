#pragma once
#include "afxwin.h"
#include "BitmapCtrl.h"

// Cuadro de diálogo de CViewDlg

class CViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~CViewDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX o DDV

	DECLARE_MESSAGE_MAP()
public:
	void		SetZoom();
	int			m_nZoom;
	CString		m_szOriginal;
	CString		m_szModificado;
	CString		m_szPixeles;
	afx_msg void OnBnClickedCpixelesm();
	afx_msg void OnBnClickedCpixeleso();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_BCerrar;
	CBitmapCtrl m_SOriginal;
	CBitmapCtrl m_SModificado;
	virtual BOOL OnInitDialog();
	CButton m_BShowO;
	CButton m_BShowM;
	CButton m_BAcercar;
	CButton m_EAlejar;
	afx_msg void OnBnClickedBalejar();
	CButton m_ERestablecer;
	afx_msg void OnBnClickedBrestablecer();
	CStatic m_EZoom;
};
