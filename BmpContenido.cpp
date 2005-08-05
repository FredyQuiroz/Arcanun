#include "StdAfx.h"
#include "bmpcontenido.h"

CBmpContenido::CBmpContenido(void)
{
	m_lpbContenido= NULL;
	m_pBitsPorPixel= NULL;
	m_lpbNuevo= NULL;
	m_nBitsDisponibles=0;
	m_bPar= false;
	m_pParser= NULL;
	m_nOffset= 0;
	m_bMarcar= false;
	m_pParent= NULL;
	m_bLBT=true;
}

CBmpContenido::~CBmpContenido(void)
{
	if(m_lpbContenido)
		delete m_lpbContenido;
	if(m_lpbNuevo)
		delete m_lpbNuevo;
	if(m_pBitsPorPixel)
		delete m_pBitsPorPixel;
	if( m_pParser)
		delete m_pParser;
}

void CBmpContenido::SetParent(CWnd *parent) {
	m_pParent= parent;
}

BYTE *CBmpContenido::GetHeader(){
	return m_bHeader;
}

void CBmpContenido::SetLBT(bool use) {

	m_bLBT= use;
}

BYTE *CBmpContenido::GetContenido(){

	return m_lpbContenido;
}

BYTE *CBmpContenido::GetNuevo() {

	return m_lpbNuevo;
}

void CBmpContenido::ResetNuevo() {

	if( m_lpbNuevo)
		delete m_lpbNuevo;
	m_lpbNuevo= new BYTE[m_nPixeles*3];
	memcpy(m_lpbNuevo, m_lpbContenido, m_nPixeles*3);
}

bool CBmpContenido::SetBitmap(CString bitmap) {
	
	m_szBitmapPath= bitmap;
	CFile fbitmap;
	if( !fbitmap.Open(bitmap, CFile::modeRead))
		return false;

	fbitmap.Read(m_bHeader,HEADER_SIZE);
	ULONGLONG tamano= fbitmap.GetLength();
	if( m_lpbContenido)
		delete m_lpbContenido;
	if( m_lpbNuevo)
		delete m_lpbNuevo;

	tamano-=HEADER_SIZE;
	m_lpbContenido= new BYTE[tamano];
	m_lpbNuevo= new BYTE[tamano];
	fbitmap.Read(m_lpbContenido, tamano);
	memcpy(m_lpbNuevo, m_lpbContenido, tamano);
	
	ASSERT(tamano%3==0);
	m_nPixeles= tamano/3;
	fbitmap.Close();
	BuildTable();
	return true;
}

int	CBmpContenido::BuildTable() {

	ASSERT(m_lpbContenido);
	m_nBitsDisponibles=0;
	int r, g, b;
	int delta,delta1;
	if(m_pBitsPorPixel)
		delete m_pBitsPorPixel;

	m_pBitsPorPixel = new char [m_nPixeles];

	for( int i=0; i<m_nPixeles; i++) {
    
		GetPixel(r, g, b, i);
		delta= abs(r-g);
		delta1= abs(g-b);
		delta = delta>delta1 ? delta : delta1;
		m_pBitsPorPixel[i]= delta>150 ? 3 : 1;
		m_nBitsDisponibles+= m_pBitsPorPixel[i];
	}
	return m_nBitsDisponibles;
}

void CBmpContenido::GetPixel(int &r, int &g, int &b, int index) {

	ASSERT(index<=m_nPixeles);
	// index= m_nPixeles-index;
	index*=3;
	r= m_lpbContenido[index+2];
	g= m_lpbContenido[index+1];
	b= m_lpbContenido[index];
}

COLORREF CBmpContenido::GetPixel(int index) {

	ASSERT(index<=m_nPixeles);
	// index= m_nPixeles-index;
	index*=3;
	return RGB(m_lpbContenido[index+2], m_lpbContenido[index+1], m_lpbContenido[index]);
}

void CBmpContenido::SetPixel(int index, COLORREF color) {

	ASSERT(index<=m_nPixeles);
	// index= m_nPixeles-index;
	index*=3;
	m_lpbContenido[index]= GetBValue(color);
	m_lpbContenido[index+1]=GetGValue(color);
	m_lpbContenido[index+2]=GetRValue(color);
}

