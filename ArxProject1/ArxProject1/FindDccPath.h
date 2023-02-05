#pragma once
#include "resource.h"
#include "afxwin.h"
//#include <afxdlgs.h>

// CFindDccPath �Ի���

class CFindDccPath : public CAcUiDialog
{
	DECLARE_DYNAMIC(CFindDccPath)

public:
	CFindDccPath(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindDccPath();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnselectdccpath();

// �Ի�������
	enum { IDD = IDD_FindDccPath };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	void createDccOutputTextDirectory();
	bool isItDccExeFile(const CString csDccPath);
	bool createDccOutputFileDirPath(const CString dccPathName,CString& outputTextFilenameAndPath);
public:
	virtual BOOL OnInitDialog();
	CEdit m_editCtrlDccPath;
};
