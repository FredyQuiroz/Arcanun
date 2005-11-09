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

# define	ID_FILE_HELP	1150
# define	HELP_TIMER		1000
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
	m_nMetodo=1;
	m_nAlgoritmo=0;
	m_nDisponibles=0;
	m_nDisponiblesLBT=0;
	m_pThreadBitmap= NULL;
	m_bExtraer= false;
	m_pOldFocus= NULL;
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
	DDX_Control(pDX, IDC_SAYUDA, m_SAyuda);
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
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FILE_HELP,OnMenuAyuda)
	ON_WM_TIMER()
END_MESSAGE_MAP()

CString GetAppPath(const char *archivo) {

	TCHAR szEXEPath[MAX_PATH];

	GetModuleFileName ( NULL, szEXEPath, MAX_PATH );
    CString path= szEXEPath;
	path.MakeLower();
	path.TrimRight("arcanun.exe");
	path+= archivo;
	return path;
}

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
	m_SGeometrico.Collapse();
	m_EPar.SetNextSectionCtrl(m_SFin);
	m_EPar.Collapse();
	m_EProgreso.SetRange(0,100);
	m_SAyuda.SetUnderline();
	m_szFileHelp= GetAppPath("ayuda.htm");
	m_SAyuda.SetURL(m_szFileHelp);

	m_EBitmap.SetBitmap(GetAppPath("logo.bmp"));

	OnStnClickedSaleatorio();

#ifdef _DEBUG
	switch(AfxGetApp()->GetProfileInt("archivo","algoritmo", m_nAlgoritmo)) {
		case 1:
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
#endif
	m_EMetodo.SetCheck(1);

	m_LCDetalles.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LCDetalles.InsertColumn(0, "Propiedad", LVCFMT_LEFT, 100);
	m_LCDetalles.InsertColumn(1, "Valor", LVCFMT_LEFT, 100);
	m_LCDetalles.InsertItem(0, "x");
	m_LCDetalles.InsertItem(1, "y");
	m_LCDetalles.InsertItem(2, "pixeles");
	m_LCDetalles.InsertItem(3, "disponibles LSB");
	m_LCDetalles.InsertItem(4, "disponibles LBT");

#ifdef _DEBUG
	 m_szBitmap= AfxGetApp()->GetProfileString("archivo", "archivo");
	 m_EArchivo.SetWindowText(m_szBitmap);
	 m_EBitmap.SetBitmap(m_szBitmap);
	 FillLista(m_szBitmap);

	 m_EECuacion.SetWindowText(AfxGetApp()->GetProfileString("archivo", "ecuacion"));
	 m_EContrasena.SetWindowText(AfxGetApp()->GetProfileString("archivo", "password"));
	 m_EOffset.SetWindowText(AfxGetApp()->GetProfileString("archivo", "offset"));
	 m_BPar.SetCheck(AfxGetApp()->GetProfileInt("archivos", "par", 1));
	 m_BShow.SetCheck(AfxGetApp()->GetProfileInt("archivos", "show", 1));
#endif
	OnBnClickedCpar();
	SetTimer(1, HELP_TIMER, NULL);
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

		if( !bmpcontenido.IsBmp24())
			return false;
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
		
		if(!FillLista(dialogo.GetPathName())) {
			MessageBox("Bitmap no compatible.\rIntente con un bitmap con profundidad de 24 bits.");
			return;
		}
		m_szBitmap= dialogo.GetPathName();
		m_EArchivo.SetWindowText(m_szBitmap);
#ifdef _DEBUG
		AfxGetApp()->WriteProfileString("archivo", "archivo", m_szBitmap);
#endif
		m_EBitmap.SetBitmap(m_szBitmap);
		m_EBitmap.Invalidate();
	}
}

void CarcanunDlg::OnBnClickedRlbt()
{
	m_nMetodo=1;
#ifdef _DEBUG
	AfxGetApp()->WriteProfileInt("archivo", "metodo", m_nMetodo);
#endif
}

void CarcanunDlg::OnBnClickedRsimple()
{
	m_nMetodo=0;
#ifdef _DEBUG
	AfxGetApp()->WriteProfileInt("archivo", "metodo", m_nMetodo);
#endif
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
	if( m_nAlgoritmo!=1) {
		m_nAlgoritmo=1;
		m_SGeometrico.Collapse();
		m_EPar.Collapse();
#ifdef _DEBUG
		AfxGetApp()->WriteProfileInt("archivo","algoritmo", m_nAlgoritmo);
#endif
	}
}

void CarcanunDlg::OnStnClickedSgeometrico()
{
	if( m_nAlgoritmo!=2) {
		m_nAlgoritmo=2;
		m_EAleatorio.Collapse();
		m_EPar.Collapse();
#ifdef _DEBUG
		AfxGetApp()->WriteProfileInt("archivo","algoritmo", m_nAlgoritmo);
#endif
	}
}

