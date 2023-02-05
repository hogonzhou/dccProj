#pragma once
#include "resource.h"
#include "acutads.h"

// DccMainDlg 对话框

class DccMainDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(DccMainDlg)

public:
	DccMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DccMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };
	void OnBnClickedButtonOk();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CButton m_buttonOk;
};
