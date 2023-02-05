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
//----- GridDlg.h : Declaration of the CGridDlg
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "GridCtrl.h"

#include "selectAndParseDimension.h"
#include "prsToleranceText.h"
#include "afxwin.h"
#include "someFunc.h"
//-----------------------------------------------------------------------------



class CGridDlg : public CAcUiDialog {
	DECLARE_DYNAMIC (CGridDlg)

public:
	CGridDlg (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_GRID} ;
	std::vector<CString> GetAllColumnName();
	void UpdateOutColGrid();

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnpickdim();
	afx_msg void OnBnClickedBtndeleteRcds();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnBnClickedBtnSelectCurDwgAll();
	afx_msg void OnBtnSwitchTolClicked();
	afx_msg void OnSelTxtSavePathBnClicked();
	afx_msg void OnBnClickedBtnSavePathToJsonAndText();

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnCancel();
protected:
	virtual void PostNcDestroy();
private:
	void getGridFirstRowTitlesName(vector<CString>& firstRowVec);
	BOOL initDialog_original();
	void fillTheGrid();
	bool selectDimEntsManually(ads_name& ssAll);
	bool selectAllDimEntsFrmCurDwg(ads_name& ssAll);
	void prsFilenameAndCode(StGridToleranceRecord& stGridRowRcd);
	void extractRcdFromSS(ads_name& ssAll,StGridToleranceRecord &stGridRowRcd);
	void assignIndex(StGridToleranceRecord &stGridRowRcd);
	bool prsCurDwgFilename(CString& csFilename);
	void showOrHideSwitchBtn();
	void initTextFilePathEditCtrl();

private:
	CGridCtrl m_grid;
	CGetDimensionInfo m_extractDimInfo;
	CGridContentManager m_gridContManger;
	AllCadFilenameRecord m_cadFilenames;
	//PrsToleranceText m_prsTolObj;
public:
	CButton m_btnPickDim;
	CButton m_btnFcfRcdSwitch;
	CEdit m_editCtrlTextFilePath;
	CButton m_saveTextBtn;
} ;
