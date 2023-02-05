// FindDccPath.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FindDccPath.h"
#include "someFunc.h"

// CFindDccPath �Ի���

IMPLEMENT_DYNAMIC(CFindDccPath, CDialog)

CFindDccPath::CFindDccPath(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CFindDccPath::IDD, pParent)
{

}

CFindDccPath::~CFindDccPath()
{
}

void CFindDccPath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITDccPath, m_editCtrlDccPath);
}


BEGIN_MESSAGE_MAP(CFindDccPath, CDialog)
	ON_BN_CLICKED(ID_BtnSelectDccPath, &CFindDccPath::OnBnClickedBtnselectdccpath)
	ON_BN_CLICKED(IDOK, &CFindDccPath::OnBnClickedOk)
END_MESSAGE_MAP()


// CFindDccPath ��Ϣ�������

BOOL CFindDccPath::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��json��ȡdcc·����ʾ��edit����;
	//����Ϊֻ����������Ķ�.
	m_editCtrlDccPath.SetReadOnly(TRUE);
	//���edit ctrl
	std::string str = SomeFunction::getDccExeFullPath();
	CString pathFromJson = str.c_str();
	m_editCtrlDccPath.SetWindowText(pathFromJson);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CFindDccPath::OnBnClickedBtnselectdccpath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�û�ѡ��dccִ���ļ�����·��(�����ļ���).
	//�û�ѡ�����ʾ��edit���ڣ���������json.
	//user select file path;
	CString filterStr = _T("Describe Files (*.exe)|*.exe|All Files (*.*)|*.*||");
	CString savePath = SomeFunction::selectFilePathByUser(filterStr);
	if(savePath.IsEmpty() == false)
	{
		//���ַ����е�'\'�滻��'/'.
		//��䣨ˢ�£�edit ctrl
		m_editCtrlDccPath.SetWindowText(savePath);
		//test:save to json;
		//SomeFunction::saveTextFullPathToJson(savePath);
	}
}

void CFindDccPath::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����Dcc exe�ļ���·����json.
	CString dccExePath;
	m_editCtrlDccPath.GetWindowText(dccExePath);
	SomeFunction::saveDccExeFullPathToJson(dccExePath);
	CAcUiDialog::OnOK();
}
