// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- GridDlg.cpp : Implementation of CGridDlg
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "GridDlg.h"
#include "GridCtrlUtil.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CGridDlg, CAcUiDialog)

BEGIN_MESSAGE_MAP(CGridDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BTNPickDim, &CGridDlg::OnBnClickedBtnpickdim)
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_BTNDeleteRcds, &CGridDlg::OnBnClickedBtndeleteRcds)
	ON_BN_CLICKED(IDC_BTNSelectCurDwg, &CGridDlg::OnBnClickedBtnSelectCurDwgAll)
	ON_BN_CLICKED(IDC_BTNSwitchTolRcd, &CGridDlg::OnBtnSwitchTolClicked)
	ON_BN_CLICKED(IDC_BtnSelTxtSavePath, &CGridDlg::OnSelTxtSavePathBnClicked)
	ON_BN_CLICKED(IDC_BtnSaveTextPathToJson, &CGridDlg::OnBnClickedBtnSavePathToJsonAndText)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CGridDlg::CGridDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CGridDlg::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CGridDlg::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX,IDC_GRID,m_grid);
	DDX_Control(pDX, IDC_BTNPickDim, m_btnPickDim);
	DDX_Control(pDX, IDC_BTNSwitchTolRcd, m_btnFcfRcdSwitch);
	DDX_Control(pDX, IDC_EDITTextFilePath, m_editCtrlTextFilePath);
	DDX_Control(pDX, IDC_BtnSaveTextPathToJson, m_saveTextBtn);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CGridDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

std::vector<CString> CGridDlg::GetAllColumnName()
{
	//
	std::vector<CString>  colNames;
	colNames.push_back(TEXT("序号"));
	colNames.push_back(TEXT("图例"));
	colNames.push_back(TEXT("设备及材料名称"));
	colNames.push_back(TEXT("规格及型号"));
	colNames.push_back(TEXT("单位"));
	colNames.push_back(TEXT("数量"));
	colNames.push_back(TEXT("备注"));
	return colNames;
}
void CGridDlg::UpdateOutColGrid()
{
	//表格更新.
	int colCnt = 0;
	m_gridContManger.getColumnCount(colCnt);

	int iCol = 0;
	int rowCnt = 0;
	const vector<StGridToleranceRecord>& rcdVec = m_gridContManger.rtnAllRcds();
	rowCnt = rcdVec.size();
	m_grid.SetRowCount(rowCnt+1);

	vector<StGridToleranceRecord>::const_iterator itr = rcdVec.begin();
	for(int iRow = 0; iRow < rowCnt; iRow++)
	{
		iCol = 0;
		m_grid.SetItemText(iRow+1,iCol,itr->m_strIndex);    //0
		m_grid.SetItemText(iRow+1,++iCol,itr->m_strMeasure);//1
		m_grid.SetItemText(iRow+1,++iCol,itr->m_tolTop);    //2
		m_grid.SetItemText(iRow+1,++iCol,itr->m_tolBottom); //3
		m_grid.SetItemText(iRow+1,++iCol,itr->dimensionType);//4
		m_grid.SetItemText(iRow+1,++iCol,itr->m_memo);       //5
		m_grid.SetItemText(iRow+1,++iCol,itr->m_filename);   //6
		m_grid.SetItemText(iRow+1,++iCol,itr->m_csHandle);   //7
		itr++;
	}
	//隐藏第六和七列
	m_grid.SetColumnWidth(6,0);
	m_grid.SetColumnWidth(7,0);
	m_grid.EnableHiddenColUnhide(FALSE);

	m_grid.AutoSizeRows(); //没有效果.
	m_grid.Invalidate(TRUE);
	//original func;
	/*fillTheGrid();*/
}

BOOL CGridDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	// 输出列Grid初始化
	vector<CString> firstRowFieldName;
	firstRowFieldName.clear();
	//getGridFirstRowTitlesName(firstRowFieldName);
	m_gridContManger.getFirstRowFields(firstRowFieldName);
	m_grid.SetColumnCount((int)firstRowFieldName.size());
	m_grid.SetFixedRowCount(1);
	//m_grid.SetFixedColumnCount(6);

	int i = 0;
	vector<CString>::iterator it = firstRowFieldName.begin();
	for(; it != firstRowFieldName.end(); it++)
	{
		m_grid.SetItemText(0,i,*it);
		m_grid.SetColumnWidth(i,90);
		i++;
	}
	UpdateOutColGrid();
	m_btnFcfRcdSwitch.ShowWindow(SW_HIDE);

	initTextFilePathEditCtrl();

	return TRUE;// return TRUE unless you set the focus to a control
	//original func here.
	//initDialog_original();
	// 异常: OCX 属性页应返回 FALSE
}

void CGridDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!UpdateData())
	{
		return;
	}
	// 获取各个输出列的内容
	for (int i = 1; i < m_grid.GetRowCount(); i++)
	{
		if (CGridCtrlUtil::GetCellChecked(m_grid, i, 1) && m_grid.GetRowHeight(i) > 0)		// 是否输出
		{
			int index = i - 1;
			CString colName = m_grid.GetItemText(i, 0);	// 列名
			CString precisionStr = m_grid.GetItemText(i, 2);		// 显示精度
			CString strTextAlignment = m_grid.GetItemText(i, 3);		// 文字水平对齐方式
			int textAlignment = 0;
			if (strTextAlignment.CompareNoCase(TEXT("左")) == 0)
			{
				textAlignment = 0;
			}
			else if (strTextAlignment.CompareNoCase(TEXT("中")) == 0)
			{
				textAlignment = 1;
			}
			else	//strTextAlignment.CompareNoCase(TEXT("右")) == 0
			{
				textAlignment = 2;
			}
		}
	}
	//acutPrintf(_T("\n Ok key is pressed..."));
	//delete this;
	//CAcUiDialog::OnOK();
	DestroyWindow();
}

void CGridDlg::OnBnClickedBtnpickdim()
{
	// TODO: 在此添加控件通知处理程序代码
	BeginEditorCommand();
	/*ads_point adsPt;
	acedGetPoint(NULL,_T("\n Please pick a point:"),adsPt);*/
	//获取当前文件名（包含路径)及代号;
	StGridToleranceRecord stGridRowRcd;
	prsFilenameAndCode(stGridRowRcd);

	ads_name ssAll;
	if(selectDimEntsManually(ssAll))
	{
		extractRcdFromSS(ssAll,stGridRowRcd);
	}
	acedSSFree(ssAll);

	CompleteEditorCommand();
	UpdateOutColGrid();
}

BOOL CGridDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	static int i = 0;
	//acutPrintf(_T("\n i am notified...%d"),i++);
	NM_GRIDVIEW *nmgv = (NM_GRIDVIEW*)(lParam);
	int mCol = nmgv->iColumn;
	int mRow = nmgv->iRow;
	if(nmgv->hdr.code == GVN_BEGINLABELEDIT)
	{
		//记录当前field（当前单元格控件）信息.
		//acutPrintf(_T("\n GVN_BEGINLABELEDIT...(%d,%d) \n"),mRow,mCol);
		CString csFilename = m_grid.GetItemText(mRow,(int)TolTableColumnFilename);
		CString csHandle = m_grid.GetItemText(mRow,(int)TolTableColumnDimEntHandle);
		CString csIndex = m_grid.GetItemText(mRow,(int)TolTableColumnStrIndex);
		m_gridContManger.locateCurRcdFiledToEdit(csFilename,csHandle,(ETolTableField)mCol,csIndex);
	}
	if(nmgv->hdr.code == GVN_ENDLABELEDIT)
	{
		//修改当前field（当前单元格控件）信息.
		//acutPrintf(_T("\n GVN_ENDLABELEDIT ...(%d,%d) \n"),mRow,mCol);
		CString newValue = m_grid.GetItemText(mRow,mCol);
		m_gridContManger.editCurRcdField(newValue);
		//立即更新.
		UpdateOutColGrid();
	}

	if(nmgv->hdr.code == GVN_SELCHANGED)
	{
		CString csFilename = m_grid.GetItemText(mRow,(int)TolTableColumnFilename);
		CString csHandle = m_grid.GetItemText(mRow,(int)TolTableColumnDimEntHandle);
		CString csIndex = m_grid.GetItemText(mRow,(int)TolTableColumnStrIndex);
		m_gridContManger.locateCurRcdFiledToEdit(csFilename,csHandle,(ETolTableField)mCol,csIndex);
		showOrHideSwitchBtn();
	}

	return CAcUiDialog::OnNotify(wParam, lParam, pResult);
}

void CGridDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	//acutPrintf(_T("\n cancel key is pressed..."));
	DestroyWindow();
	//CAcUiDialog::OnCancel();
}

void CGridDlg::OnKillFocus(CWnd* pNewWnd)
{
	CAcUiDialog::OnKillFocus(pNewWnd);
	//acutPrintf(_T("\n foucus killed ..."));
	// TODO: 在此处添加消息处理程序代码
}

void CGridDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	CAcUiDialog::PostNcDestroy();
	delete this;
}

void CGridDlg::getGridFirstRowTitlesName(vector<CString>& firstRowVec)
{
	firstRowVec.push_back(TEXT("编号"));
	firstRowVec.push_back(TEXT("基本尺寸"));
	firstRowVec.push_back(TEXT("上偏差"));
	firstRowVec.push_back(TEXT("下偏差"));
	firstRowVec.push_back(TEXT("属性"));
	firstRowVec.push_back(TEXT("环说明"));
}

void CGridDlg::fillTheGrid()
{
	m_grid.SetRowCount(1);

	int row = 1, col = 0;
	int i = 0;
	std::vector<CString> colNames = GetAllColumnName();
	m_grid.SetRowCount((int)colNames.size() + 1);
	for (int i = 0; i < colNames.size(); i++)
	{
		m_grid.SetItemText(row + i, col, colNames[i]);
	}

	// 精度下拉框的内容
	std::vector<CString> precisionStrs;
	CString val = TEXT("0");
	int precisionCount = 9;
	for (i = 0; i < precisionCount; i++)
	{
		precisionStrs.push_back(val);
		if (i == 0)
		{
			val += TEXT(".");
		}
		val += TEXT("0");
	}

	// 文字对齐方式的选项值
	std::vector<CString> textAlignments;
	textAlignments.push_back(TEXT("左"));
	textAlignments.push_back(TEXT("中"));
	textAlignments.push_back(TEXT("右"));

	int defaultAligmentIndex = 2;
	// 设置单元格的内容和状态
	int colWidth = 80;
	for (i = 1; i < m_grid.GetRowCount(); i++)
	{
		// 第一列为不可编辑
		m_grid.SetItemState(i, 0, GVIS_READONLY);

		// 设置第二列为复选框
		int col = 1;
		bool cellCheck = true;
		CGridCtrlUtil::SetCellCheckType(cellCheck, m_grid, i, col);
		m_grid.SetItemData(i, col, 1);

		// 设置第三列为下拉框
		col = 2;

		if (i != m_grid.GetRowCount() - 2)
		{
			m_grid.SetItemState(i, col, GVIS_READONLY);
		}
		else
		{
			CGridCtrlUtil::SetCellComboType(m_grid, i, col);
			CGridCtrlUtil::SetCellComboDwStyle(m_grid, i, col, CBS_DROPDOWNLIST);
			CGridCtrlUtil::SetCellComboOptions(m_grid, i, col, precisionStrs);
			m_grid.SetColumnWidth(col, colWidth);
			m_grid.SetItemText(i, col, TEXT("0.00"));
		}

		// 文字对齐方式列
		col = 3;
		CGridCtrlUtil::SetCellComboType(m_grid, i, col);
		CGridCtrlUtil::SetCellComboDwStyle(m_grid, i, col, CBS_DROPDOWNLIST);
		CGridCtrlUtil::SetCellComboOptions(m_grid, i, col, textAlignments);
		// 设置默认的对齐方式
		if (i == 1 || (i == m_grid.GetRowCount() - 1))	// 编号列、备注列为文字列，默认对齐方式为左对齐
		{
			defaultAligmentIndex = 0;		// 左对齐
		}
		else
		{
			defaultAligmentIndex = 1;		// 居中
		}
		m_grid.SetItemText(i, col, textAlignments[defaultAligmentIndex]);
	}

	m_grid.AutoSizeRows();
}

BOOL CGridDlg::initDialog_original()
{
	//original contents.
	m_grid.SetRowCount((int)GetAllColumnName().size());
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(4);
	m_grid.SetItemText(0, 0, TEXT("列名"));
	m_grid.SetItemText(0, 1, TEXT("输出"));
	m_grid.SetItemText(0, 2, TEXT("精度"));
	m_grid.SetItemText(0, 3, TEXT("文字对齐"));
	m_grid.SetColumnWidth(0, 160);
	m_grid.SetColumnWidth(1, 50);
	m_grid.SetColumnWidth(2, 75);
	m_grid.SetColumnWidth(3, 75);
	UpdateOutColGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
}

bool CGridDlg::prsCurDwgFilename(CString& csFilename)
{
	const ACHAR* filename; // = new ACHAR[256];
	int flag = acdbHostApplicationServices()->workingDatabase()->getFilename(filename);
	if(flag != Acad::eOk)
	{
		//delete[] filename;
		return false;
	}
	else
	{
		csFilename = filename;
		//delete[] filename;
		return true;
	}
}

