#pragma once
#include "resource.h"
#include "acutads.h"

// DccMainDlg �Ի���

class DccMainDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(DccMainDlg)

public:
	DccMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DccMainDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };
	void OnBnClickedButtonOk();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CButton m_buttonOk;
};