void CBmpContenido::GetPixelD(int &r, int &g, int &b, int index) {

	ASSERT(index<=m_nPixeles);
	// index= m_nPixeles-index;
	index*=3;
	r= m_lpbNuevo[index+2];
	g= m_lpbNuevo[index+1];
	b= m_lpbNuevo[index];
}

COLORREF CBmpContenido::GetPixelD(int index) {

	ASSERT(index<=m_nPixeles);
	// index= m_nPixeles-index;
	index*=3;
	return RGB(m_lpbNuevo[index+2], m_lpbNuevo[index+1], m_lpbNuevo[index]);
}

void CBmpContenido::SetPixelD(int index, COLORREF color) {

	ASSERT(index<=m_nPixeles);
	// index= m_nPixeles-index;
	index*=3;
	m_lpbNuevo[index]= GetBValue(color);
	m_lpbNuevo[index+1]=GetGValue(color);
	m_lpbNuevo[index+2]=GetRValue(color);
}

int	CBmpContenido::GetPixeles() {

	return m_nPixeles;
}

int	CBmpContenido::GetBitsDisponibles() {

	return m_nBitsDisponibles;
}

void CBmpContenido::SetEcuacion(CString ecuacion) {
	m_szEcuacion= ecuacion;
}

void CBmpContenido::SetPassword(CString password) {

	m_szPassword= password;
}

void CBmpContenido::AddBytesSimple(const char *bytes, int tamano) {

	int r,g,b;
	int bits=0;
	int i=0;
	
	for( bits=0; bits<tamano;) {
		if( m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_BITMAP, 0, (bits*100)/tamano);
        GetPixel(r, g, b, i);
		if(m_bMarcar) {
			r=g=b=0;
			b=255;
			bits+=3;
		} else {
			r&=254;
			g&=254;
			b&=254;
			r|=bytes[bits++];
			g|=bytes[bits++];
			b|=bytes[bits++];
		}
		SetPixelD(i, RGB(r,g,b));
		i++;
	}
}

CSize CBmpContenido::GetBitmaptamano() {

	CBitmap mibitmap;
	HBITMAP handle= (HBITMAP)LoadImage(NULL, m_szBitmapPath,IMAGE_BITMAP,NULL,NULL,LR_LOADFROMFILE);
	mibitmap.Attach(handle);
	BITMAP bmp;
	mibitmap.GetBitmap(&bmp);
	mibitmap.Detach();
	DeleteObject(handle);
	return CSize(bmp.bmWidth,bmp.bmHeight); 
}

int	CBmpContenido::GetNextPixelMetodo(int pos) {

	m_nOffset++;
	if( m_nOffset>=m_szPassword.GetLength()-1) 
		m_nOffset=0;
	pos+= m_szPassword[m_nOffset];
	if( pos>=m_nPixeles)
		pos=0;
	if( m_pBitsPorPixel[pos])
		return pos;
	return GetNextPixel(pos);
}

int	CBmpContenido::GetNextPixelEcuacion(int pos) {

	int y= (int) -1*m_pParser->SetEcuacion(m_szEcuacion, pos);

	if( y>m_szTamano.cy)
		y= m_szTamano.cy;
	else if( y<-m_szTamano.cy)
		y= -m_szTamano.cy;
	y= m_szTamano.cy-y;
	pos+= m_szTamano.cx;
	y*=2*m_szTamano.cx;
	y+=pos;
	if( m_pBitsPorPixel[y])
		return y;
	return GetNextPixel(y);
}