void CGridDlg::OnBnClickedBtndeleteRcds()
{
	// TODO: 在此添加控件通知处理程序代码
	//删除某行或多行记录.
	CString strIndex;
	bool bIsRowSelected = false;
	int iRowCnt = m_grid.GetRowCount();
	int iColCnt = m_grid.GetColumnCount();
	for(int i = 1; i < iRowCnt; i++) //从1开始，不检查0行
	{
		for(int j =0; j < iColCnt; j++)
		{
			if(m_grid.IsCellSelected(i,j))
			{
				strIndex.Empty();
				strIndex = m_grid.GetItemText(i,(int)(ETolTableField::TolTableColumnStrIndex));
				m_gridContManger.deleteRcdByStrIndex(strIndex);
				break; //跳出，检查下一行.
			}
		}
	}
	//删除完成，更新表格.
	UpdateOutColGrid();
}

void CGridDlg::OnBnClickedBtnSelectCurDwgAll()
{
	// TODO: 在此添加控件通知处理程序代码
	//从全图当中提取标注信息.
	//acutPrintf(_T("\n select all ..."));
		// TODO: 在此添加控件通知处理程序代码
	//BeginEditorCommand();
	/*ads_point adsPt;
	acedGetPoint(NULL,_T("\n Please pick a point:"),adsPt);*/
	//获取当前文件名（包含路径)及代号;
	StGridToleranceRecord stGridRowRcd;
	prsFilenameAndCode(stGridRowRcd);

	ads_name ssAll;
	if(selectAllDimEntsFrmCurDwg(ssAll))
	{
		extractRcdFromSS(ssAll,stGridRowRcd);
	}
	acedSSFree(ssAll);

	//CompleteEditorCommand();
	UpdateOutColGrid();
}

bool CGridDlg::selectDimEntsManually(ads_name& ssAll)
{
	//用户手选标注实体;
	ads_name ssUnit;
	int flag = acedSSGet(NULL,NULL,NULL,NULL,ssAll);
	if(flag != RTNORM)
	{
		//acedSSFree(ssAll);
		//CompleteEditorCommand();
		return false;
	}
	long numSS = 0;
	acedSSLength(ssAll,&numSS);
	if(numSS <= 0)
	{
		acedSSFree(ssAll);
		return false;
	}
	return true;
}

void CGridDlg::prsFilenameAndCode(StGridToleranceRecord& stGridRowRcd)
{
	CString csFilename;
	prsCurDwgFilename(csFilename);

	//设置文件名;
	stGridRowRcd.m_filename = csFilename;
	//获取文件名的代号
	CString strFilenameCode = m_cadFilenames.prsStrCode(csFilename);
	stGridRowRcd.m_filenameCode = strFilenameCode;
	//调试用
	stGridRowRcd.m_filename = csFilename;
}

void CGridDlg::extractRcdFromSS(ads_name& ssAll,StGridToleranceRecord &stGridRowRcd)
{
	//stGridRowRcd已经记录了dwg文件名;
	vector<StGridToleranceRecord> tolRcdVect;
	tolRcdVect.clear();

	ads_name ssUnit;
	long numSS = 0;
	acedSSLength(ssAll,&numSS);
	for(int i = 0; i < numSS; i++)
	{
		acedSSName(ssAll,i,ssUnit);
		if(m_extractDimInfo.getAcDbDimensionInfo(ssUnit,stGridRowRcd))
		{
			if(m_gridContManger.isTheRcdExist(stGridRowRcd))
			{
				continue;//不允许对同一个实体提取多个记录;
			}
			//获取记录的序号.
			int idx = m_gridContManger.calStrIndex(stGridRowRcd.m_filename);
			idx++;
			CString strIdx;
			strIdx.Format(TEXT("%d"),idx);
			CString strIndex = stGridRowRcd.m_filenameCode + strIdx;
			stGridRowRcd.m_strIndex = strIndex;
			stGridRowRcd.m_index = idx;
			//insert.
			m_gridContManger.insertRowRcd(stGridRowRcd);
		}
		else if(m_extractDimInfo.getAcDbFcfTolInfo(ssUnit,stGridRowRcd,tolRcdVect))
		{
			if(tolRcdVect.empty() == true)
			{
				continue;
			}
			vector<StGridToleranceRecord>::iterator itr = tolRcdVect.begin();
			if(m_gridContManger.isTheRcdExist(*itr))
			{
				continue;//不允许对同一个实体提取多个记录;
			}
			for(; itr != tolRcdVect.end(); itr++)
			{
				//获取记录的序号.
				assignIndex((*itr));
				//insert.
				m_gridContManger.insertRowRcd((*itr));
			}
		}
	}
}

