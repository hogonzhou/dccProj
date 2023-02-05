// FindDccPath.cpp : 实现文件
//

#include "stdafx.h"
#include "FindDccPath.h"
#include "someFunc.h"

// CFindDccPath 对话框

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


// CFindDccPath 消息处理程序

BOOL CFindDccPath::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//从json获取dcc路径显示在edit框内;
	//设置为只读，不允许改动.
	m_editCtrlDccPath.SetReadOnly(TRUE);
	//填充edit ctrl
	std::string str = SomeFunction::getDccExeFullPath();
	CString pathFromJson = str.c_str();
	m_editCtrlDccPath.SetWindowText(pathFromJson);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFindDccPath::OnBnClickedBtnselectdccpath()
{
	// TODO: 在此添加控件通知处理程序代码
	//用户选择dcc执行文件所在路径(包含文件名).
	//用户选择后显示于edit窗口，并不存入json.
	//user select file path;
	CString filterStr = _T("Describe Files (*.exe)|*.exe|All Files (*.*)|*.*||");
	CString savePath = SomeFunction::selectFilePathByUser(filterStr);
	if(savePath.IsEmpty() == false)
	{
		//将字符串中的'\'替换成'/'.
		//填充（刷新）edit ctrl
		m_editCtrlDccPath.SetWindowText(savePath);
		//test:save to json;
		//SomeFunction::saveTextFullPathToJson(savePath);
	}
	//检测用户所选择的文件名是否为DCC.exe
	if(isItDccExeFile(savePath) == false)
	{
		MessageBox(TEXT("检测到所选文件名字不是DCC.exe"),TEXT("DCC路径设置"));
	}
}

void CFindDccPath::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//保存Dcc exe文件的路径到json.
	CString dccExePath;
	m_editCtrlDccPath.GetWindowText(dccExePath);
	SomeFunction::saveDccExeFullPathToJson(dccExePath);
	//检测是否为DCC.exe文件
	if(isItDccExeFile(dccExePath) == false)
	{
		MessageBox(TEXT("检测到所选文件名字不是DCC.exe"),TEXT("DCC路径设置"));
		return;
	}
	//给出output文件夹路径;
	CString csDccOutputDir;
	if(createDccOutputFileDirPath(dccExePath,csDccOutputDir) == false)
	{
		return;
	}
	//保存text保存路径到json.
	const CString outputFilename = "DCC_INFO_INPUT.txt";
	csDccOutputDir += outputFilename;
	SomeFunction::saveTextFullPathToJson(csDccOutputDir);
	CAcUiDialog::OnOK();
}

bool CFindDccPath::isItDccExeFile(const CString csDccPath)
{
	//分析取得的dcc路径是否合法，标准是文件名是否为"DCC.exe";
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
		return true; //判定为Dcc.exe文件,不区分大小写
	}
}

bool CFindDccPath::createDccOutputFileDirPath(const CString dccPathName,CString& outputTextFilenameAndPath)
{
	//前提是dccPathName是正确的DCC.exe的路径.
	const CString outputFristDir = "\\尺寸链计算及公差分析V12.5.0";
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
		//建立第一级目录		
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


