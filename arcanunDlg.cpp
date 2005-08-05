// arcanunDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "arcanun.h"
#include "arcanunDlg.h"
# include "parser.h"
# include "bmphide.h"
# include "viewdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cuadro de diálogo de CarcanunDlg



CarcanunDlg::CarcanunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CarcanunDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nMetodo=0;
	m_nAlgoritmo=0;
	m_nDisponibles=0;
	m_nDisponiblesLBT=0;
	m_pThreadBitmap= NULL;
	m_bExtraer= false;
}

void CarcanunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EARCHIVO, m_EArchivo);
	DDX_Control(pDX, IDC_BEXPLORAR, m_Explorar);
	DDX_Control(pDX, IDC_PROGRESO, m_EProgreso);
	DDX_Control(pDX, IDC_RLBT, m_EMetodo);
	DDX_Control(pDX, IDC_ECONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDC_ECUACION, m_EECuacion);
	DDX_Control(pDX, IDC_ECOFFSET, m_EOffset);
	DDX_Control(pDX, IDC_CPAR, m_BPar);
	DDX_Control(pDX, IDC_SALEATORIO, m_EAleatorio);
	DDX_Control(pDX, IDC_SGEOMETRICO, m_SGeometrico);
	DDX_Control(pDX, IDC_SPAR, m_EPar);
	DDX_Control(pDX, IDC_SFIN, m_SFin);
	DDX_Control(pDX, IDC_SBITMAP, m_EBitmap);
	DDX_Control(pDX, IDC_SECUACION, m_SEcuacion);
	DDX_Control(pDX, IDC_LDETALLES, m_LCDetalles);
	DDX_Control(pDX, IDC_RSIMPLE, m_BRSimple);
	DDX_Control(pDX, IDC_CSHOW, m_BShow);
}

BEGIN_MESSAGE_MAP(CarcanunDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BEXPLORAR, OnBnClickedBexplorar)
	ON_BN_CLICKED(IDC_RLBT, OnBnClickedRlbt)
	ON_BN_CLICKED(IDC_RSIMPLE, OnBnClickedRsimple)
	ON_BN_CLICKED(IDC_CPAR, OnBnClickedCpar)
	ON_EN_CHANGE(IDC_ECOFFSET, OnEnChangeEcoffset)
	ON_EN_CHANGE(IDC_ECUACION, OnEnChangeEcuacion)
	ON_STN_CLICKED(IDC_SALEATORIO, OnStnClickedSaleatorio)
	ON_STN_CLICKED(IDC_SGEOMETRICO, OnStnClickedSgeometrico)
	ON_STN_CLICKED(IDC_SPAR, OnStnClickedSpar)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(ID_BEXTRAER, OnBnClickedBextraer)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BITMAP, OnBitmapMessage)
	ON_BN_CLICKED(IDC_CSHOW, OnBnClickedCshow)
END_MESSAGE_MAP()