bool CGridDlg::selectAllDimEntsFrmCurDwg(ads_name& ssAll)
{
	//用户手选标注实体;
	ads_name ssUnit;
	int flag = acedSSGet(_T("A"),NULL,NULL,NULL,ssAll);
	if(flag != RTNORM)
	{
		//acedSSFree(ssAll);
		//CompleteEditorCommand();
		return false;
	}
	long numSS = 0;
	acedSSLength(ssAll,&numSS);
	if(numSS <= 0)
	{
		acedSSFree(ssAll);
		return false;
	}
	return true;
}

void CGridDlg::showOrHideSwitchBtn()
{
	//acutPrintf(_T("\n GVN_SELCHANGED...grid(%d,%d) \n"),mRow,mCol);
	CCellRange cr =  m_grid.GetSelectedCellRange();
	int minRow = cr.GetMinRow();
	int maxRow = cr.GetMaxRow();
	//acutPrintf(_T("\n GVN_SELCHANGED...(minRow,maxRow) = (%d,%d) \n"),minRow,maxRow);

	if(minRow == maxRow && minRow != -1)
	{
		//说明只选中了一行中的单元格;
		CString csTolType = m_grid.GetItemText(minRow,(int)TolTableColumnTolType);
		if(csTolType == TEXT("形位公差环"))
		{
			m_btnFcfRcdSwitch.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnFcfRcdSwitch.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_btnFcfRcdSwitch.ShowWindow(SW_HIDE);
	}
}

void CGridDlg::OnBtnSwitchTolClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	//对当前选中的形位公差记录数值切换，如何切换？
	//若基本尺寸为0，直接把上公差填入“基本尺寸”栏;
	//若基本尺寸不为0,将之设置为0;
	m_gridContManger.switchOptOnToleranceRcd();
	UpdateOutColGrid();
}

void CGridDlg::OnSelTxtSavePathBnClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	//先从json文件中获得之前路径,填充edit控件.
	//json文件默认是utf8格式保存.
	//填充edit ctrl
	//CString pathFromJson = SomeFunction::getDefaultTextSaveFullPath();
	//m_editCtrlTextFilePath.SetWindowText(pathFromJson);
	//user select file path;
	CString filterStr = _T("Describe Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CString savePath = SomeFunction::selectFilePathByUser(filterStr);
	if(savePath.IsEmpty() == false)
	{
		m_editCtrlTextFilePath.SetWindowText(savePath);
		//test:save to json;
	}
}

void CGridDlg::OnBnClickedBtnSavePathToJsonAndText()
{
	// TODO: 在此添加控件通知处理程序代码.
	//检查edit里面的内容是否为空； 不为空，保存该路径到json.
	//这个路径作为保存text用的文件.
	CString savePath;
	m_editCtrlTextFilePath.GetWindowText(savePath);
	if(savePath.IsEmpty() == false)
	{		
		//这里不再写入json
		//SomeFunction::saveTextFullPathToJson(savePath);
		//接下来就是保存表格内容到text文件，该text文件路径如上.
		if(m_gridContManger.saveGridContToTextFile(savePath))
		{
			MessageBox(TEXT("文本文件保存成功!"),TEXT("尺寸公差信息提取"));
		}
		else
		{
			MessageBox(TEXT("文本文件保存失败!"),TEXT("尺寸公差信息提取软件"),MB_ICONHAND |MB_OK);
		}
	}
}

void CGridDlg::initTextFilePathEditCtrl()
{
	//预先从json读取text存放路径.
	m_editCtrlTextFilePath.SetReadOnly(TRUE);
	CString pathFromJson = SomeFunction::getDefaultTextSaveFullPath();
	m_editCtrlTextFilePath.SetWindowText(pathFromJson);
}

void CGridDlg::assignIndex(StGridToleranceRecord &stGridRowRcd)
{
	//获取并给记录赋序号.
	int idx = m_gridContManger.calStrIndex(stGridRowRcd.m_filename);
	idx++;
	CString strIdx;
	strIdx.Format(TEXT("%d"),idx);
	CString strIndex = stGridRowRcd.m_filenameCode + strIdx;
	stGridRowRcd.m_strIndex = strIndex;
	stGridRowRcd.m_index = idx;
}
