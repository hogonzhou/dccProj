// DccMainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DccMainDlg.h"


// DccMainDlg �Ի���

IMPLEMENT_DYNAMIC(DccMainDlg, CAcUiDialog)

DccMainDlg::DccMainDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(DccMainDlg::IDD, pParent)
{

}

DccMainDlg::~DccMainDlg()
{
}

void DccMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_buttonOk);
}


BEGIN_MESSAGE_MAP(DccMainDlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &DccMainDlg::OnBnClickedButtonOk)
END_MESSAGE_MAP()

void DccMainDlg::OnBnClickedButtonOk()
{
	acutPrintf(_T("\n hello..."));
}


// DccMainDlg ��Ϣ�������
