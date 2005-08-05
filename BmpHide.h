#pragma once
#include "bmpcontenido.h"


class CBmpHide
{
	CString			m_szHeaderMark;
	CString			m_szBitmap;
	char			*m_pBufferAdd;
	CBmpContenido	*m_pBmpNormal;
	CBmpContenido	*m_pBmpModificado;
	int				m_nTipo;
	int				m_nOffset;
	bool			m_bPar;
	CString			m_szEcuacion;
	bool			m_bMarcar;
	CWnd			*m_pParent;
	CString			m_szPassword;
	bool			m_bLBT;

	inline void		AddByte(char *bytes, int &pos, int byte);
	inline unsigned char GetByte(char *bytes, int &pos);
	inline void		AddInt(char *bytes, int &pos, int numero);
	inline int		GetInt(char *bytes, int &pos);
public:

	CBmpHide(void);
	~CBmpHide(void);
	void		SetPassword(CString password);
	void		SetParent(CWnd *parent);
	void		SetTipo(int tipo);
	void		SetParametrosTipo2( int offset, bool par);
	int			GetTipo();
	void		SetMarcar(bool marcar);
	bool		SetBitmap(CString bitmap);
	CString		WriteFile(CString File);
	bool		AddFile(CString file);
	bool		ExtractFile(CString bitmap, CString archivo);
	bool		HasAnyHide(CString bitmap);
	void		SetEcuacion(CString ecuacion);
	void			SetLBT(bool use);
};