BOOL CarcanunDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_EAleatorio.SetNextSectionCtrl(m_SGeometrico);
	m_SGeometrico.SetNextSectionCtrl(m_EPar);
	m_EPar.SetNextSectionCtrl(m_SFin);
	m_EProgreso.SetRange(0,100);
	
	switch(AfxGetApp()->GetProfileInt("archivo","algoritmo", m_nAlgoritmo)) {
		case 1:
			OnStnClickedSaleatorio();
			break;
		case 2:
			OnStnClickedSgeometrico();
			break;
		case 3:
			OnStnClickedSpar();
			break;
	}

	if((m_nMetodo= AfxGetApp()->GetProfileInt("archivo", "metodo",1))==1) 
		m_EMetodo.SetCheck(1);
	else 
		m_BRSimple.SetCheck(1);

	m_LCDetalles.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LCDetalles.InsertColumn(0, "Propiedad", LVCFMT_LEFT, 100);
	m_LCDetalles.InsertColumn(1, "Valor", LVCFMT_LEFT, 100);
	m_LCDetalles.InsertItem(0, "x");
	m_LCDetalles.InsertItem(1, "y");
	m_LCDetalles.InsertItem(2, "pixeles");
	m_LCDetalles.InsertItem(3, "disponibles LSB");
	m_LCDetalles.InsertItem(4, "disponibles LBT");

	m_szBitmap= AfxGetApp()->GetProfileString("archivo", "archivo");
	m_EArchivo.SetWindowText(m_szBitmap);
	m_EBitmap.SetBitmap(m_szBitmap);
	FillLista(m_szBitmap);

	m_EECuacion.SetWindowText(AfxGetApp()->GetProfileString("archivo", "ecuacion"));
	m_EContrasena.SetWindowText(AfxGetApp()->GetProfileString("archivo", "password"));
	m_EOffset.SetWindowText(AfxGetApp()->GetProfileString("archivo", "offset"));
	m_BPar.SetCheck(AfxGetApp()->GetProfileInt("archivos", "par", 1));
	m_BShow.SetCheck(AfxGetApp()->GetProfileInt("archivos", "show", 1));
	OnBnClickedCpar();
	return TRUE;
}

void CarcanunDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CarcanunDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

HCURSOR CarcanunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CarcanunDlg::FillLista(CString bitmap) {

	CBmpContenido bmpcontenido;
	if( !bitmap.IsEmpty() && bmpcontenido.SetBitmap(bitmap)) {

		m_nDisponibles = bmpcontenido.GetPixeles()/8;
		CSize tamano= bmpcontenido.GetBitmaptamano();
		m_nDisponiblesLBT= bmpcontenido.GetBitsDisponibles()/8;

		CString temporal;
		temporal.Format("%d", tamano.cx);
		m_LCDetalles.SetItemText(0, 1, temporal);
		temporal.Format("%d", tamano.cy);
		m_LCDetalles.SetItemText(1, 1, temporal);
		temporal.Format("%d", bmpcontenido.GetPixeles());
		m_LCDetalles.SetItemText(2, 1, temporal);
		temporal.Format("%d", m_nDisponibles);
		m_LCDetalles.SetItemText(3, 1, temporal);
		temporal.Format("%d", m_nDisponiblesLBT);
		m_LCDetalles.SetItemText(4, 1, temporal);
		return true;
	}
	return false;
}

void CarcanunDlg::OnBnClickedBexplorar()
{
	CFileDialog dialogo(true, "bmp", "", NULL, "Archivos de Imagen (*.bmp)|*.bmp|Todos los archivos (*.*)|*.*||", this);
	dialogo.m_pOFN->lpstrTitle= "Abrir Bitmap Original";
	if( dialogo.DoModal()==IDOK) {
		
		m_szBitmap= dialogo.GetPathName();
		m_EArchivo.SetWindowText(m_szBitmap);
		AfxGetApp()->WriteProfileString("archivo", "archivo", m_szBitmap);
		m_EBitmap.SetBitmap(m_szBitmap);
		m_EBitmap.Invalidate();
		FillLista(m_szBitmap);
	}
}

void CarcanunDlg::OnBnClickedRlbt()
{
	m_nMetodo=1;
	AfxGetApp()->WriteProfileInt("archivo", "metodo", m_nMetodo);
}

void CarcanunDlg::OnBnClickedRsimple()
{
	m_nMetodo=0;
	AfxGetApp()->WriteProfileInt("archivo", "metodo", m_nMetodo);
}

void CarcanunDlg::OnBnClickedCpar()
{
	m_BPar.SetWindowText(m_BPar.GetCheck() ? "Par" : "Impar");
}

void CarcanunDlg::OnEnChangeEcoffset()
{
	CString offset;
	m_EOffset.GetWindowText(offset);
	if( atol(offset)>= m_nDisponiblesLBT) {
		offset.Format("%d", m_nDisponiblesLBT-1);
		m_EOffset.SetWindowText(offset);
	}
}