void CarcanunDlg::OnStnClickedSpar()
{
	if( m_nAlgoritmo!=3) {
		
		m_nAlgoritmo=3;
		m_EAleatorio.Collapse();
		m_SGeometrico.Collapse();
#ifdef _DEBUG
		AfxGetApp()->WriteProfileInt("archivo","algoritmo", m_nAlgoritmo); 
#endif
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
	dialogo.m_pOFN->lpstrTitle= "Archivo A Agregar";
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
#ifdef _DEBUG
			AfxGetApp()->WriteProfileString("archivo", "password", parametro);
#endif
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
#ifdef _DEBUG
			AfxGetApp()->WriteProfileString("archivo", "ecuacion", parametro);
#endif
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
#ifdef _DEBUG
            AfxGetApp()->WriteProfileString("archivo", "offset", parametro);
			AfxGetApp()->WriteProfileInt("archivos", "par", m_BPar.GetCheck());
#endif
			m_pThreadBitmap->SetParImpar(m_BPar.GetCheck()==1, atol(parametro));
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
#ifdef _DEBUG
			AfxGetApp()->WriteProfileString("archivo", "password", parametro);
#endif
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
#ifdef _DEBUG
			AfxGetApp()->WriteProfileString("archivo", "ecuacion", parametro);
# endif
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
#ifdef _DEBUG
            AfxGetApp()->WriteProfileString("archivo", "offset", parametro);
			AfxGetApp()->WriteProfileInt("archivos", "par", m_BPar.GetCheck());
#endif
			m_pThreadBitmap->SetParImpar(m_BPar.GetCheck()==1, atol(parametro));
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
#ifdef _DEBUG
	AfxGetApp()->WriteProfileInt("archivos", "show", m_BShow.GetCheck());
#endif
}

void CarcanunDlg::SetHelp(CWnd *item, CString &texto, CString &ayuda) {

	ayuda= m_szFileHelp;
	texto= "Ayuda";
	m_SAyuda.SetWindowText(texto);
	m_SAyuda.SetURL(ayuda);
	m_SAyuda.Invalidate();
	switch(item->GetDlgCtrlID()) {
		case IDC_SVISOR:
		case IDC_EARCHIVO:
            ayuda+="#VISOR";
			texto="Ayuda - Visor";
			break;
		case IDC_BEXPLORAR:
			ayuda+="#EXPLORAR";
			texto= "Ayuda - Explorar";
			break;
		case IDC_SBITMAP:
			ayuda+="#VISOR3";
			texto="Ayuda - Previsualización";
			break;
		case IDC_LDETALLES:
			ayuda+="#GRILLA4";
			texto="Ayuda - Detalles";
			break;
		case IDC_RLBT:
		case IDC_RSIMPLE:
			ayuda+="#BOTONES5";
			texto="Ayuda - Metodo";
			break;
		case IDC_CSHOW:
			ayuda+="#PIXEL6";
			texto="Ayuda - Mostrar Pixeles";
			break;
		case IDC_ECONTRASENA:
		case IDC_SALEATORIO:
			ayuda+="#DESPLE7";
			texto="Ayuda - Aleatorio";
			break;
		case IDC_SGEOMETRICO:
		case IDC_ECUACION:
			ayuda+="#DESPLE8";
			texto="Ayuda - Geométrico";
			break;
		case IDC_SPAR:
		case IDC_ECOFFSET:
			ayuda+="#DESPLE9";
			texto="Ayuda - Par e impar";
			break;
		case IDC_CPAR:
			ayuda+="#PAREIMPAR10";
			if( m_BPar.GetCheck())
				texto="Ayuda - Par";
			else texto= "Ayuda - Impar";
			break;
		case IDC_PROGRESO:
			ayuda+="#BARRA11";
			texto="Ayuda - Progreso";
			break;
		case IDOK:
			ayuda+="#BOTON12";
			texto="Ayuda - Agregar";
			break;
		case ID_BEXTRAER:
			ayuda+="#BOTON13";
			texto="Ayuda - Extraer";
			break;
		case IDCANCEL:
			ayuda+="#BOTON14";
			texto="Ayuda - Cancelar";
			break;
	}
	m_SAyuda.SetWindowText(texto);
	m_SAyuda.SetURL(ayuda);
	m_SAyuda.Invalidate();
}

void CarcanunDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if( pWnd==this)
		Default();
	else if( pWnd) {
		
		CString texto, ayuda;
		m_pOldFocus= pWnd;
		SetHelp(pWnd, texto, ayuda);
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_FILE_HELP, texto);
		menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void CarcanunDlg::OnMenuAyuda() {

	m_SAyuda.OnClicked();
}

void CarcanunDlg::OnTimer(UINT nIDEvent)
{
	if( nIDEvent==1) {
		CWnd *focus= GetFocus();
		if( !focus || focus==m_pOldFocus)
			return;
		m_pOldFocus= focus;
		CString texto, ayuda;
		SetHelp(m_pOldFocus, texto, ayuda);
	}
	CDialog::OnTimer(nIDEvent);
}
