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
	colNames.push_back(TEXT("���"));
	colNames.push_back(TEXT("ͼ��"));
	colNames.push_back(TEXT("�豸����������"));
	colNames.push_back(TEXT("����ͺ�"));
	colNames.push_back(TEXT("��λ"));
	colNames.push_back(TEXT("����"));
	colNames.push_back(TEXT("��ע"));
	return colNames;
}
void CGridDlg::UpdateOutColGrid()
{
	//������.
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
	//���ص���������
	m_grid.SetColumnWidth(6,0);
	m_grid.SetColumnWidth(7,0);
	m_grid.EnableHiddenColUnhide(FALSE);

	m_grid.AutoSizeRows(); //û��Ч��.
	m_grid.Invalidate(TRUE);
	//original func;
	/*fillTheGrid();*/
}

BOOL CGridDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// �����Grid��ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGridDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (!UpdateData())
	{
		return;
	}
	// ��ȡ��������е�����
	for (int i = 1; i < m_grid.GetRowCount(); i++)
	{
		if (CGridCtrlUtil::GetCellChecked(m_grid, i, 1) && m_grid.GetRowHeight(i) > 0)		// �Ƿ����
		{
			int index = i - 1;
			CString colName = m_grid.GetItemText(i, 0);	// ����
			CString precisionStr = m_grid.GetItemText(i, 2);		// ��ʾ����
			CString strTextAlignment = m_grid.GetItemText(i, 3);		// ����ˮƽ���뷽ʽ
			int textAlignment = 0;
			if (strTextAlignment.CompareNoCase(TEXT("��")) == 0)
			{
				textAlignment = 0;
			}
			else if (strTextAlignment.CompareNoCase(TEXT("��")) == 0)
			{
				textAlignment = 1;
			}
			else	//strTextAlignment.CompareNoCase(TEXT("��")) == 0
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginEditorCommand();
	/*ads_point adsPt;
	acedGetPoint(NULL,_T("\n Please pick a point:"),adsPt);*/
	//��ȡ��ǰ�ļ���������·��)������;
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
	// TODO: �ڴ����ר�ô����/����û���
	static int i = 0;
	//acutPrintf(_T("\n i am notified...%d"),i++);
	NM_GRIDVIEW *nmgv = (NM_GRIDVIEW*)(lParam);
	int mCol = nmgv->iColumn;
	int mRow = nmgv->iRow;
	if(nmgv->hdr.code == GVN_BEGINLABELEDIT)
	{
		//��¼��ǰfield����ǰ��Ԫ��ؼ�����Ϣ.
		//acutPrintf(_T("\n GVN_BEGINLABELEDIT...(%d,%d) \n"),mRow,mCol);
		CString csFilename = m_grid.GetItemText(mRow,(int)TolTableColumnFilename);
		CString csHandle = m_grid.GetItemText(mRow,(int)TolTableColumnDimEntHandle);
		CString csIndex = m_grid.GetItemText(mRow,(int)TolTableColumnStrIndex);
		m_gridContManger.locateCurRcdFiledToEdit(csFilename,csHandle,(ETolTableField)mCol,csIndex);
	}
	if(nmgv->hdr.code == GVN_ENDLABELEDIT)
	{
		//�޸ĵ�ǰfield����ǰ��Ԫ��ؼ�����Ϣ.
		//acutPrintf(_T("\n GVN_ENDLABELEDIT ...(%d,%d) \n"),mRow,mCol);
		CString newValue = m_grid.GetItemText(mRow,mCol);
		m_gridContManger.editCurRcdField(newValue);
		//��������.
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
	// TODO: �ڴ����ר�ô����/����û���
	//acutPrintf(_T("\n cancel key is pressed..."));
	DestroyWindow();
	//CAcUiDialog::OnCancel();
}

void CGridDlg::OnKillFocus(CWnd* pNewWnd)
{
	CAcUiDialog::OnKillFocus(pNewWnd);
	//acutPrintf(_T("\n foucus killed ..."));
	// TODO: �ڴ˴������Ϣ����������
}

void CGridDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	CAcUiDialog::PostNcDestroy();
	delete this;
}

void CGridDlg::getGridFirstRowTitlesName(vector<CString>& firstRowVec)
{
	firstRowVec.push_back(TEXT("���"));
	firstRowVec.push_back(TEXT("�����ߴ�"));
	firstRowVec.push_back(TEXT("��ƫ��"));
	firstRowVec.push_back(TEXT("��ƫ��"));
	firstRowVec.push_back(TEXT("����"));
	firstRowVec.push_back(TEXT("��˵��"));
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

	// ���������������
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

	// ���ֶ��뷽ʽ��ѡ��ֵ
	std::vector<CString> textAlignments;
	textAlignments.push_back(TEXT("��"));
	textAlignments.push_back(TEXT("��"));
	textAlignments.push_back(TEXT("��"));

	int defaultAligmentIndex = 2;
	// ���õ�Ԫ������ݺ�״̬
	int colWidth = 80;
	for (i = 1; i < m_grid.GetRowCount(); i++)
	{
		// ��һ��Ϊ���ɱ༭
		m_grid.SetItemState(i, 0, GVIS_READONLY);

		// ���õڶ���Ϊ��ѡ��
		int col = 1;
		bool cellCheck = true;
		CGridCtrlUtil::SetCellCheckType(cellCheck, m_grid, i, col);
		m_grid.SetItemData(i, col, 1);

		// ���õ�����Ϊ������
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

		// ���ֶ��뷽ʽ��
		col = 3;
		CGridCtrlUtil::SetCellComboType(m_grid, i, col);
		CGridCtrlUtil::SetCellComboDwStyle(m_grid, i, col, CBS_DROPDOWNLIST);
		CGridCtrlUtil::SetCellComboOptions(m_grid, i, col, textAlignments);
		// ����Ĭ�ϵĶ��뷽ʽ
		if (i == 1 || (i == m_grid.GetRowCount() - 1))	// ����С���ע��Ϊ�����У�Ĭ�϶��뷽ʽΪ�����
		{
			defaultAligmentIndex = 0;		// �����
		}
		else
		{
			defaultAligmentIndex = 1;		// ����
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
	m_grid.SetItemText(0, 0, TEXT("����"));
	m_grid.SetItemText(0, 1, TEXT("���"));
	m_grid.SetItemText(0, 2, TEXT("����"));
	m_grid.SetItemText(0, 3, TEXT("���ֶ���"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ɾ��ĳ�л���м�¼.
	CString strIndex;
	bool bIsRowSelected = false;
	int iRowCnt = m_grid.GetRowCount();
	int iColCnt = m_grid.GetColumnCount();
	for(int i = 1; i < iRowCnt; i++) //��1��ʼ�������0��
	{
		for(int j =0; j < iColCnt; j++)
		{
			if(m_grid.IsCellSelected(i,j))
			{
				strIndex.Empty();
				strIndex = m_grid.GetItemText(i,(int)(ETolTableField::TolTableColumnStrIndex));
				m_gridContManger.deleteRcdByStrIndex(strIndex);
				break; //�����������һ��.
			}
		}
	}
	//ɾ����ɣ����±��.
	UpdateOutColGrid();
}

void CGridDlg::OnBnClickedBtnSelectCurDwgAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȫͼ������ȡ��ע��Ϣ.
	//acutPrintf(_T("\n select all ..."));
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	//BeginEditorCommand();
	/*ads_point adsPt;
	acedGetPoint(NULL,_T("\n Please pick a point:"),adsPt);*/
	//��ȡ��ǰ�ļ���������·��)������;
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
	//�û���ѡ��עʵ��;
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

	//�����ļ���;
	stGridRowRcd.m_filename = csFilename;
	//��ȡ�ļ����Ĵ���
	CString strFilenameCode = m_cadFilenames.prsStrCode(csFilename);
	stGridRowRcd.m_filenameCode = strFilenameCode;
	//������
	stGridRowRcd.m_filename = csFilename;
}

void CGridDlg::extractRcdFromSS(ads_name& ssAll,StGridToleranceRecord &stGridRowRcd)
{
	//stGridRowRcd�Ѿ���¼��dwg�ļ���;
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
				continue;//�������ͬһ��ʵ����ȡ�����¼;
			}
			//��ȡ��¼�����.
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
				continue;//�������ͬһ��ʵ����ȡ�����¼;
			}
			for(; itr != tolRcdVect.end(); itr++)
			{
				//��ȡ��¼�����.
				assignIndex((*itr));
				//insert.
				m_gridContManger.insertRowRcd((*itr));
			}
		}
	}
}