void CarcanunDlg::OnEnChangeEcuacion()
{
	CString ecuacion;
	m_EECuacion.GetWindowText(ecuacion);
	CParser parser;
	parser.SetEcuacion(ecuacion);
	m_SEcuacion.SetWindowText(parser.GetStringError());
}

void CarcanunDlg::OnStnClickedSaleatorio()
{
	if( m_nAlgoritmo==1) {
		m_EAleatorio.Uncollapse();
	} else {
		m_nAlgoritmo=1;
		m_SGeometrico.Collapse();
		m_EPar.Collapse();
		AfxGetApp()->WriteProfileInt("archivo","algoritmo", m_nAlgoritmo);
	}
}

void CarcanunDlg::OnStnClickedSgeometrico()
{
	if( m_nAlgoritmo==2) {
		m_SGeometrico.Uncollapse();
	} else {
		m_nAlgoritmo=2;
		m_EAleatorio.Collapse();
		m_EPar.Collapse();
		AfxGetApp()->WriteProfileInt("archivo","algoritmo", m_nAlgoritmo);
	}
}

void CarcanunDlg::OnStnClickedSpar()
{
	if( m_nAlgoritmo==3) {
		m_EPar.Uncollapse();
	} else {
		m_nAlgoritmo=3;
		m_EAleatorio.Collapse();
		m_SGeometrico.Collapse();
		AfxGetApp()->WriteProfileInt("archivo","algoritmo", m_nAlgoritmo); 
	}
}

