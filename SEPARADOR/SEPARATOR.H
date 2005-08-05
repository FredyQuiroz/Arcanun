#pragma once

class __declspec(dllexport) CSeparator : public CStatic
{
// Construction
public:
	CSeparator();
	virtual ~CSeparator();

    void Create(DWORD dwStyle, CRect rc, CWnd* pParent, int nID = 0xffff);

protected:
    virtual void Print(CDC* pDC, DWORD dwFlags);

public:
    DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
    afx_msg LRESULT OnPrint(WPARAM, LPARAM);
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void PreSubclassWindow();
};
