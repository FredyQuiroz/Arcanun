#pragma once
#include "Separator.h"

class __declspec(dllexport) CStaticSeparator : public CStatic
{
// Construction
public:
	CStaticSeparator();
    virtual ~CStaticSeparator();

    void OnUpdateContents();

protected:
    CFont m_font;
    CSeparator m_ctrlSeparator;
    int m_nOldWidth;

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnSize( UINT nType, int cx, int cy );
    afx_msg void OnMove(int x, int y);
    afx_msg void OnDestroy();
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
protected:
    virtual void PreSubclassWindow();
};
