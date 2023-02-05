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
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

//
#include "DccMainDlg.h"
#include "GridDlg.h"
#include "FindDccPath.h"
#include "CJsonObject.hpp"
//#include "someFunc.h"
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

//-----------------------------------------------------------------------------
#define szRDS _RXST("hugo")
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxProject1App : public AcRxArxApp {

public:
	CArxProject1App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}


	// - hugoArxProject1.qqt command (do not rename)
	static void hugoArxProject1qqt(void)
	{
		// Add your code for command hugoArxProject1.qqt here
		//DccMainDlg dlg;
		//dlg.DoModal();
		//
		//DccMainDlg* pDlg = new DccMainDlg;
		//pDlg->Create(DccMainDlg::IDD,acedGetAcadFrame());
		//pDlg->ShowWindow(SW_SHOW);

		//current module file path
		ACHAR filename[256];
		GetModuleFileNameW(NULL,filename,256);
		acutPrintf(filename);

		CAcModuleResourceOverride myResource;
		CGridDlg* pDlg = new CGridDlg;
		pDlg->Create(IDD_GRID,NULL);
		CRect rect;
		pDlg->GetWindowRect(&rect);
		rect.MoveToXY(500,200);
		pDlg->MoveWindow(&rect);
		pDlg->ShowWindow(SW_SHOW);
	}

	// ----- hugoGridDialog1._openDccExe command
	static void hugoGridDialog1_openDccExe(void)
	{
		// Add your code for command hugoGridDialog1._findDccPath here
		//取json中存放的dcc执行文件的路径，然后运行它.
		string strPath = SomeFunction::getDccExeFullPath();
		WinExec(strPath.c_str(), SW_SHOW);
	}

	// - hugoGridDialog1._findDccExePath command (do not rename)
	static void hugoGridDialog1_findDccExePath(void)
	{
		// Add your code for command hugoGridDialog1._findDccExePath here
		//指定Dcc的路径;
		CAcModuleResourceOverride myResource;
		CFindDccPath pDlg;
		pDlg.DoModal();
	}
} ;



//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxProject1App)

ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, hugoArxProject1, qqt, qqt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, hugoGridDialog1, _openDccExe, openDcc, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject1App, hugoGridDialog1, _findDccExePath, dccPath, ACRX_CMD_TRANSPARENT, NULL)