// agregar un archivo
void CarcanunDlg::OnBnClickedOk()
{
	if( m_szBitmap.IsEmpty()) {
		OnBnClickedBexplorar();
		if(m_szBitmap.IsEmpty())
			return;
	}
	CFileDialog dialogo(true, "", "", NULL, "Todos los archivos (*.*)|*.*||", this);
	dialogo.m_pOFN->lpstrTitle= "Abrir Archivo A Agregar";
	CString add,szDestino;

	while( dialogo.DoModal()==IDOK) {
		
		add= dialogo.GetPathName();
		CFile archivo;
		if( !archivo.Open(add, CFile::modeRead)) {
			MessageBox("No se puede Abrir el archivo " + add);
			return;
		}
		if((m_BRSimple.GetCheck() && archivo.GetLength()>=m_nDisponibles) ||  
			( !m_BRSimple.GetCheck() && archivo.GetLength()>= m_nDisponiblesLBT-20)) {
			archivo.Close();
			if( MessageBox("El Archivo " + add + "\nTiene un tamaño superior al permitido.\n¿Desea Seleccionar otro archivo?", AfxGetAppName(), MB_ICONINFORMATION | MB_YESNO)==IDNO) 
				return;
			else continue;
		}
		archivo.Close();
		break;
	}
	if( add.IsEmpty())
		return;

	CFileDialog destino(false, "bmp", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Archivos de Imagen (*.bmp)|*.bmp|Todos los archivos (*.*)|*.*||", this);
	destino.m_pOFN->lpstrTitle= "Guardar Bitmap Modificado";
	if( destino.DoModal()==IDOK) {
		szDestino= destino.GetPathName();
	} else return;
	
	CString parametro;

	ASSERT(!m_pThreadBitmap);
	if(m_pThreadBitmap)
		delete m_pThreadBitmap;
	m_pThreadBitmap= new CThreadBmp(this);
	m_szDestino= szDestino;
	m_pThreadBitmap->SetArchivos(m_szBitmap, szDestino, add);

	switch( m_nAlgoritmo) {
		case 1: // aleatorio	
			m_EContrasena.GetWindowText(parametro);
			if( parametro.IsEmpty()) {
				delete m_pThreadBitmap;
				m_pThreadBitmap= NULL;
				MessageBox("Debe ingresar una contraseña");
				m_EContrasena.SetFocus();
				return;
			}
			m_pThreadBitmap->SetPassword(parametro);
			AfxGetApp()->WriteProfileString("archivo", "password", parametro);
			break;
		case 2: // geometrico
			m_EECuacion.GetWindowText(parametro);
			if( parametro.IsEmpty()) {
				delete m_pThreadBitmap;
				m_pThreadBitmap= NULL;
				MessageBox("Debe Ingresar una ecuacion");
				m_EECuacion.SetFocus();
				return;
			}
			m_pThreadBitmap->SetEcuacion(parametro);
			AfxGetApp()->WriteProfileString("archivo", "ecuacion", parametro);
			m_SEcuacion.GetWindowText(parametro);
			if( !parametro.IsEmpty()) {
				delete m_pThreadBitmap;
				m_pThreadBitmap= NULL;
				MessageBox("Debe Ingresar una ecuacion correcta");
				m_EECuacion.SetFocus();
				return;
			}
			break;
		case 3: // par e impar
			m_EOffset.GetWindowText(parametro);
			if( parametro.IsEmpty()) {
				delete m_pThreadBitmap;
				m_pThreadBitmap= NULL;
				MessageBox("Debe ingresar un offset");
				m_EOffset.SetFocus();
				return;
			}
            AfxGetApp()->WriteProfileString("archivo", "offset", parametro);
			m_pThreadBitmap->SetParImpar(m_BPar.GetCheck()==1, atol(parametro));
			AfxGetApp()->WriteProfileInt("archivos", "par", m_BPar.GetCheck());
			break;
	}
	CWnd *boton= GetDlgItem(IDOK);
	if( boton)
		boton->EnableWindow(false);
	if( boton= GetDlgItem(ID_BEXTRAER))
		boton->EnableWindow(false);

	m_pThreadBitmap->SetLBT(m_BRSimple.GetCheck()==0);
	m_bExtraer= false;
	m_pThreadBitmap->SetShowPixeles(m_BShow.GetCheck()==1);
	m_pThreadBitmap->Run();
	// OnOK();
}

// extraer un archivo
void CarcanunDlg::OnBnClickedBextraer()
{
	if( m_szBitmap.IsEmpty()) {
		OnBnClickedBexplorar();
		if(m_szBitmap.IsEmpty())
			return;
	}

	CString szDestino,szOriginal;

	CFileDialog original(true, "bmp", "", NULL, "Archivos de Imagen (*.bmp)|*.bmp|Todos los archivos (*.*)|*.*||", this);
	original.m_pOFN->lpstrTitle= "Abrir Bitmap Modificado";
	if( original.DoModal()==IDOK) {
		szOriginal= original.GetPathName();
	} else return;

	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;
	bi.lpszTitle = "Ruta de Extracción";

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	TCHAR szFolder[MAX_PATH*2];
	szFolder[0] = _T('\0');

	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
			szDestino= szFolder;
		IMalloc *pMalloc = NULL; 
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc) 
		{
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
		}
	}
	if( szDestino.IsEmpty())
		return;
	
	CString parametro;

	ASSERT(!m_pThreadBitmap);
	if(m_pThreadBitmap)
		delete m_pThreadBitmap;
	m_pThreadBitmap= new CThreadBmp(this);
	m_pThreadBitmap->SetArchivos(szOriginal, szDestino);
	m_pThreadBitmap->SetOriginalBitmap(m_szBitmap);

	switch( m_nAlgoritmo) {
		case 1: // aleatorio	
			m_EContrasena.GetWindowText(parametro);
			if( parametro.IsEmpty()) {
				delete m_pThreadBitmap;
				m_pThreadBitmap= NULL;
				MessageBox("Debe ingresar una contraseña");
				m_EContrasena.SetFocus();
				return;
			}
			m_pThreadBitmap->SetPassword(parametro);
			AfxGetApp()->WriteProfileString("archivo", "password", parametro);
			break;
		case 2: // geometrico
			m_EECuacion.GetWindowText(parametro);
			if( parametro.IsEmpty()) {
				delete m_pThreadBitmap;
				m_pThreadBitmap= NULL;
				MessageBox("Debe Ingresar una ecuacion");
				m_EECuacion.SetFocus();
				return;
			}
			m_pThreadBitmap->SetEcuacion(parametro);
			AfxGetApp()->WriteProfileString("archivo", "ecuacion", parametro);
			m_SEcuacion.GetWindowText(parametro);
			if( !parametro.IsEmpty()) {
				delete m_pThreadBitmap;
				m_pThreadBitmap= NULL;
				MessageBox("Debe Ingresar una ecuacion correcta");
				m_EECuacion.SetFocus();
				return;
			}
			break;
		case 3: // par e impar
			m_EOffset.GetWindowText(parametro);
			if( parametro.IsEmpty()) {
				delete m_pThreadBitmap;
				m_pThreadBitmap= NULL;
				MessageBox("Debe ingresar un offset");
				m_EOffset.SetFocus();
				return;
			}
            AfxGetApp()->WriteProfileString("archivo", "offset", parametro);
			m_pThreadBitmap->SetParImpar(m_BPar.GetCheck()==1, atol(parametro));
			AfxGetApp()->WriteProfileInt("archivos", "par", m_BPar.GetCheck());
			break;
	}

	m_pThreadBitmap->SetLBT(m_BRSimple.GetCheck()==0);
	CWnd *boton= GetDlgItem(IDOK);
	if( boton)
		boton->EnableWindow(false);
	if( boton= GetDlgItem(ID_BEXTRAER))
		boton->EnableWindow(false);
	m_bExtraer= true;
	m_pThreadBitmap->Run();
}