void CBmpContenido::AddBytesEcuacion(const char *bytes, int tamano) {

	int r,g,b;
	int bits=0;
	int x=-m_szTamano.cx;
	int y=0;
	int cantidad;
	int traslados=0;

	
	for( bits=0; bits<tamano;) {

		if( m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_BITMAP, 0, (bits*100)/tamano);
		if((y = GetNextPixelEcuacion(x))==-1)
			break;
        GetPixel(r, g, b, y);
		cantidad= m_bLBT ? m_pBitsPorPixel[y] : 1;
		m_pBitsPorPixel[y]=0;
		if(m_bMarcar) {
			r=g=b=0;
			cantidad==1 ? r=255 : b=255;
			cantidad==1 ? bits+=3 : bits+=8;
		} else {
			if( cantidad==1) {
				r&=254;
				g&=254;
				b&=254;
				r|=bytes[bits++];
				g|=bytes[bits++];
				b|=bytes[bits++];
			} else {
				r&=248;
				g&=248;
				b&=248;
				r|=bytes[bits++];
				r|=bytes[bits++]*2;
				r|=bytes[bits++]*4;
				g|=bytes[bits++];
				g|=bytes[bits++]*2;
				g|=bytes[bits++]*4;
				b|=bytes[bits++];
				b|=bytes[bits++]*2;
				b|=bytes[bits++]*4;
			}
		}
		SetPixelD(y, RGB(r,g,b));
		x++;
		if(x>m_szTamano.cx) {
			CString delta;
			delta.Format(traslados>0 ? "+%d" : "%d", traslados);
			if(traslados) 
				m_szEcuacion.Delete(m_szEcuacion.GetLength()-delta.GetLength(), delta.GetLength());
			x= -m_szTamano.cx;
			traslados+= traslados>=0 ? 1 : -1;
			delta.Format("+%d", traslados);
			m_szEcuacion+=delta;
			if(traslados>=m_szTamano.cy)
				traslados=-1;
		}
	}
	delete m_pParser;
	m_pParser= NULL;
}

bool CBmpContenido::GetBytesEcuacion(char *bytes, int tamano, CBmpContenido *original) {

	if(!original || !original->m_pBitsPorPixel || original->m_nPixeles!= m_nPixeles)
		return false;
	if(m_nPixeles<tamano)
		return false;

	int r,g,b;
	int bits=0;
	int cantidad;
	int x=-m_szTamano.cx, y=0;
	int traslados=0;
	
	if(m_pBitsPorPixel)
		delete m_pBitsPorPixel;

	m_pBitsPorPixel= original->m_pBitsPorPixel;

	for( int i=0;i<tamano; i++) {

		if( m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_BITMAP, 0, (i*100)/tamano);
		if((y = GetNextPixelEcuacion(x))==-1)
			break;
		GetPixel(r, g,b,y);
		cantidad= m_bLBT ? m_pBitsPorPixel[y] : 1;
		m_pBitsPorPixel[y]=0;

		if( cantidad==1) {
			bytes[bits++]=r&1;
			bytes[bits++]=g&1;
			bytes[bits++]=b&1;
		} else {

			bytes[bits++]=r&1;
			bytes[bits++]=r&2 ? 1 : 0;
			bytes[bits++]=r&4 ? 1 : 0;
			bytes[bits++]=g&1;
			bytes[bits++]=g&2 ? 1 : 0;
			bytes[bits++]=g&4 ? 1 : 0;
			bytes[bits++]=b&1;
			bytes[bits++]=b&2 ? 1 : 0;
			bytes[bits++]=b&4 ? 1 : 0;
		}
        x++;
		if(x>m_szTamano.cx) {
			CString delta;
			delta.Format(traslados >= 0 ? "+%d" : "%d", traslados);
			if(traslados) 
				m_szEcuacion.Delete(m_szEcuacion.GetLength()-delta.GetLength(), delta.GetLength());
			x= -m_szTamano.cx;
			traslados+= traslados>=0 ? 1 : -1;
			delta.Format("+%d", traslados);
			m_szEcuacion+=delta;
			if(traslados>=m_szTamano.cy)
				traslados=-1;
		}
	}
	m_pBitsPorPixel= NULL;
	delete m_pParser;
	m_pParser= NULL;
	return true;
}


void CBmpContenido::AddBytesMetodo(const char *bytes, int tamano)  {

	int r,g,b;
	int bits=0;
	int i=-1;
	int cantidad;
	m_nOffset=0;
	
	for( bits=0; bits<tamano;) {
		
		if( m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_BITMAP, 0, (bits*100)/tamano);
		i= GetNextPixelMetodo(i);
        GetPixel(r, g, b, i);
		cantidad= m_bLBT ? m_pBitsPorPixel[i] : 1;
		m_pBitsPorPixel[i]=0;
		if(m_bMarcar) {
			r=g=b=0;
			cantidad==1 ? r=255 : b=255;
			cantidad==1 ? bits+=3 : bits+=8;
		} else {
			if( cantidad==1) {
				r&=254;
				g&=254;
				b&=254;
				r|=bytes[bits++];
				g|=bytes[bits++];
				b|=bytes[bits++];
			} else {

				r&=248;
				g&=248;
				b&=248;
				r|=bytes[bits++];
				r|=bytes[bits++]*2;
				r|=bytes[bits++]*4;
				g|=bytes[bits++];
				g|=bytes[bits++]*2;
				g|=bytes[bits++]*4;
				b|=bytes[bits++];
				b|=bytes[bits++]*2;
				b|=bytes[bits++]*4;
			}
		}
		SetPixelD(i, RGB(r,g,b));
	}
}

