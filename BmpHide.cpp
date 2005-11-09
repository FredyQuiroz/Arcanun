#include "StdAfx.h"
#include "bmphide.h"
# include "math.h"

# define	MARGEN_ERROR	10

CBmpHide::CBmpHide(void)
{
	m_pParent=NULL;
	m_szHeaderMark="JH";
	m_pBufferAdd= NULL;
	m_pBmpModificado= NULL;
	m_pBmpNormal= NULL;
	m_nTipo= 0;
	m_nOffset= 0;
	m_bPar=false;
	m_bMarcar= false;
	m_pParent= NULL;
	m_bLBT=true;
}

CBmpHide::~CBmpHide(void)
{
	if(m_pBufferAdd)
		delete m_pBufferAdd;
	if( m_pBmpModificado)
		delete m_pBmpModificado;
	if( m_pBmpNormal)
		delete m_pBmpNormal;
}

void CBmpHide::SetParent(CWnd *parent) {
	m_pParent= parent;
}

void CBmpHide::SetParametrosTipo2(int offset, bool par) {
	
	m_nOffset= offset;
	m_bPar= par;
}

void CBmpHide::SetLBT(bool use) {

	m_bLBT= use;
}

void CBmpHide::SetEcuacion(CString ecuacion) {

	m_szEcuacion= ecuacion;
}

void CBmpHide::SetTipo(int tipo) {

	m_nTipo= tipo;
}

int	CBmpHide::GetTipo() {

	return m_nTipo;
}

bool CBmpHide::SetBitmap(CString bitmap) {

	m_szBitmap= bitmap;
	if( m_szBitmap.IsEmpty())
		return false;
	if( !m_pBmpNormal)
		m_pBmpNormal= new CBmpContenido();
	return m_pBmpNormal->SetBitmap(m_szBitmap);
}

CString CBmpHide::WriteFile(CString File) {

	ASSERT(m_pBmpNormal);
	if(!m_pBmpNormal)
		return NULL;
	CFile temporal;
	if(!temporal.Open(File, CFile::modeWrite | CFile::modeCreate)) {
		ASSERT(FALSE);
		return NULL;
	}
	temporal.Write(m_pBmpNormal->GetHeader(), HEADER_SIZE);
	temporal.Write(m_pBmpNormal->GetNuevo(), m_pBmpNormal->GetPixeles()*3);
	temporal.Close();
	return File;
}

void CBmpHide::AddByte(char *bytes, int &pos, int byte) {

	bytes[pos++]=byte&1 ? 1 : 0;
	bytes[pos++]=byte&2 ? 1 : 0;
	bytes[pos++]=byte&4 ? 1 : 0;
	bytes[pos++]=byte&8 ? 1 : 0;
	bytes[pos++]=byte&16 ? 1 : 0;
	bytes[pos++]=byte&32 ? 1 : 0;
	bytes[pos++]=byte&64 ? 1 : 0;
	bytes[pos++]=byte&128 ? 1 : 0;
}

bool CBmpHide::AddFile(CString file) {

	CFile temporal;
	if(!m_pBmpNormal)
		return false;
	if( !temporal.Open(file, CFile::modeRead)) {
		ASSERT(FALSE);
		return false;
	}
	int tamano= temporal.GetLength();
	if( tamano*8> m_pBmpNormal->GetPixeles()*3) {
		temporal.Close();
        return false;
	}
	if( m_pBmpModificado)
		delete m_pBmpModificado;
	m_pBmpModificado= NULL;

	m_pBmpNormal->ResetNuevo();
	m_pBmpNormal->SetParametrosTipo2(m_nOffset, m_bPar);
	m_pBmpNormal->SetPassword(m_szPassword);
	m_pBmpNormal->SetLBT(m_bLBT);
	m_pBmpNormal->SetEcuacion(m_szEcuacion);
	m_pBmpNormal->SetParent(m_pParent);

	if(m_pBufferAdd)
		delete m_pBufferAdd;
	BYTE *buffer= new BYTE[tamano];
	temporal.Read(buffer, tamano);
	temporal.Close();

	int nIndex=file.ReverseFind('\\');
	if(nIndex>0)
		file=file.Right(file.GetLength()-nIndex-1);
	nIndex= file.GetLength();

	m_pBufferAdd= new char[tamano*8+32+nIndex*8+32+ m_szHeaderMark.GetLength()*8];
	int bits=0;

	// marca de agua para reconocer el archivo
	for( int i=0; i<m_szHeaderMark.GetLength(); i++)
		AddByte(m_pBufferAdd, bits, m_szHeaderMark[i]);
	ASSERT(bits%8==0);
	// tamano del archivo en bytes
	AddInt(m_pBufferAdd, bits, tamano);
	ASSERT(bits%8==0);
	// tamano del nombre del archivo en bytes
	AddInt(m_pBufferAdd, bits, nIndex);
	ASSERT(bits%8==0);
	// nombre del archivo
	for( int i=0; i<file.GetLength(); i++) 
		AddByte(m_pBufferAdd, bits, file[i]);
	ASSERT(bits%8==0);
	// contenido del archivo
	for( int i=0; i<tamano; i++) 
		AddByte(m_pBufferAdd, bits, buffer[i]);

	ASSERT(bits%8==0);
	delete buffer;

	tamano*=8;
	tamano+=64;
	// tamano del nombre del archivo
	tamano+=nIndex*8;
	// tamano del encabezado
	tamano+= m_szHeaderMark.GetLength()*8;
	m_pBmpNormal->SetMarcar(m_bMarcar);
	m_pBmpNormal->AddBytes(m_pBufferAdd, tamano, m_nTipo);
	delete m_pBufferAdd;
	m_pBufferAdd=NULL;
	return true;
}