void CarcanunDlg::OnBnClickedCancel()
{
	CWaitCursor wait;
	if( m_pThreadBitmap) {
        
		m_pThreadBitmap->m_Thread.Suspend();
		if( MessageBox("¿Esta seguro de cancelar la operacion?", AfxGetAppName(), MB_ICONQUESTION | MB_YESNO)==IDNO) {
			m_pThreadBitmap->m_Thread.Resume();
		} else {
			delete m_pThreadBitmap;
			m_pThreadBitmap= NULL;
			CWnd *boton= GetDlgItem(IDOK);
			if( boton)
				boton->EnableWindow();
			if( boton= GetDlgItem(ID_BEXTRAER))
				boton->EnableWindow();
		}
	} else OnCancel();
}

void CarcanunDlg::OnDestroy()
{
	CWaitCursor wait;
	CDialog::OnDestroy();
	if( m_pThreadBitmap)
		delete m_pThreadBitmap;
}

LRESULT CarcanunDlg::OnBitmapMessage(WPARAM wParam, LPARAM lParam) {

	if( wParam==0) {
		m_EProgreso.SetPos((int)lParam);
	} else if( wParam==2) { // termino
		
		ASSERT(m_pThreadBitmap);
		delete m_pThreadBitmap;
		m_pThreadBitmap= NULL;
		CWnd *boton= GetDlgItem(IDOK);
		if( boton)
			boton->EnableWindow();
		if( boton= GetDlgItem(ID_BEXTRAER))
			boton->EnableWindow();
		if( lParam==1) {
			MessageBox("Operación completa");
			if(!m_bExtraer && m_BShow.GetCheck()) {
				CViewDlg dialogo;
				dialogo.m_szModificado=m_szDestino;
				dialogo.m_szOriginal=m_szBitmap;
				char temporal[MAX_PATH];
				GetTempPath( MAX_PATH, temporal);
				dialogo.m_szPixeles= temporal;
				dialogo.m_szPixeles+= "temporal.bmp";
				dialogo.DoModal();
			}
		} else {
			MessageBox("Ocurrio un error durante la operación", AfxGetAppName(), MB_ICONERROR);
		}
	}
	return 0;
}

void CarcanunDlg::OnBnClickedCshow()
{
	AfxGetApp()->WriteProfileInt("archivos", "show", m_BShow.GetCheck());
}
