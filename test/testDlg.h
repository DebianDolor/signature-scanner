#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector;
using std::filesystem::recursive_directory_iterator;


class CtestDlg : public CDialogEx
{
public:
	CtestDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

	BOOL PreTranslateMessage(MSG* pMsg);

	bool validateQueryString(CString& temp);
	bool validatePath(CString& temp);

	void LockControls();
	void UnlockControls();
	void resetList();

	static UINT getFiles(LPVOID param);
	void compare(CString& path, CString& source);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedStart();

	int m_count;

	string strPath;
	CString m_path;
	CString m_strQuery;
	CString m_temp;
	CListBox m_listBox;
	CListBox m_res;
};