unsigned char CBmpHide::GetByte(char *bytes, int &pos) {

	unsigned char byte=0;
	for( int j=0; j<8; j++) { // tamano del byte
		if( bytes[pos++])
			byte|=((int)pow(2, j));
	}
	return byte;
}

void CBmpHide::AddInt(char *bytes, int &pos, int numero) {

	for( int i=0; i<32; i++) 
		bytes[pos++]= numero&((int)pow(2, i)) ? 1 : 0;
}

int CBmpHide::GetInt(char *bytes, int &pos) {

	int numero=0;
	for(int i=0; i<32; i++) {// tamaño del archivo a extraer
		if(bytes[pos++]) {// es uno
			numero|= ((int)pow(2, i));
		}
	}
	return numero;
}

bool CBmpHide::HasAnyHide(CString bitmap) {

	if( !m_pBmpModificado)
		m_pBmpModificado= new CBmpContenido();
	if( !m_pBmpModificado->SetBitmap(bitmap))
		return false;
	if(!m_pBmpNormal && m_nTipo)
		return false;

	m_pBmpModificado->SetParametrosTipo2(m_nOffset, m_bPar);
	if( m_pBmpNormal) {
		m_pBmpNormal->SetParametrosTipo2(m_nOffset, m_bPar);
		m_pBmpNormal->SetEcuacion(m_szEcuacion);
		m_pBmpNormal->SetParent(m_pParent);
		m_pBmpNormal->SetPassword(m_szPassword);
		m_pBmpNormal->SetLBT(m_bLBT);
	}
	m_pBmpModificado->SetEcuacion(m_szEcuacion);
	m_pBmpModificado->SetPassword(m_szPassword);
	m_pBmpModificado->SetParent(m_pParent);
	m_pBmpModificado->SetLBT(m_bLBT);

	if(m_pBufferAdd)
		delete m_pBufferAdd;
	m_pBufferAdd= new char[(5*m_szHeaderMark.GetLength())*8];
	m_pBmpModificado->ResetNuevo();
	
	if( !m_pBmpModificado->GetBytes(m_pBufferAdd, m_szHeaderMark.GetLength()*8+3, m_nTipo, m_pBmpNormal))
		return false;
    int bits=0;
	for( int i=0; i<m_szHeaderMark.GetLength(); i++) {
		if( m_szHeaderMark[i]!= GetByte(m_pBufferAdd, bits)) {// no corresponde la marca de agua
			return false;
		}
	}
	delete m_pBufferAdd;
	m_pBufferAdd=NULL;
	return true;
}

void CBmpHide::SetMarcar(bool marcar) {
	
	m_bMarcar=marcar;
}

void CBmpHide::SetPassword(CString password) {

	m_szPassword= password;
}