bool CBmpContenido::GetBytesMetodo(char *bytes, int tamano, CBmpContenido *original) {

	if(!original || !original->m_pBitsPorPixel || original->m_nPixeles!= m_nPixeles)
		return false;
	if(m_nPixeles<tamano)
		return false;

	int r,g,b;
	int bits=0;
	int pos=-1;
	int cantidad;
	m_nOffset=0;
	if(m_pBitsPorPixel)
		delete m_pBitsPorPixel;
	m_pBitsPorPixel= original->m_pBitsPorPixel;

	for( int i=0;i<tamano; i++) {

		if( m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_BITMAP, 0, (i*100)/tamano);
		pos= GetNextPixelMetodo(pos);
		GetPixel(r, g,b,pos);
		cantidad= m_bLBT ? m_pBitsPorPixel[pos] : 1;
		m_pBitsPorPixel[pos]=0;
		if( cantidad==1) {

			bytes[bits++]=r&1;
			bytes[bits++]=g&1;
			bytes[bits++]=b&1;

		} else {

			bytes[bits++]=r&1;
			bytes[bits++]=r&2 ? 1 : 0;
			bytes[bits++]=r&4 ? 1 : 0;
			bytes[bits++]=g&1;
			bytes[bits++]=g&2 ? 1 : 0;
			bytes[bits++]=g&4 ? 1 : 0;
			bytes[bits++]=b&1;
			bytes[bits++]=b&2 ? 1 : 0;
			bytes[bits++]=b&4 ? 1 : 0;
		}
	}
	m_pBitsPorPixel= NULL;
	return true;
}

void CBmpContenido::SetMarcar(bool marcar) {
	
	m_bMarcar= marcar;
}

bool CBmpContenido::GetBytesSimple(char *bytes, int tamano) {

	int r,g,b;
	int bits=0;

	if(m_nPixeles<tamano)
		return false;

	for( int i=0;i<tamano; i++) {

		if( m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_BITMAP, 0, (i*100)/tamano);
		GetPixel(r, g,b,i);
		bytes[bits++]=r&1;
		bytes[bits++]=g&1;
		bytes[bits++]=b&1;
	}
	return true;
}

void CBmpContenido::AddBytes(const char *bytes, int tamano, int metodo) {
	switch(metodo) {
		case 0:
			AddBytesSimple(bytes, tamano);
			break;
		case 1:
			AddBytesMetodo(bytes, tamano);
			break;
		case 2:
			AddBytesParoIm(bytes, tamano);
			break;
		case 3:
			m_szTamano= GetBitmaptamano();
			m_szTamano.cx/=2;
			m_szTamano.cy/=2;
			if( m_pParser) 
				delete m_pParser;
			m_pParser= new CParser();
			AddBytesEcuacion(bytes, tamano);
			break;
		default:
			ASSERT(false);
	}
}

int CBmpContenido::GetNextPixel(int pos) {

	bool found=false;
	bool inicio=false;

	pos+=2;
	if( pos==0)
		inicio=true;
	if( pos>m_nPixeles)
		pos=0;
	for( int i=pos; i<m_nPixeles; i++) {
		if( m_pBitsPorPixel[i]) { // buscar un campo en blanco
			pos= i;
			found= true;
			break;
		}
	}
	if( found)
		return pos;
	else if( !inicio)
		return GetNextPixel(-2);
	else return -1;
}

