//John Melas
//jmelas@cc.ece.ntua.gr

#include "stdafx.h"
#include "BitmapCtrl.h"
#include <math.h>

// helper function prototypes
static BITMAPINFO *PrepareRGBBitmapInfo(WORD wWidth, WORD wHeight);
static void ShrinkData(BYTE *pInBuff, WORD wWidth, WORD wHeight,
				BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight);
static void EnlargeData(BYTE *pInBuff, WORD wWidth, WORD wHeight,
				 BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight);

///////////////////////////////////////////////////////////
// Main resize function

HBITMAP ScaleBitmap(HBITMAP hBmp, WORD wNewWidth, WORD wNewHeight)
{
 BITMAP bmp;
 ::GetObject(hBmp, sizeof(BITMAP), &bmp);

 // check for valid size
 if ((bmp.bmWidth > wNewWidth && bmp.bmHeight < wNewHeight) || (bmp.bmWidth < wNewWidth && bmp.bmHeight > wNewHeight))
  return NULL;

 HDC hDC = ::GetDC(NULL);
 BITMAPINFO *pbi = PrepareRGBBitmapInfo((WORD)bmp.bmWidth, 
                                        (WORD)bmp.bmHeight);

 BYTE *pData = new BYTE[pbi->bmiHeader.biSizeImage];

 ::GetDIBits(hDC, 
             hBmp, 
             0, 
             bmp.bmHeight, 
             pData, 
             pbi, 
             DIB_RGB_COLORS);

 delete pbi;
 pbi = PrepareRGBBitmapInfo(wNewWidth, wNewHeight);
 BYTE *pData2 = new BYTE[pbi->bmiHeader.biSizeImage];

 if(bmp.bmWidth >= wNewWidth 
 && bmp.bmHeight >= wNewHeight)
  ShrinkData(pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight,
  pData2, wNewWidth, wNewHeight);
 else
  EnlargeData(pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight,
  pData2, wNewWidth, wNewHeight);
 
 delete pData;

 HBITMAP hResBmp = ::CreateCompatibleBitmap(hDC, 
                                            wNewWidth, 
                                            wNewHeight);

 ::SetDIBits(hDC, 
             hResBmp, 
             0, 
             wNewHeight, 
             pData2, 
             pbi, 
             DIB_RGB_COLORS);

 ::ReleaseDC(NULL, hDC);

 delete pbi;
 delete pData2;

 return hResBmp;
}

///////////////////////////////////////////////////////////

BITMAPINFO *PrepareRGBBitmapInfo(WORD wWidth, WORD wHeight)
{
 BITMAPINFO *pRes = new BITMAPINFO;
 ::ZeroMemory(pRes, sizeof(BITMAPINFO));
 pRes->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 pRes->bmiHeader.biWidth = wWidth;
 pRes->bmiHeader.biHeight = wHeight;
 pRes->bmiHeader.biPlanes = 1;
 pRes->bmiHeader.biBitCount = 24;
 pRes->bmiHeader.biSizeImage = ((3 * wWidth + 3) & ~3) 
                                * wHeight;

 return pRes;
}

///////////////////////////////////////////////////////////

static float *CreateCoeff(int nLen, int nNewLen, BOOL bShrink)
{
 int nSum = 0, nSum2;
 float *pRes = new float[2 * nLen];
 float *pCoeff = pRes;
 float fNorm = (bShrink)? (float)nNewLen / nLen : 1;
 int	  nDenom = (bShrink)? nLen : nNewLen;

 ::ZeroMemory(pRes, 2 * nLen * sizeof(float));
 for(int i = 0; i < nLen; i++, pCoeff += 2)
 {
  nSum2 = nSum + nNewLen;
  if(nSum2 > nLen)
  {
   *pCoeff = (float)(nLen - nSum) / nDenom;
   pCoeff[1] = (float)(nSum2 - nLen) / nDenom;
   nSum2 -= nLen;
  }
  else
  {
   *pCoeff = fNorm;
   if(nSum2 == nLen)
   {
    pCoeff[1] = -1;
    nSum2 = 0;
   }
  }
  nSum = nSum2;
 }

 return pRes;
}

///////////////////////////////////////////////////////////

#define F_DELTA		0.0001f

