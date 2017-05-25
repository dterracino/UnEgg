
// UnEGGSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnEGGSample.h"
#include "UnEGGSampleDlg.h"
#include "QueryPasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _WIN64 
const LPCTSTR MODULE_FILENAME = _T("UnEGG64.dll");
#else
const LPCTSTR MODULE_FILENAME = _T("UnEGG32.dll");
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// UnEGGSampleDlg dialog




UnEGGSampleDlg::UnEGGSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(UnEGGSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

UnEGGSampleDlg::~UnEGGSampleDlg(void)
{
    module_.Finalize();
}

void UnEGGSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_PROGRESS1, progTotal_);
    DDX_Control(pDX, IDC_PROGRESS2, progCurrent_);
    DDX_Control(pDX, IDC_LIST1, list_);
}

BEGIN_MESSAGE_MAP(UnEGGSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON1, &UnEGGSampleDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &UnEGGSampleDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &UnEGGSampleDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &UnEGGSampleDlg::OnBnClickedButton4)
    ON_WM_DESTROY()
    ON_MESSAGE(WM_SETPROG, &UnEGGSampleDlg::OnSetProgress)
    ON_MESSAGE(WM_QUERYPASSWORD, &UnEGGSampleDlg::OnQueryPassword)
END_MESSAGE_MAP()


// UnEGGSampleDlg message handlers

BOOL UnEGGSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

    if (module_.Initialize(MODULE_FILENAME))
    {
        UnEGGObject::module = &module_;

        list_.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 300);
        list_.InsertColumn(1, _T("Packed Size"), LVCFMT_RIGHT, 150);
        list_.InsertColumn(2, _T("Unpacked Size"), LVCFMT_RIGHT, 150);
    }
    else
    {
        MessageBox(_T("Module Init Error."), NULL, MB_OK);
        PostQuitMessage(0);
    }

    progCurrent_.SetRange(0, 10000);
    progTotal_.SetRange(0, 10000);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void UnEGGSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void UnEGGSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR UnEGGSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void UnEGGSampleDlg::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    CFileDialog dlg(TRUE, _T(".egg"), NULL, 4 | 2, _T("ALZip EGG Files(*.egg)|*.egg|ALZip ALZ Files(*.alz)|*.alz||"));

    if (dlg.DoModal() == IDOK)
    {
        list_.DeleteAllItems();
        obj_.Create();
        obj_.CloseArchive();
        UINT r = obj_.OpenArchive(dlg.GetPathName());
        if (r == EGG_ERROR_SUCCESS)
        {
            for (size_t i = 0; i < obj_.GetFileCount(); ++i)
            {
                EGGFileHeader h;
                obj_.GetFileHeader(i, &h);

                int p = list_.GetItemCount();
                list_.InsertItem(p, h.fileName);

                CString buf;
                _i64tot_s(h.packedSize, buf.GetBuffer(60), 60, 10);
                buf.ReleaseBuffer();
                list_.SetItemText(p, 1, buf);

                _i64tot_s(h.unpackedSize, buf.GetBuffer(60), 60, 10);
                buf.ReleaseBuffer();
                list_.SetItemText(p, 2, buf);
            }
        }
    }
}

void UnEGGSampleDlg::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    obj_.CloseArchive();
    obj_.Destroy();
    list_.DeleteAllItems();
}

void UnEGGSampleDlg::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
    if (obj_.IsOpened())
    {
        TCHAR buf[MAX_PATH];

        BROWSEINFO info = { 0, };
        info.hwndOwner = GetSafeHwnd();
        info.lpszTitle = _T("폴더선택");
        info.pidlRoot = NULL;
        info.pszDisplayName = buf;

        LPITEMIDLIST id = SHBrowseForFolder(&info);
        if (id)
        {
            if (list_.GetSelectedCount())
            {
                std::vector<bool>& ext = obj_.GetExtractingPart();
                ext.resize(list_.GetItemCount());
                std::fill(ext.begin(), ext.end(), false);

                POSITION pos = list_.GetFirstSelectedItemPosition();
                while (pos)
                {
                    ext[(int)pos - 1] = true;
                    list_.GetNextSelectedItem(pos);
                }
            }
            TCHAR path[MAX_PATH];
            SHGetPathFromIDList(id, path);

            IMalloc* malloc = NULL;

            SHGetMalloc(&malloc);
            if (malloc)
            {
                malloc->Free(id);
                malloc->Release();
            }
            obj_.Extract(path);
        }
    }
}

void UnEGGSampleDlg::OnBnClickedButton4()
{
    // TODO: Add your control notification handler code here
    CFileDialog dlg(TRUE);

    if (dlg.DoModal() == IDOK)
    {
        egg_type type = module_.IsValidArchive(dlg.GetFileName());
        switch (type)
        {
        case EGG_TYPE_EGG:
            AfxMessageBox(_T("EGG 파일입니다."));
            break;
        case EGG_TYPE_ALZ:
            AfxMessageBox(_T("ALZ 파일입니다."));
            break;
        case EGG_TYPE_INVALID:
            AfxMessageBox(_T("알 수 없는 파일입니다."));
            break;
        }
    }
}

void UnEGGSampleDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
    obj_.CloseArchive();
    obj_.Destroy();
}

LRESULT UnEGGSampleDlg::OnSetProgress(WPARAM wParam, LPARAM lParam)
{
    progTotal_.SetPos(wParam);
    progCurrent_.SetPos(lParam);
    
    return 1;
}

LRESULT UnEGGSampleDlg::OnQueryPassword(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    PasswordInfo* info = reinterpret_cast<PasswordInfo*>(wParam);
    EGGFileHeader header;
    obj_.GetFileHeader(info->index, &header);

    QueryPasswordDlg dlg(header.fileName, this);
    INT_PTR res = dlg.DoModal();
    if (res == IDOK)
    {
        _tcscpy_s(info->password, MAX_PATH, dlg.GetPassword());
    }

    return dlg.GetResult();
}