bool CBmpHide::ExtractFile(CString bitmap, CString archivo) {

	if( !m_pBmpModificado)
		m_pBmpModificado= new CBmpContenido();
	if( !m_pBmpModificado->SetBitmap(bitmap))
		return false;
	if(!m_pBmpNormal && m_nTipo)
		return false;

	m_pBmpModificado->SetParametrosTipo2(m_nOffset, m_bPar);
	m_pBmpModificado->SetPassword(m_szPassword);
	if( m_pBmpNormal) {
		m_pBmpNormal->SetPassword(m_szPassword);
		m_pBmpNormal->SetParametrosTipo2(m_nOffset, m_bPar);
		m_pBmpNormal->SetEcuacion(m_szEcuacion);
		m_pBmpNormal->SetParent(m_pParent);
		m_pBmpNormal->SetLBT(m_bLBT);
	}
	m_pBmpModificado->SetEcuacion(m_szEcuacion);
	m_pBmpModificado->SetParent(m_pParent);
	m_pBmpModificado->SetLBT(m_bLBT);

	int tamano=0;
	if(m_pBufferAdd)
		delete m_pBufferAdd;
	int totalbits= m_szHeaderMark.GetLength()*8+1;

	m_pBufferAdd= new char[totalbits+MARGEN_ERROR];
	m_pBmpModificado->ResetNuevo();

	// obtenemos la marca de agua
	if(!m_pBufferAdd || !m_pBmpModificado->GetBytes(m_pBufferAdd, totalbits, m_nTipo, m_pBmpNormal)) 
		return false;

	int bits=0;

	// comprobamos la marca de agua
	for( int i=0; i<m_szHeaderMark.GetLength(); i++) {
		if( m_szHeaderMark[i]!= GetByte(m_pBufferAdd, bits)) {// no corresponde la marca de agua
			return false;
		}
	}
	if( m_pBufferAdd)
		delete m_pBufferAdd;
	// obtenemos el tamano del archivo guardado y el tamano del archivo en bits
	totalbits+= sizeof(int)*16;
	m_pBufferAdd= new char[totalbits+MARGEN_ERROR];

	if( !m_pBufferAdd || !m_pBmpModificado->GetBytes(m_pBufferAdd, totalbits, m_nTipo, m_pBmpNormal))
		return false;

	// obtenemos el tamaño del archivo
	tamano=GetInt(m_pBufferAdd, bits);
	if( tamano<=0)
		return false;
	// obtenemos el tamaño del nombre archivo
	int sizeoffile=GetInt(m_pBufferAdd, bits);

	if(sizeoffile<=0)
		return false;

	if( m_pBufferAdd)
		delete m_pBufferAdd;
	
	totalbits+= (8*tamano)+(8*sizeoffile);

	m_pBufferAdd= new char[totalbits+MARGEN_ERROR];
	
	if( !m_pBufferAdd || !m_pBmpModificado->GetBytes(m_pBufferAdd, totalbits, m_nTipo, m_pBmpNormal))
		return false;

	//// tamano del archivo en bytes
	//AddInt(m_pBufferAdd, bits, tamano);
	//ASSERT(bits%8==0);
	//// tamano del nombre del archivo en bytes
	//AddInt(m_pBufferAdd, bits, nIndex);
	//ASSERT(bits%8==0);
	//// nombre del archivo
	//for( int i=0; i<file.GetLength(); i++) 
	//	AddByte(m_pBufferAdd, bits, file[i]);
	//ASSERT(bits%8==0);
	//// contenido del archivo
	//for( int i=0; i<tamano; i++) 
	//	AddByte(m_pBufferAdd, bits, buffer[i]);

	/*if(!m_pBmpModificado->GetBytes(m_pBufferAdd, m_pBmpModificado->GetPixeles(), m_nTipo,m_pBmpNormal))
		return false;*/
    
	CString nombreArchivo;

	for( int i=0; i<sizeoffile; i++) 
		nombreArchivo+=GetByte(m_pBufferAdd, bits);
	if( archivo[archivo.GetLength()-1]!='\\')
		archivo+="\\";
	archivo+=nombreArchivo;

	ASSERT(tamano>0);
	
	unsigned char *resultado= new unsigned char [tamano];
	ASSERT(resultado);

	for( int i=0; i<tamano; i++) // tamaño del archivo en bytes
		resultado[i]=GetByte(m_pBufferAdd, bits);

	CFile temporal;
	if(!temporal.Open(archivo, CFile::modeWrite | CFile::modeCreate)) {
		ASSERT(FALSE);
		return false;
	}
	temporal.Write(resultado, tamano);
	temporal.Close();
	if(m_pBufferAdd)
		delete m_pBufferAdd;
	m_pBufferAdd=NULL;
	return true;
}
