#pragma once
#include "resource.h"
#include "afxwin.h"
//#include <afxdlgs.h>

// CFindDccPath 对话框

class CFindDccPath : public CAcUiDialog
{
	DECLARE_DYNAMIC(CFindDccPath)

public:
	CFindDccPath(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindDccPath();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnselectdccpath();

// 对话框数据
	enum { IDD = IDD_FindDccPath };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void createDccOutputTextDirectory();
	bool isItDccExeFile(const CString csDccPath);
	bool createDccOutputFileDirPath(const CString dccPathName,CString& outputTextFilenameAndPath);
public:
	virtual BOOL OnInitDialog();
	CEdit m_editCtrlDccPath;
};