bool CGridDlg::selectAllDimEntsFrmCurDwg(ads_name& ssAll)
{
	//�û���ѡ��עʵ��;
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
		//˵��ֻѡ����һ���еĵ�Ԫ��;
		CString csTolType = m_grid.GetItemText(minRow,(int)TolTableColumnTolType);
		if(csTolType == TEXT("��λ���"))
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�Ե�ǰѡ�е���λ�����¼��ֵ�л�������л���
	//�������ߴ�Ϊ0��ֱ�Ӱ��Ϲ������롰�����ߴ硱��;
	//�������ߴ粻Ϊ0,��֮����Ϊ0;
	m_gridContManger.switchOptOnToleranceRcd();
	UpdateOutColGrid();
}

void CGridDlg::OnSelTxtSavePathBnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ȴ�json�ļ��л��֮ǰ·��,���edit�ؼ�.
	//json�ļ�Ĭ����utf8��ʽ����.
	//���edit ctrl
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������.
	//���edit����������Ƿ�Ϊ�գ� ��Ϊ�գ������·����json.
	//���·����Ϊ����text�õ��ļ�.
	CString savePath;
	m_editCtrlTextFilePath.GetWindowText(savePath);
	if(savePath.IsEmpty() == false)
	{		
		//���ﲻ��д��json
		//SomeFunction::saveTextFullPathToJson(savePath);
		//���������Ǳ��������ݵ�text�ļ�����text�ļ�·������.
		if(m_gridContManger.saveGridContToTextFile(savePath))
		{
			MessageBox(TEXT("�ı��ļ�����ɹ�!"),TEXT("�ߴ繫����Ϣ��ȡ"));
		}
		else
		{
			MessageBox(TEXT("�ı��ļ�����ʧ��!"),TEXT("�ߴ繫����Ϣ��ȡ���"),MB_ICONHAND |MB_OK);
		}
	}
}

void CGridDlg::initTextFilePathEditCtrl()
{
	//Ԥ�ȴ�json��ȡtext���·��.
	m_editCtrlTextFilePath.SetReadOnly(TRUE);
	CString pathFromJson = SomeFunction::getDefaultTextSaveFullPath();
	m_editCtrlTextFilePath.SetWindowText(pathFromJson);
}

void CGridDlg::assignIndex(StGridToleranceRecord &stGridRowRcd)
{
	//��ȡ������¼�����.
	int idx = m_gridContManger.calStrIndex(stGridRowRcd.m_filename);
	idx++;
	CString strIdx;
	strIdx.Format(TEXT("%d"),idx);
	CString strIndex = stGridRowRcd.m_filenameCode + strIdx;
	stGridRowRcd.m_strIndex = strIndex;
	stGridRowRcd.m_index = idx;
}
