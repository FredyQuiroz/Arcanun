#pragma once

#include "Separator.h"
#include <set>
using namespace std;

class __declspec(dllexport) CCheckBoxSeparator : public CButton
{
public:
	CCheckBoxSeparator();
    virtual ~CCheckBoxSeparator();

    void SetNextSectionCtrl(CWnd& wnd);
    void SetNextSectionId(int nNextId);

    void AddControlledDlgItem(int);
    void UpdateControlledDlgItems();
	void SetEnable(bool enable=true);
protected:
    CFont m_font;
    CSeparator m_ctrlSeparator;
	bool		m_bEnable;
    set<int> m_ControlledIDs;

    void OnUpdateContents();

public:
    DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnClicked();
    afx_msg void OnMove(int x, int y);
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
protected:
    virtual void PreSubclassWindow();
};
