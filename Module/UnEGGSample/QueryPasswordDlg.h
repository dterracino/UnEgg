#pragma once


// QueryPasswordDlg dialog

class QueryPasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(QueryPasswordDlg)

public:
	QueryPasswordDlg(LPCTSTR fileName, CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryPasswordDlg();

    inline CString GetPassword(void) {return password_;}
    inline UINT GetResult(void) {return res_;}

// Dialog Data
	enum { IDD = IDD_QUERYPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedBtnSkip();
    afx_msg void OnClose();

public:
    virtual BOOL OnInitDialog();

public:
    CString fileName_;
    CString password_;
    UINT res_; // 0 : ok, 1 : skip, 2 : cancel
};
