// QueryPasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnEGGSample.h"
#include "QueryPasswordDlg.h"


// QueryPasswordDlg dialog

IMPLEMENT_DYNAMIC(QueryPasswordDlg, CDialog)

QueryPasswordDlg::QueryPasswordDlg(LPCTSTR fileName, CWnd* pParent /*=NULL*/)
	: CDialog(QueryPasswordDlg::IDD, pParent)
    , password_(_T(""))
    , res_(0)
    , fileName_(fileName)
{

}

QueryPasswordDlg::~QueryPasswordDlg()
{
}

void QueryPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, password_);
	DDV_MaxChars(pDX, password_, 250);
    DDX_Text(pDX, IDC_STATIC_FILENAME, fileName_);
}


BEGIN_MESSAGE_MAP(QueryPasswordDlg, CDialog)
    ON_BN_CLICKED(IDOK, &QueryPasswordDlg::OnBnClickedOk)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BTN_SKIP, &QueryPasswordDlg::OnBnClickedBtnSkip)
    ON_BN_CLICKED(IDCANCEL, &QueryPasswordDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// QueryPasswordDlg message handlers

BOOL QueryPasswordDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void QueryPasswordDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    UpdateData();

    if (!password_.IsEmpty())
    {
        OnOK();
    }
    else
    {
        CString msg;
        msg.LoadString(IDS_REQUEST_PASSWORD);
        AfxMessageBox(msg, MB_OK | MB_ICONWARNING);
    }
}

void QueryPasswordDlg::OnBnClickedBtnSkip()
{
    // TODO: Add your control notification handler code here

    res_ = 1; // skip
    EndDialog(2);
}

void QueryPasswordDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default

    res_ = 2; // cancel
    CDialog::OnClose();
}
void QueryPasswordDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    res_ = 2; // cancel
    OnCancel();
}
