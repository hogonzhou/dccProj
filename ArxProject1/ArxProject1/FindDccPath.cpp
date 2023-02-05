// FindDccPath.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FindDccPath.h"
#include "someFunc.h"

// CFindDccPath �Ի���

IMPLEMENT_DYNAMIC(CFindDccPath, CAcUiDialog)

CFindDccPath::CFindDccPath(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CFindDccPath::IDD, pParent)
{

}

CFindDccPath::~CFindDccPath()
{
}

void CFindDccPath::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITDccPath, m_editCtrlDccPath);
}


BEGIN_MESSAGE_MAP(CFindDccPath, CAcUiDialog)
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
	//����û���ѡ����ļ����Ƿ�ΪDCC.exe
	if(isItDccExeFile(savePath) == false)
	{
		MessageBox(TEXT("��⵽��ѡ�ļ����ֲ���DCC.exe"),TEXT("DCC·������"));
	}
}

void CFindDccPath::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����Dcc exe�ļ���·����json.
	CString dccExePath;
	m_editCtrlDccPath.GetWindowText(dccExePath);
	SomeFunction::saveDccExeFullPathToJson(dccExePath);
	//����Ƿ�ΪDCC.exe�ļ�
	if(isItDccExeFile(dccExePath) == false)
	{
		MessageBox(TEXT("��⵽��ѡ�ļ����ֲ���DCC.exe"),TEXT("DCC·������"));
		return;
	}
	//����output�ļ���·��;
	CString csDccOutputDir;
	if(createDccOutputFileDirPath(dccExePath,csDccOutputDir) == false)
	{
		return;
	}
	//����text����·����json.
	const CString outputFilename = "DCC_INFO_INPUT.txt";
	csDccOutputDir += outputFilename;
	SomeFunction::saveTextFullPathToJson(csDccOutputDir);
	CAcUiDialog::OnOK();
}

bool CFindDccPath::isItDccExeFile(const CString csDccPath)
{
	//����ȡ�õ�dcc·���Ƿ�Ϸ�����׼���ļ����Ƿ�Ϊ"DCC.exe";
	const CString exeAffixName = TEXT("DCC.exe");

	if(csDccPath.IsEmpty() == true)
	{
		return false;
	}
	int csLen = csDccPath.GetLength();
	int startPos = csDccPath.ReverseFind('\\');
	CString  csName;
	csName = csDccPath.Mid(startPos+1,csLen-1);
	if(csName.CompareNoCase(exeAffixName))
	{
		return false;
	}
	else
	{
		return true; //�ж�ΪDcc.exe�ļ�,�����ִ�Сд
	}
}

bool CFindDccPath::createDccOutputFileDirPath(const CString dccPathName,CString& outputTextFilenameAndPath)
{
	//ǰ����dccPathName����ȷ��DCC.exe��·��.
	const CString outputFristDir = "\\�ߴ������㼰�������V12.5.0";
	const CString outputSecondDir = "\\output";
	const CString outputThreeDir = "\\DCC_INFO_INPUT\\";
	CString strTemp;
	int firstSlopePos = dccPathName.ReverseFind('\\');
	if(firstSlopePos == -1)
	{
		return false;
	}
	else
	{
		strTemp = dccPathName.Mid(0,firstSlopePos);
		int secondSlopePos = strTemp.ReverseFind('\\');
		if(secondSlopePos == -1)
		{
			return false;
		}
		strTemp = strTemp.Mid(0,secondSlopePos);

		outputTextFilenameAndPath.Empty();
		//������һ��Ŀ¼		
		outputTextFilenameAndPath = strTemp  + outputSecondDir;
		if(SomeFunction::createDir(outputTextFilenameAndPath))
		{
			outputTextFilenameAndPath = outputTextFilenameAndPath + outputThreeDir;
			if(SomeFunction::createDir(outputTextFilenameAndPath) == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		
	}
}


