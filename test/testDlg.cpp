#include "pch.h"
#include "framework.h"
#include "test.h"
#include "testDlg.h"
#include "afxdialogex.h"
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CtestDlg::CtestDlg(CWnd* pParent)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
	, m_path(_T(""))
	, m_strQuery(_T(""))
	, m_count(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listBox);
	DDX_Text(pDX, IDC_EDIT_PATH, m_path);
	DDX_Text(pDX, IDC_EDIT_QUERY, m_strQuery);
	DDX_Control(pDX, IDC_SHOW_RES, m_res);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECT, &CtestDlg::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_START, &CtestDlg::OnBnClickedStart)
END_MESSAGE_MAP()


BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);


	return TRUE;
}


void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CtestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}


void CtestDlg::LockControls() {
	auto pPath = GetDlgItem(IDC_EDIT_PATH);
	auto pQuery = GetDlgItem(IDC_EDIT_QUERY);
	auto pStart = GetDlgItem(IDC_START);
	auto pSelect = GetDlgItem(IDC_SELECT);
	pPath->EnableWindow(FALSE);
	pQuery->EnableWindow(FALSE);
	pStart->EnableWindow(FALSE);
	pSelect->EnableWindow(FALSE);
}


void CtestDlg::UnlockControls() {
	auto pPath = GetDlgItem(IDC_EDIT_PATH);
	auto pQuery = GetDlgItem(IDC_EDIT_QUERY);
	auto pStart = GetDlgItem(IDC_START);
	auto pSelect = GetDlgItem(IDC_SELECT);
	pPath->EnableWindow();
	pQuery->EnableWindow();
	pStart->EnableWindow();
	pSelect->EnableWindow();
}


void CtestDlg::OnBnClickedSelect()
{
	UpdateData();
	CFolderPickerDialog dlgFolder;

	if (dlgFolder.DoModal() == IDC_START)
	{
		CString strFolder = dlgFolder.GetPathName();
		AfxMessageBox(strFolder);
	}
	m_path = dlgFolder.GetPathName();
	UpdateData(FALSE);
}


void CtestDlg::resetList() {
	int numOfItems = m_listBox.GetCount();

	for (int i = numOfItems; i >= 0; i--) {
		m_listBox.DeleteString(i);
	}

	int n = m_res.GetCount();

	for (int i = n; i >= 0; i--) {
		m_res.DeleteString(i);
	}

	m_count = 0;
}


void CtestDlg::OnBnClickedStart()
{
	resetList();

	if (validateQueryString(m_strQuery) == true && validatePath(m_path) == true) {

		AfxBeginThread(getFiles, this);
	}
}


bool CtestDlg::validateQueryString(CString& temp) {
	UpdateData();

	CT2CA pszConvertedAnsiString(temp);

	std::string s(pszConvertedAnsiString);

	const char* c = s.c_str();

	if (*c == NULL) {
		AfxMessageBox(_T("Signature sai định dạng (xâu rỗng)"));
		return false;
	}
	else if (strlen(c) & 1) {
		AfxMessageBox(_T("Signature sai định dạng (số kí tự hex không chẵn)"));
		return false;
	}
	else {
		if (c[strspn(c, "0123456789abcdefABCDEF")] != 0)
		{
			AfxMessageBox(_T("Signature sai định dạng (chứa kí tự không hợp lệ)"));
			return false;
		}
	}

	UpdateData(FALSE);

	return true;
}


bool CtestDlg::validatePath(CString& temp) {
	UpdateData();

	if (temp.IsEmpty()) {
		AfxMessageBox(_T("Không tìm thấy thư mục cần quét"));
		return false;
	}
	CFileFind finder;
	DWORD attrib = GetFileAttributes(temp);

	if (!finder.FindFile(m_path)) {
		AfxMessageBox(_T("Không tìm thấy thư mục cần quét"));
		return false;
	}

	if ((attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
		AfxMessageBox(_T("Không tìm thấy thư mục cần quét"));
		return false;
	}

	return true;
}


// get all files in a directory and check each file
UINT CtestDlg::getFiles(LPVOID param) {

	CtestDlg* wnd = reinterpret_cast<CtestDlg*>(param);
	std::wstring path = CStringW(wnd->m_path);
	wnd->LockControls();


	CString t = CTime::GetCurrentTime().Format("%I:%M:%S %p %d/%m/%Y");
	wnd->m_listBox.AddString(_T("Chương trình bắt đầu quét từ ") + t + _T(", tại thư mục ") + wnd->m_path);

	for (const auto& file : recursive_directory_iterator(path)) {

		CString temp = file.path().c_str();

		if (temp.GetLength() >= 260) {
			AfxMessageBox(_T("Đường dẫn tới file quá dài, không hợp lệ!"));
			return 1;
		}
		else {

			DWORD attrib = GetFileAttributes((file.path()).c_str());
			if ((attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				wnd->m_temp = (file.path()).c_str();
				wnd->compare(wnd->m_temp, wnd->m_strQuery);
			}
		}
	}

	std::string s = std::to_string(wnd->m_count);
	wnd->m_temp = _T("Chương trình đã quét xong, tìm thấy ");
	wnd->m_temp += s.c_str();
	wnd->m_temp += _T(" file chứa signature!");
	AfxMessageBox(wnd->m_temp);

	t = CTime::GetCurrentTime().Format("%I:%M:%S %p %d/%m/%Y");
	wnd->m_listBox.AddString(_T("Chương trình đã quét xong vào ") + t + _T(", tìm thấy ") + s.c_str() + _T(" file chứa signature!"));

	wnd->UnlockControls();
	
	return 0;
}


// check if the given file contains the user's hex input or not
void CtestDlg::compare(CString& path, CString& s) {

	
	std::wstring dest_path = CStringW(path);

	std::ifstream file(dest_path, std::ios::binary);
	if (file.is_open()) {

		file.seekg(0, std::ios::end);
		int iFileSize = static_cast<int>(file.tellg());

		if (iFileSize > 50000000) {
			m_listBox.AddString(_T("[!] File ") + path + _T(" có kích thước quá lớn, bỏ qua!"));
			return;
		}

		file.seekg(0, std::ios::beg);
		std::vector<char> memblock(iFileSize);
		file.read(((char*)memblock.data()), iFileSize);

		std::ostringstream ostrData;

		for (int i = 0; i < memblock.size(); i++) {
			int z = memblock[i] & 0xff;
			ostrData << std::hex << std::setfill('0') << std::setw(2) << z;
		}

		string strDataHex = ostrData.str();
		string source = (CT2A)s;
		if (strDataHex.find(source) != string::npos) {
			m_listBox.AddString(_T("[+] Đã quét xong file ") + path + _T(", tìm thấy signature trong file!"));
			m_count++;
			m_res.AddString(path);
			return;
		}
		else {
			m_listBox.AddString(_T("[-] Đã quét xong file ") + path + _T(", không tìm thấy signature trong file!"));
			return;
		}
	}
}

