
// MFCApplication1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "winioctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEVICE_SEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
#define DEVICE_REC CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_DATA)

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CAboutDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg dialog



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_RADIO1, &CMFCApplication1Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMFCApplication1Dlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMFCApplication1Dlg::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication1Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HANDLE devicehandle = NULL;

void CMFCApplication1Dlg::OnBnClickedButton1()
{
	devicehandle = CreateFile(L"\\\\.\\rootkitlink", GENERIC_ALL, 0,0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (devicehandle == INVALID_HANDLE_VALUE)
		MessageBox(L"Invalid handle value", 0, 0);
}

WCHAR mode = 's';
int index = 0;

void CMFCApplication1Dlg::OnBnClickedButton3()
{
	WCHAR message[30] = { 0 };
	WCHAR returnMessage[1024] = { 0 };
	ULONG returnLength = 0;
	ULONG returnLength1 = 0;
	message[0] = mode;
	LPTSTR lpWord = new TCHAR[30];
	memset(lpWord, '\0', 30 * sizeof(TCHAR));
	GetDlgItemText(IDC_EDIT1, lpWord, 30);
	index = _ttoi(lpWord);
	if (index > 0x191)
		index = 0x191;
	_itow_s(index, message + 1, 29, 10);

	if (devicehandle != INVALID_HANDLE_VALUE && devicehandle != NULL) {
		if (DeviceIoControl(devicehandle, DEVICE_SEND, message, 30, NULL, 0, &returnLength, 0)) {
			if (DeviceIoControl(devicehandle, DEVICE_REC, NULL, 0, returnMessage, 1024, &returnLength1, 0)) {
				MessageBox(returnMessage, 0, 0);
			}
			else {
				MessageBox(L"error", 0, 0);
				return;
			}
		}
		else {
			MessageBox(L"error", 0, 0);
			return;
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	mode = 's';
}


void CMFCApplication1Dlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	mode = 'g';
}

void CMFCApplication1Dlg::OnBnClickedRadio3() {
	mode = 'd';
}

void CAboutDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