bool CBmpContenido::GetBytesParoIm(char *bytes, int tamano, CBmpContenido *original) {

	if(!original || !original->m_pBitsPorPixel || original->m_nPixeles!= m_nPixeles)
		return false;
	if(m_nPixeles<tamano)
		return false;

	int r,g,b;
	int bits=0;
	int cantidad;
	int pixel=m_nOffset;
	bool impar=pixel%2;
	if( m_bPar==impar)
		pixel++;
	pixel-= 2;

	for( int i=0;i<tamano; i++) {

		if( m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_BITMAP, 0, (i*100)/tamano);
		if((pixel=original->GetNextPixel(pixel))==-1)
			break;
		GetPixel(r, g,b,pixel);
		cantidad= m_bLBT ? original->m_pBitsPorPixel[pixel] : 1;
		original->m_pBitsPorPixel[pixel]=0;

		if( cantidad==1) {
			bytes[bits++]=r&1;
			bytes[bits++]=g&1;
			bytes[bits++]=b&1;
		} else {

			bytes[bits++]=r&1;
			bytes[bits++]=r&2 ? 1 : 0;
			bytes[bits++]=r&4 ? 1 : 0;
			bytes[bits++]=g&1;
			bytes[bits++]=g&2 ? 1 : 0;
			bytes[bits++]=g&4 ? 1 : 0;
			bytes[bits++]=b&1;
			bytes[bits++]=b&2 ? 1 : 0;
			bytes[bits++]=b&4 ? 1 : 0;
		}
	}
	return true;
}

void CBmpContenido::AddBytesParoIm(const char *bytes, int tamano) {

	int r,g,b;
	int bits=0;
	int i=m_nOffset;
	int cantidad;
	bool impar=i%2;
	if( m_bPar==impar)
		i++;
	i-= 2;

	for( bits=0; bits<tamano;) {

		if( m_pParent && ::IsWindow(*m_pParent)) 
			m_pParent->SendMessage(WM_BITMAP, 0, (bits*100)/tamano);
		if((i = GetNextPixel(i))==-1)
			break;
        GetPixel(r, g, b, i);
		cantidad= m_bLBT ? m_pBitsPorPixel[i] : 1;
		m_pBitsPorPixel[i]=0;
		if(m_bMarcar) {
			r=g=b=0;
			cantidad==1 ? r=255 : b=255;
			cantidad==1 ? bits+=3 : bits+=8;
		} else {
			if( cantidad==1) {
				r&=254;
				g&=254;
				b&=254;
				r|=bytes[bits++];
				g|=bytes[bits++];
				b|=bytes[bits++];
			} else {
				r&=248;
				g&=248;
				b&=248;
				r|=bytes[bits++];
				r|=bytes[bits++]*2;
				r|=bytes[bits++]*4;
				g|=bytes[bits++];
				g|=bytes[bits++]*2;
				g|=bytes[bits++]*4;
				b|=bytes[bits++];
				b|=bytes[bits++]*2;
				b|=bytes[bits++]*4;
			}
		}
		SetPixelD(i, RGB(r,g,b));
	}
}

bool CBmpContenido::GetBytes(char *bytes, int tamano, int metodo, CBmpContenido *original) {

	switch(metodo) {
		case 0:
			return GetBytesSimple(bytes, tamano);
		case 1:
			ASSERT(original);
			return GetBytesMetodo(bytes, tamano, original);
			break;
		case 2:
			ASSERT(original);
			return GetBytesParoIm(bytes, tamano, original);
		case 3:
			ASSERT(original);
			if( m_pParser) 
				delete m_pParser;
			m_pParser= new CParser();
			m_szTamano= original->GetBitmaptamano();
			m_szTamano.cx/=2;
			m_szTamano.cy/=2;
			if(original->m_lpbNuevo)
				delete original->m_lpbNuevo;
			original->m_lpbNuevo=NULL;
			return GetBytesEcuacion(bytes, tamano, original);
		default:
			ASSERT(false);
	}
	return false;
}

int	 CBmpContenido::GetBitsNecesarios(int tipo, CBmpContenido *original) {

	switch( tipo) {
		case 0:
			return m_nPixeles*8*3;
			break;
		case 1:
		case 2:
		case 3:
            if( original)
				return original->m_nBitsDisponibles*3;
			return 0;
			break;
		default:
			ASSERT(false);
			break;
	}
	return false;
}

void CBmpContenido::SetParametrosTipo2(int offset, bool par) {
	
	m_nOffset= offset;
	m_bPar= par;
}
