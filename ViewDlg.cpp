// ViewDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "arcanun.h"
#include "ViewDlg.h"


// Cuadro de diálogo de CViewDlg

IMPLEMENT_DYNAMIC(CViewDlg, CDialog)
CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewDlg::IDD, pParent)
{
	m_nZoom=100;
}

CViewDlg::~CViewDlg()
{
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BCerrar);
	DDX_Control(pDX, IDC_CPIXELESO, m_BShowO);
	DDX_Control(pDX, IDC_CPIXELESM, m_BShowM);
	DDX_Control(pDX, IDOK, m_BAcercar);
	DDX_Control(pDX, IDC_BALEJAR, m_EAlejar);
	DDX_Control(pDX, IDC_BRESTABLECER, m_ERestablecer);
	DDX_Control(pDX, IDC_SZOOM, m_EZoom);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialog)
	ON_BN_CLICKED(IDC_CPIXELESM, OnBnClickedCpixelesm)
	ON_BN_CLICKED(IDC_CPIXELESO, OnBnClickedCpixeleso)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BALEJAR, OnBnClickedBalejar)
	ON_BN_CLICKED(IDC_BRESTABLECER, OnBnClickedBrestablecer)
END_MESSAGE_MAP()


// Controladores de mensajes de CViewDlg

void CViewDlg::OnBnClickedCpixelesm()
{
	CWaitCursor wait;
	m_SModificado.LoadFile( m_BShowM.GetCheck() ? m_szPixeles : m_szModificado);
	m_SModificado.Resize(m_nZoom);
}

void CViewDlg::OnBnClickedCpixeleso()
{
	CWaitCursor wait;
	m_SOriginal.LoadFile( m_BShowO.GetCheck() ? m_szPixeles : m_szOriginal);
	m_SOriginal.Resize(m_nZoom);
}

void CViewDlg::OnBnClickedOk()
{
	m_nZoom+= m_nZoom > 100 ? 50 : 20;
	SetZoom();
}

void CViewDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if( ::IsWindow(m_SOriginal)) {
		m_SOriginal.MoveWindow(5,5, cx/2-5, cy-50);
		m_SModificado.MoveWindow(cx/2,5, cx/2-10, cy-50);
		CRect rect;
		m_BCerrar.GetWindowRect(rect);
		m_BCerrar.MoveWindow(cx-rect.Width()-5, cy-5-rect.Height(), rect.Width(), rect.Height());
		m_BAcercar.GetWindowRect(rect);
		m_BAcercar.MoveWindow(cx-2*rect.Width()-10, cy-5-rect.Height(), rect.Width(), rect.Height());
		m_EAlejar.GetWindowRect(rect);
		m_EAlejar.MoveWindow(cx-3*rect.Width()-15, cy-5-rect.Height(), rect.Width(), rect.Height());
		m_ERestablecer.GetWindowRect(rect);
		m_ERestablecer.MoveWindow(cx-4*rect.Width()-20, cy-5-rect.Height(), rect.Width(), rect.Height());
		m_EZoom.GetWindowRect(rect);
		m_EZoom.MoveWindow(5, cy-5-rect.Height(), rect.Width(), rect.Height());
		m_BShowM.GetWindowRect(rect);
		m_BShowM.MoveWindow(cx/2, cy-40,rect.Width(), rect.Height());
		m_BShowO.GetWindowRect(rect);
		m_BShowO.MoveWindow(5, cy-40, rect.Width(), rect.Height());
		m_SModificado.SetBitmap(m_SModificado.hBitmap);
		m_SOriginal.SetBitmap(m_SOriginal.hBitmap);
	}
}

BOOL CViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CRect brect;
	GetDlgItem(IDC_SORIGINAL)->GetWindowRect(brect);
	ScreenToClient(brect);
	m_SOriginal.Create(WS_CHILD|WS_VISIBLE,brect,this);

	GetDlgItem(IDC_SMODIFICADO)->GetWindowRect(brect);
	ScreenToClient(brect);
	m_SModificado.Create(WS_CHILD|WS_VISIBLE,brect,this);

	m_SOriginal.LoadFile(m_szOriginal);
	m_SModificado.LoadFile(m_szModificado);
	m_ERestablecer.EnableWindow(false);

	SetWindowText("Visor de Bitmaps - Original - Modificado [100%]");
	return TRUE;
}

void CViewDlg::SetZoom() {

	CWaitCursor wait;
	CString temporal;
	temporal.Format("%d%%", m_nZoom);
	m_EZoom.SetWindowText(temporal);
	temporal.Format("Visor de Bitmaps - Original - Modificado [%d%%]", m_nZoom);
	SetWindowText(temporal);
	m_ERestablecer.EnableWindow(m_nZoom!=100);
	m_EAlejar.EnableWindow(m_nZoom>20);
	m_BAcercar.EnableWindow(m_nZoom<5000);
	m_SOriginal.Resize(m_nZoom);
	m_SModificado.Resize(m_nZoom);
}

void CViewDlg::OnBnClickedBalejar()
{
	m_nZoom-= m_nZoom>100 ? 50 : 20;
	SetZoom();
}

void CViewDlg::OnBnClickedBrestablecer()
{
	m_nZoom= 100;
	SetZoom();
}
