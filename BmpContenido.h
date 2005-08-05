#pragma once

# define HEADER_SIZE 54
# include "parser.h"

# define	WM_BITMAP	WM_USER+400

class CBmpContenido
{
	CString		m_szBitmapPath;
	BYTE		m_bHeader[HEADER_SIZE+1];
	BYTE		*m_lpbContenido;
	BYTE		*m_lpbNuevo;
	char		*m_pBitsPorPixel;
	int			m_nPixeles;
	int			m_nBitsDisponibles;
	int			BuildTable();
	int			m_nOffset;
	bool		m_bPar;
	bool		m_bMarcar;
	bool			m_bLBT;
	CString			m_szPassword;
	CWnd			*m_pParent;
	inline COLORREF	GetPixel(int index);
	inline void		GetPixel(int &r, int &g, int &b, int index);
	inline void		SetPixel(int index, COLORREF color);
	inline COLORREF	GetPixelD(int index);
	inline void		GetPixelD(int &r, int &g, int &b, int index);
	inline void		SetPixelD(int index, COLORREF color);
	void		AddBytesSimple(const char *bytes, int tamano);
	bool		GetBytesSimple(char *bytes, int tamano);
	void		AddBytesMetodo(const char *bytes, int tamano);
	bool		GetBytesMetodo(char *bytes, int tamano, CBmpContenido *original);
	bool		GetBytesParoIm(char *bytes, int tamano, CBmpContenido *original);
	void		AddBytesParoIm(const char *bytes, int tamano);
	void		AddBytesEcuacion(const char *bytes, int tamano);
	bool		GetBytesEcuacion(char *bytes, int tamano, CBmpContenido *original);
	inline int			GetNextPixel(int pos);
	inline int			GetNextPixelEcuacion(int pos);
	inline int			GetNextPixelMetodo(int pos);
	CSize		m_szTamano;
	CString		m_szEcuacion;
	CParser		*m_pParser;
public:
	CBmpContenido(void);
	~CBmpContenido(void);
	void			SetParent(CWnd *parent);
	CSize		GetBitmaptamano();
	void		SetMarcar(bool marcar);
	bool		SetBitmap(CString bitmap);
	int			GetPixeles();
	void		SetEcuacion(CString ecuacion);
	int			GetBitsDisponibles();
	BYTE		*GetHeader();
	BYTE		*GetContenido();
	BYTE		*GetNuevo();
	void		ResetNuevo();
	void		AddBytes(const char *bytes, int tamano, int metodo);
	bool		GetBytes(char *bytes, int tamano, int metodo, CBmpContenido *original);
	int			GetBitsNecesarios(int tipo, CBmpContenido *original);
	void		SetParametrosTipo2( int offset, bool par);
	void		SetPassword(CString password);
	void			SetLBT(bool use);
};