void ShrinkData(BYTE *pInBuff, 
                WORD wWidth, 
                WORD wHeight,
                BYTE *pOutBuff, 
                WORD wNewWidth, 
                WORD wNewHeight)
{
 BYTE  *pLine = pInBuff, *pPix;
 BYTE  *pOutLine = pOutBuff;
 DWORD dwInLn = (3 * wWidth + 3) & ~3;
 DWORD dwOutLn = (3 * wNewWidth + 3) & ~3;
 int   x, y, i, ii;
 BOOL  bCrossRow, bCrossCol;
 float *pRowCoeff = CreateCoeff(wWidth, wNewWidth, TRUE);
 float *pColCoeff = CreateCoeff(wHeight, wNewHeight, TRUE);
 float fTmp, *pXCoeff, *pYCoeff = pColCoeff;
 DWORD dwBuffLn = 3 * wNewWidth * sizeof(float);
 float *fBuff = new float[6 * wNewWidth];
 float *fCurrLn = fBuff, 
       *fCurrPix, 
       *fNextLn = fBuff + 3 * wNewWidth, 
       *fNextPix;

 ::ZeroMemory(fBuff, 2 * dwBuffLn);

 y = 0;
 while(y < wNewHeight)
 {
  pPix = pLine;
  pLine += dwInLn;

  fCurrPix = fCurrLn;
  fNextPix = fNextLn;

  x = 0;
  pXCoeff = pRowCoeff;
  bCrossRow = pYCoeff[1] > F_DELTA;
  while(x < wNewWidth)
  {
   fTmp = *pXCoeff * *pYCoeff;
   for(i = 0; i < 3; i++)
   fCurrPix[i] += fTmp * pPix[i];
   bCrossCol = pXCoeff[1] > F_DELTA;
   if(bCrossCol)
   {
    fTmp = pXCoeff[1] * *pYCoeff;
    for(i = 0, ii = 3; i < 3; i++, ii++)
     fCurrPix[ii] += fTmp * pPix[i];
   }

   if(bCrossRow)
   {
    fTmp = *pXCoeff * pYCoeff[1];
    for(i = 0; i < 3; i++)
     fNextPix[i] += fTmp * pPix[i];
    if(bCrossCol)
    {
     fTmp = pXCoeff[1] * pYCoeff[1];
     for(i = 0, ii = 3; i < 3; i++, ii++)
      fNextPix[ii] += fTmp * pPix[i];
    }
   }

  if(fabs(pXCoeff[1]) > F_DELTA)
  {
   x++;
   fCurrPix += 3;
   fNextPix += 3;
  }

  pXCoeff += 2;
  pPix += 3;
 }

 if(fabs(pYCoeff[1]) > F_DELTA)
 {
  // set result line
  fCurrPix = fCurrLn;
  pPix = pOutLine;
  for(i = 3 * wNewWidth; i > 0; i--, fCurrPix++, pPix++)
  *pPix = (BYTE)*fCurrPix;

  // prepare line buffers
  fCurrPix = fNextLn;
  fNextLn = fCurrLn;
  fCurrLn = fCurrPix;
  ::ZeroMemory(fNextLn, dwBuffLn);

  y++;
  pOutLine += dwOutLn;
  }
  pYCoeff += 2;
 }

 delete [] pRowCoeff;
 delete [] pColCoeff;
 delete [] fBuff;
} 

///////////////////////////////////////////////////////////

void EnlargeData(BYTE *pInBuff, 
                 WORD wWidth, 
                 WORD wHeight,
                 BYTE *pOutBuff, 
                 WORD wNewWidth, 
                 WORD wNewHeight)
{
 BYTE  *pLine = pInBuff, 
       *pPix = pLine, 
       *pPixOld, 
       *pUpPix, 
       *pUpPixOld;
 BYTE  *pOutLine = pOutBuff, *pOutPix;
 DWORD dwInLn = (3 * wWidth + 3) & ~3;
 DWORD dwOutLn = (3 * wNewWidth + 3) & ~3;
 int   x, y, i;
 BOOL  bCrossRow, bCrossCol;
 float *pRowCoeff = CreateCoeff(wNewWidth, wWidth, FALSE);
 float *pColCoeff = CreateCoeff(wNewHeight, wHeight, FALSE);
 float fTmp, fPtTmp[3], *pXCoeff, *pYCoeff = pColCoeff;

 y = 0;
 while(y < wHeight)
 {
  bCrossRow = pYCoeff[1] > F_DELTA;
  x = 0;
  pXCoeff = pRowCoeff;
  pOutPix = pOutLine;
  pOutLine += dwOutLn;
  pUpPix = pLine;
  if(fabs(pYCoeff[1]) > F_DELTA)
  {
   y++;
   pLine += dwInLn;
   pPix = pLine;
  }

  while(x < wWidth)
  {
   bCrossCol = pXCoeff[1] > F_DELTA;
   pUpPixOld = pUpPix;
   pPixOld = pPix;
   if(fabs(pXCoeff[1]) > F_DELTA)
   {
    x++;
    pUpPix += 3;
    pPix += 3;
   }
   fTmp = *pXCoeff * *pYCoeff;
   for(i = 0; i < 3; i++)
    fPtTmp[i] = fTmp * pUpPixOld[i];
   if(bCrossCol)
   {
    fTmp = pXCoeff[1] * *pYCoeff;
    for(i = 0; i < 3; i++)
    fPtTmp[i] += fTmp * pUpPix[i];
   }
   if(bCrossRow)
   {
    fTmp = *pXCoeff * pYCoeff[1];
    for(i = 0; i < 3; i++)
    fPtTmp[i] += fTmp * pPixOld[i];
    if(bCrossCol)
    {
     fTmp = pXCoeff[1] * pYCoeff[1];
     for(i = 0; i < 3; i++)
      fPtTmp[i] += fTmp * pPix[i];
    }
   }
   for(i = 0; i < 3; i++, pOutPix++)
    *pOutPix = (BYTE)fPtTmp[i];
   pXCoeff += 2;
  }
  pYCoeff += 2;
 }

 delete [] pRowCoeff;
 delete [] pColCoeff;
} 

