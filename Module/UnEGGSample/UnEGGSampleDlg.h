
// UnEGGSampleDlg.h : header file
//

#pragma once

#include "UnEGGObject.h"

const int WM_SETPROG = WM_USER + 0x10;
const int WM_QUERYPASSWORD = WM_USER + 0x11;

// UnEGGSampleDlg dialog
class UnEGGSampleDlg : public CDialog
{
// Construction
public:
	UnEGGSampleDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~UnEGGSampleDlg(void);

// Dialog Data
	enum { IDD = IDD_UNEGGSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnDestroy();
    afx_msg LRESULT OnSetProgress(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnQueryPassword(WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    UnEGGModule module_;
    CProgressCtrl progTotal_;
    CProgressCtrl progCurrent_;
    CListCtrl list_;
    UnEGGObject obj_;
};
