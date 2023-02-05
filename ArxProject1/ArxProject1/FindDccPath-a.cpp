// FindDccPath.cpp : 实现文件
//

#include "stdafx.h"
#include "FindDccPath.h"
#include "someFunc.h"

// CFindDccPath 对话框

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
}

void CFindDccPath::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//保存Dcc exe文件的路径到json.
	CString dccExePath;
	m_editCtrlDccPath.GetWindowText(dccExePath);
	SomeFunction::saveDccExeFullPathToJson(dccExePath);
	CAcUiDialog::OnOK();
}