CBitmapCtrl::~CBitmapCtrl()
{
	if(memDC.GetSafeHdc() != NULL)
	{
		memDC.SelectObject(pOldBitmap);
		if(pBitmap != NULL)
			delete pBitmap;
	}
}

BEGIN_MESSAGE_MAP(CBitmapCtrl, CWnd)
	//{{AFX_MSG_MAP(CBitmapCtrl)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()

CBitmapCtrl::CBitmapCtrl() {
	pBitmap=NULL;
}

BOOL CBitmapCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd) 
{
	scrollbar_width = GetSystemMetrics(SM_CXVSCROLL);
	
	// create window
	BOOL ret;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	ret = CWnd::CreateEx(WS_EX_CLIENTEDGE,className,NULL,dwStyle, 
		rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(),0);
	// init virtual screen
	GetClientRect(rcClient);
	CClientDC dc(this);
	if(memDC.GetSafeHdc() == NULL)
	{
		memDC.CreateCompatibleDC(&dc);
		if(pBitmap)
			delete pBitmap;
		if((pBitmap = new CBitmap()) == NULL) return FALSE;
		pBitmap->CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
		pOldBitmap = memDC.SelectObject(pBitmap);
		memDC.SelectStockObject(ANSI_VAR_FONT);
		memDC.SetBkMode(TRANSPARENT);
	}
	x_offset = 0;
	y_offset = 0;
	memDC.FillSolidRect(&rcClient,RGB(128,128,128));

	return ret;
}

void CBitmapCtrl::OnPaint() 
{
	CPaintDC dc(this);
	if(memDC.GetSafeHdc() != NULL)
		dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);
}


void CBitmapCtrl::SetBitmap(HBITMAP hBmp)
{
	if( !hBmp)
		return;
	//reset offsets
	x_offset = 0;
	y_offset = 0;

	//reset scrollbar
	SCROLLINFO si;
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nMin = 0;
	si.nMax = 0;
	si.nPage = 0;
	SetScrollInfo(SB_HORZ, &si, TRUE);
	SetScrollInfo(SB_VERT, &si, TRUE);
	
	//redraw background
	GetClientRect(&rcClient);
	memDC.FillSolidRect(&rcClient,RGB(128,128,128));
		
	//get bitmap handle
	hBitmap = hBmp;

	if (hBitmap == 0)
	{
		memDC.TextOut(0,0,"File not found.");
		Invalidate();
		return;
	}
	
	//get image size
	CPoint point(x_offset,y_offset);
	BITMAP bmpInfo;
	CBitmap::FromHandle(hBitmap)->GetBitmap(&bmpInfo);
	image_size.cx = bmpInfo.bmWidth;
	image_size.cy = bmpInfo.bmHeight;
	
	if( !original_size.cx && !original_size.cy) {
		original_size.cx = bmpInfo.bmWidth;
		original_size.cy = bmpInfo.bmHeight;
	}
	//check image size
	BOOL x_fit;
	BOOL y_fit;
	x_fit = (bmpInfo.bmWidth <= rcClient.Width());
	if (!x_fit)
		rcClient.bottom -= scrollbar_width;
	y_fit = (bmpInfo.bmHeight <= rcClient.Height());
	if (!y_fit)
	{
		rcClient.right -= scrollbar_width;
		x_fit = (bmpInfo.bmWidth <= rcClient.Width());
	}
	if (!x_fit)
	{
		// show scrollbar
		ShowScrollBar(SB_HORZ);
		// update scrollbar
		SCROLLINFO si;
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = 0;
		si.nMax = bmpInfo.bmWidth-1;
		si.nPage = rcClient.Width();
		SetScrollInfo(SB_HORZ, &si, TRUE);
	}
	if (!y_fit)
	{
		// show scrollbar
		ShowScrollBar(SB_VERT);
		// update scrollbar
		SCROLLINFO si;
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = 0;
		si.nMax = bmpInfo.bmHeight-1;
		si.nPage = rcClient.Height();
		SetScrollInfo(SB_VERT, &si, TRUE);
	}
	
	DrawBitmap();
}

void CBitmapCtrl::Resize(int porcentaje) {

	SetBitmap(ScaleBitmap(m_hBitmapOriginal, original_size.cx*porcentaje/100, original_size.cy*porcentaje/100));
}

void CBitmapCtrl::Resize(int x, int y) {
	
	SetBitmap(ScaleBitmap(m_hBitmapOriginal, x, y));
}

void CBitmapCtrl::DrawBitmap()
{
	CPoint point(-x_offset,-y_offset);
	memDC.DrawState(point,image_size,hBitmap,DST_BITMAP|DSS_NORMAL);
	Invalidate();
}

void CBitmapCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nSmall = 1;
	int nLarge = 20;

	if(pScrollBar == GetScrollBarCtrl(SB_HORZ))
	{
		if(nSBCode == SB_LEFT || nSBCode == SB_LINELEFT)
			x_offset -= nSmall;
		if(nSBCode == SB_PAGELEFT)
			x_offset -= nLarge;
		if(nSBCode == SB_RIGHT || nSBCode == SB_LINERIGHT)
			x_offset += nSmall;
		if(nSBCode == SB_PAGERIGHT)
			x_offset += nLarge;
		if(nSBCode == SB_THUMBPOSITION)
			x_offset = (int)nPos;
		if(nSBCode == SB_THUMBTRACK)
			x_offset = (int)nPos;
		if (x_offset<0)
			x_offset = 0;
		if (x_offset>image_size.cx - rcClient.Width())
			x_offset = image_size.cx - rcClient.Width();
		SetScrollPos(SB_HORZ, x_offset, TRUE);
		DrawBitmap();
	}
}

void CBitmapCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nSmall = 1;
	int nLarge = 20;

	if(pScrollBar == GetScrollBarCtrl(SB_VERT))
	{
		if(nSBCode == SB_TOP || nSBCode == SB_LINEUP)
			y_offset -= nSmall;
		if(nSBCode == SB_PAGEUP)
			y_offset -= nLarge;
		if(nSBCode == SB_BOTTOM || nSBCode == SB_LINEDOWN)
			y_offset += nSmall;
		if(nSBCode == SB_PAGEDOWN)
			y_offset += nLarge;
		if(nSBCode == SB_THUMBPOSITION)
			y_offset = (int)nPos;
		if(nSBCode == SB_THUMBTRACK)
			y_offset = (int)nPos;
		if (y_offset<0)
			y_offset = 0;
		if (y_offset>image_size.cy - rcClient.Height())
			y_offset = image_size.cy - rcClient.Height();
		SetScrollPos(SB_VERT, y_offset, TRUE);
		DrawBitmap();
	}
}

void CBitmapCtrl::LoadFile(CString filename)
{
	original_size= CSize(0);
	m_hBitmapOriginal = (HBITMAP) LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	SetBitmap(m_hBitmapOriginal);
}


void CBitmapCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if( ::IsWindow(*this)) {
        
		GetClientRect(rcClient);
		CClientDC dc(this);
		memDC.DeleteDC();

		if(memDC.GetSafeHdc() == NULL)
		{
			memDC.CreateCompatibleDC(&dc);
			if(pBitmap)
				delete pBitmap;
			if((pBitmap = new CBitmap()) == NULL) return;
			pBitmap->CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
			pOldBitmap = memDC.SelectObject(pBitmap);
			memDC.SelectStockObject(ANSI_VAR_FONT);
			memDC.SetBkMode(TRANSPARENT);
		}
		x_offset = 0;
		y_offset = 0;
		memDC.FillSolidRect(&rcClient,RGB(128,128,128));
	}
}
