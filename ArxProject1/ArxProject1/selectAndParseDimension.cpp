//void CGetDimensionInfo::getDimInfo(ads_name unitSS,vector<vector<CString>>& dimInfoVec)
//{
//	AcDbAlignedDimension* pAlignDim = NULL;
//	if(isAcDbAlignedDimension(unitSS,pAlignDim))
//	{
//		AcDbDimStyleTableRecord* pDimStyleRcd = NULL;
//		pAlignDim->getDimstyleData(pDimStyleRcd);
//		//
//		//pDimStyleRcd->getField();
//		ACHAR* ptext = pAlignDim->dimensionText();
//		acutPrintf(ptext);
//	}
//	return;
//}

#include "StdAfx.h"
#include "selectAndParseDimension.h"

//bool CGetDimensionInfo::getAcDbRotatedDimensionInfo(ads_name unitSS,StGridToleranceRecord& dimInfoVec)
//{
//	//double dist;
//	//AcDbDimension* testDimPtr = NULL;
//	//testDimPtr->dimtp();
//	//testDimPtr->dimtm();
//	//testDimPtr->measurement(dist);
//
//	AcDbRotatedDimension* pAlignDim = NULL;
//	if(isAcDbRotatedDimension(unitSS,pAlignDim))
//	{
//		double tp = pAlignDim->dimtp();
//		dimInfoVec.m_tolTop.Format(TEXT("%f"),tp);
//		//acutPrintf(_T("\n dimtp is %.20f "),tp);
//		double bm = pAlignDim->dimtm();
//		dimInfoVec.m_tolBottom.Format(TEXT("%f"),bm);
//		//acutPrintf(_T("\n dimtm is %.20f "),tp);
//		double dimMeasure = -1.0;
//		pAlignDim->measurement(dimMeasure);
//		dimInfoVec.m_strMeasure.Format(TEXT("%f"),dimMeasure);
//
//		dimInfoVec.dimensionType = TEXT("尺寸");
//
//		dimInfoVec.m_csHandle = getAcDbHandleCstr(pAlignDim);
//
//		pAlignDim->close();
//		return true;
//	}
//	return false;
//}

bool CGetDimensionInfo::isAcDbRotatedDimension(ads_name unitSS,AcDbRotatedDimension*& pAlignDim)
{
	AcDbEntity* pEnt = NULL;
	AcDbObjectId entId;
	acdbGetObjectId(entId,unitSS);
	acdbOpenAcDbEntity(pEnt,entId,AcDb::OpenMode::kForRead);
	if(pEnt->isA() == AcDbRotatedDimension::desc())
	{
		pAlignDim = AcDbRotatedDimension::cast(pEnt);
		//pEnt->close();
		return true;
	}
	else
	{
		pEnt->close();
		return false;
	}
}

CGetDimensionInfo::CGetDimensionInfo()
{
	 m_pDimStyleRcd = new AcDbDimStyleTableRecord;
}

CGetDimensionInfo::~CGetDimensionInfo()
{
	if(m_pDimStyleRcd != NULL)
	{
		delete m_pDimStyleRcd;
	}
}

CString CGetDimensionInfo::getAcDbHandleCstr(const AcDbEntity* pEnt)
{
	ACHAR wcHandle[64] = {};
	AcDbHandle hdl;
	pEnt->getAcDbHandle(hdl);
	hdl.getIntoAsciiBuffer(wcHandle);
	CString csTemp = wcHandle;
	return csTemp;
}

bool CGetDimensionInfo::getAcDbDimensionInfo(ads_name unitSS,StGridToleranceRecord& dimInfoVec)
{
	AcDbDimension* pAcdbDim = NULL;
	if(isAcDbDimension(unitSS,pAcdbDim))
	{
		double tp = pAcdbDim->dimtp();
		if(pAcdbDim->isA() == AcDb2LineAngularDimension::desc())
		{
			tp = degreeToMinute(tp);
		}
		dimInfoVec.m_tolTop.Format(TEXT("%.3f"),tp);
		cutAffixZeroChar(dimInfoVec.m_tolTop);
		//acutPrintf(_T("\n dimtp is %.20f "),tp);

		double bm = pAcdbDim->dimtm();
		//bm = isGreaterThanZero(bm)?-bm:bm;
		bm = -bm;
		bm =isZero(bm)? 0:bm;
		if(pAcdbDim->isA() == AcDb2LineAngularDimension::desc())
		{
			bm = degreeToMinute(bm);
		}
		//bm = angleToRadian(bm);
		dimInfoVec.m_tolBottom.Format(TEXT("%.3f"),bm);
		cutAffixZeroChar(dimInfoVec.m_tolBottom);
		//acutPrintf(_T("\n dimtm is %.20f "),tp);

		double dimMeasure = -1.0;
		pAcdbDim->measurement(dimMeasure);
		if(pAcdbDim->isA() == AcDb2LineAngularDimension::desc())
		{
			dimMeasure = radianToAngle(dimMeasure);
		}
		dimInfoVec.m_strMeasure.Format(TEXT("%.3f"),dimMeasure);
		//砍掉后缀无效字符'0'.
		cutAffixZeroChar(dimInfoVec.m_strMeasure);

		prsAcDbDimensionType(pAcdbDim,dimInfoVec);

		dimInfoVec.m_csHandle = getAcDbHandleCstr(pAcdbDim);

		pAcdbDim->close();
		return true;
	}
	return false;
}

bool CGetDimensionInfo::isAcDbDimension(ads_name unitSS,AcDbDimension*& pAcdbDim)
{
	AcDbEntity* pEnt = NULL;
	AcDbObjectId entId;
	acdbGetObjectId(entId,unitSS);
	acdbOpenAcDbEntity(pEnt,entId,AcDb::OpenMode::kForRead);
	if(pEnt->isA() == AcDbRotatedDimension::desc())
	{
		pAcdbDim = AcDbRotatedDimension::cast(pEnt);
		return true;
	}
	else if(pEnt->isA() == AcDbAlignedDimension::desc())
	{
		pAcdbDim = AcDbAlignedDimension::cast(pEnt);
		return true;
	}
	else if(pEnt->isA() == AcDb2LineAngularDimension::desc())
	{
		pAcdbDim = AcDb2LineAngularDimension::cast(pEnt);
		return true;
	}
	else if(pEnt->isA() == AcDb3PointAngularDimension::desc())
	{
		pAcdbDim = AcDb3PointAngularDimension::cast(pEnt);
		return true;
	}
	else if(pEnt->isA() == AcDbArcDimension::desc())
	{
		pAcdbDim = AcDbArcDimension::cast(pEnt);
		return true;
	}
	else if(pEnt->isA() == AcDbDiametricDimension::desc())
	{
		pAcdbDim = AcDbDiametricDimension::cast(pEnt);
		return true;
	}
	else if(pEnt->isA() == AcDbOrdinateDimension::desc())
	{
		pAcdbDim = AcDbOrdinateDimension::cast(pEnt);
		return true;
	}
	else if(pEnt->isA() == AcDbRadialDimension::desc())
	{
		pAcdbDim = AcDbRadialDimension::cast(pEnt);
		return true;
	}
	else if(pEnt->isA() == AcDbRadialDimensionLarge::desc())
	{
		pAcdbDim = AcDbRadialDimensionLarge::cast(pEnt);
		return true;
	}
	else
	{
		pEnt->close();
		pAcdbDim = NULL;
		return false;
	}
}

bool CGetDimensionInfo::isAcDbFcf(ads_name unitSS,AcDbFcf*& pAcdbFcf)
{
	AcDbEntity* pEnt = NULL;
	AcDbObjectId entId;
	acdbGetObjectId(entId,unitSS);
	acdbOpenAcDbEntity(pEnt,entId,AcDb::OpenMode::kForRead);
	if(pEnt->isA() == AcDbFcf::desc())
	{
		pAcdbFcf = AcDbFcf::cast(pEnt);
		return true;
	}
	else
	{
		pAcdbFcf = NULL;
		pEnt->close();
		return false;
	}
}

bool CGetDimensionInfo::prsAcDbDimensionType(const AcDbDimension* acdbDimPtr,
											 StGridToleranceRecord& dimInfoVec)
{
	//
	if(acdbDimPtr->isA() == AcDbRotatedDimension::desc())
	{
		dimInfoVec.dimensionType = _T("尺寸环");
		return true;
	}
	else if(acdbDimPtr->isA() == AcDbAlignedDimension::desc())
	{
		dimInfoVec.dimensionType = _T("尺寸环");
		return true;
	}
	else if(acdbDimPtr->isA() == AcDb2LineAngularDimension::desc())
	{
		dimInfoVec.dimensionType = _T("角度环");
		return true;
	}
	else if(acdbDimPtr->isA() == AcDb3PointAngularDimension::desc())
	{
		dimInfoVec.dimensionType = _T("角度环");
		return true;
	}
	else if(acdbDimPtr->isA() == AcDbArcDimension::desc())
	{
		dimInfoVec.dimensionType = _T("角度环");
		return true;
	}
	else if(acdbDimPtr->isA() == AcDbDiametricDimension::desc())
	{
		dimInfoVec.dimensionType = _T("尺寸环");
		return true;
	}
	else if(acdbDimPtr->isA() == AcDbOrdinateDimension::desc())
	{
		dimInfoVec.dimensionType = _T("尺寸环");
		return true;
	}
	else if(acdbDimPtr->isA() == AcDbRadialDimension::desc())
	{
		dimInfoVec.dimensionType = _T("尺寸环");
		return true;
	}
	else if(acdbDimPtr->isA() == AcDbRadialDimensionLarge::desc())
	{
		dimInfoVec.dimensionType = _T("尺寸环");
		return true;
	}
	return false;
}


bool CGetDimensionInfo::getAcDbFcfTolInfo(ads_name unitSS,
										StGridToleranceRecord& dimInfoVec,
										vector<StGridToleranceRecord>& tolRcdVect)
{
	tolRcdVect.clear();

	AcDbFcf* pAcdbFcf = NULL;
	if(isAcDbFcf(unitSS,pAcdbFcf))
	{
		CString strCont = pAcdbFcf->text();

		double firstTol = -1;
		double secondTol = -1;
		m_prsTolTextFieldObj.inputText(strCont);
		if(m_prsTolTextFieldObj.rtnFirstTol(firstTol) == true)
		{
			StGridToleranceRecord firstRcd = dimInfoVec;
			if(isZero(firstTol))
			{
				firstTol = 0;
			}
			fillWithFcfTolVal(firstRcd, firstTol);
			//test
			//acutPrintf(_T("\n firstTol is:%f"),firstTol);
			//acutPrintf(_T("\n secondTol is:%f"),secondTol);
			//test end
			//type;
			firstRcd.dimensionType = _T("形位公差环");
			//handle.
			firstRcd.m_csHandle = getAcDbHandleCstr(pAcdbFcf);
			tolRcdVect.push_back(firstRcd);
		}
		if(m_prsTolTextFieldObj.rtnSecondTol(secondTol) == true)
		{
			StGridToleranceRecord secondRcd = dimInfoVec;
			if(isZero(secondTol))
			{
				secondTol = 0;
			}
			fillWithFcfTolVal(secondRcd, secondTol);
			//test
			//acutPrintf(_T("\n firstTol is:%f"),firstTol);
			//acutPrintf(_T("\n secondTol is:%f"),secondTol);
			//test end
			//type;
			secondRcd.dimensionType = _T("形位公差环");
			//handle.
			secondRcd.m_csHandle = getAcDbHandleCstr(pAcdbFcf);
			tolRcdVect.push_back(secondRcd);
		}
		pAcdbFcf->close();
		return true;
	}
	return false;
}

void CGetDimensionInfo::cutAffixZeroChar(CString& cs)
{
	//去掉末尾无效的0字符;前提是cs是double形式的字符串.
	//检查是否含有小数点,若没有，则不用处理.
	if(cs.Find('.') == false)
	{
		return;
	}
	int csLen = cs.GetLength();
	while(csLen > 1)
	{
		if(cs.GetAt(csLen-1) == '0')
		{
			cs = cs.Mid(0,csLen-1);
			csLen--;
			continue;
		}
		else if(cs.GetAt(csLen-1) == '.')
		{
			cs = cs.Mid(0,csLen-1);
			csLen--;
			break;
		}
		else
		{
			break;
		}
	}
}

double CGetDimensionInfo::angleToRadian(const double& ang)
{
	//输入：角度
	//输出：弧度
	double outRadian = ang / 180.0 * DccDimInfoEx::Pi;
	return outRadian;
}

double CGetDimensionInfo::radianToAngle(const double& rad)
{
	double ang = rad / DccDimInfoEx::Pi * 180.0;
	return ang;
}

bool CGetDimensionInfo::isZero(const double& fVal)
{
	if(abs(fVal) <= 1E-6)
	{
		return true;
	}
	return false;
}

bool CGetDimensionInfo::isGreaterThanZero(const double& fVal)
{
	if(fVal > 1E-6)
	{
		return true;
	}
	return false;
}

double CGetDimensionInfo::degreeToMinute(const double& fDegree)
{
	//用于角度公差环，将上公差和下公差单位从度转化为分;
	//角度标注提取信息，基本尺寸以弧度为单位，上下公差以度为单位;
	double minutes = fDegree * 60.0;
	return minutes;
}

void CGetDimensionInfo::fillWithFcfTolVal(StGridToleranceRecord &dimInfoVec, const double fcfTolVal)
{
	dimInfoVec.m_tolTop.Format(TEXT("%.3f"),fcfTolVal / 2.0);
	dimInfoVec.m_tolBottom.Format(TEXT("%.3f"),- fcfTolVal / 2.0);
	dimInfoVec.m_strMeasure.Format(TEXT("%.3f"),0);
	
	//cut affix 0
	cutAffixZeroChar(dimInfoVec.m_tolTop);
	cutAffixZeroChar(dimInfoVec.m_tolBottom);
	cutAffixZeroChar(dimInfoVec.m_strMeasure);
}

//void CGetDimensionInfo::getDimInfo(ads_name unitSS,vector<vector<CString>>& dimInfoVec)
//{
//	AcDbAlignedDimension* pAlignDim = NULL;
//	if(isAcDbAlignedDimension(unitSS,pAlignDim))
//	{
//		AcDbDimStyleTableRecord* pDimStyleRcd = NULL;
//		pAlignDim->getDimstyleData(pDimStyleRcd);
//		//
//		//pDimStyleRcd->getField();
//		ACHAR* ptext = pAlignDim->dimensionText();
//		acutPrintf(ptext);
//	}
//	return;
//}

void CGridContentManager::getFirstRowFields(vector<CString>& firstRowFieldsVec)
{
	firstRowFieldsVec = m_firstRowFields;
}


bool CGridContentManager::getColumnCount(int& nColumnCount)
{
	if(m_firstRowFields.empty())
	{

		return false;
	}
	else
	{
		nColumnCount = (int)m_firstRowFields.size();
	}
}

CGridContentManager::CGridContentManager()
{
	m_firstRowFields.push_back(TEXT("编号"));
	m_firstRowFields.push_back(TEXT("基本尺寸"));
	m_firstRowFields.push_back(TEXT("上偏差"));
	m_firstRowFields.push_back(TEXT("下偏差"));
	m_firstRowFields.push_back(TEXT("属性"));
	m_firstRowFields.push_back(TEXT("环说明"));
	m_firstRowFields.push_back(TEXT("文件名"));
	m_firstRowFields.push_back(TEXT("句柄"));
}

const vector<StGridToleranceRecord>& CGridContentManager::rtnAllRcds()
{
	return m_contRcdVec;
}

void CGridContentManager::insertRowRcd(const StGridToleranceRecord& gridRowRcd)
{
	//插入后需要按编码排序.
	StGridToleranceRecord tempRcd = gridRowRcd;
	//计算strIndex;
	;
	m_contRcdVec.push_back(gridRowRcd);
	sort(m_contRcdVec.begin(),m_contRcdVec.end(),sortByIndex);
}

void CGridContentManager::deleteRowRcd(const CString strCode)
{
	vector<StGridToleranceRecord>::iterator itr = m_contRcdVec.begin();
	for(; itr != m_contRcdVec.end(); itr++)
	{
		if(itr->m_strIndex == strCode)
		{
			itr = m_contRcdVec.erase(itr);
			return;
		}
	}
}

int CGridContentManager::calStrIndex(const CString& filename)
{
	//根据文件名和所有记录信息，计算新加入记录的编号（必须唯一);
	//找到同一dwg文件下的标注实体的最大序号，返回它.新加入
	//记录的序号在它上面加1即可.
	vector<StGridToleranceRecord>::const_iterator itr = m_contRcdVec.begin();
	int iMaxIdx = 0;
	list<int> indexList;
	for(; itr != m_contRcdVec.end(); itr++)
	{
		if(itr->m_filename == filename)
		{
			indexList.push_back(itr->m_index);
		}
	}
	//sort;
	if(indexList.empty())
	{
		iMaxIdx = 0;
	}
	else
	{
		/*std::sort(indexList.begin(),indexList.end());*/
		indexList.sort();
		iMaxIdx = indexList.back();
	}
	return iMaxIdx;
}

bool CGridContentManager::editCurRcdField(const CString newCont)
{
	//前提是每一个标注实体只能有一条记录;
	//否则，下面的修改会把所有符合条件的记录全修改掉.
	if(! isFieldNewValValid(newCont))
	{
		return false;
	}
	CString originalVal;
	vector<StGridToleranceRecord>::iterator it = m_contRcdVec.begin();
	for(; it != m_contRcdVec.end(); it++)
	{
		if(it->m_csHandle == m_strHandleOfCurRcdToEdit 
			&& it->m_filename == m_filenameOfCurRcdToEdit
			&& it->m_strIndex == m_strIndexOfCurRcdToEdit)
		{
			switch(m_fieldToBeEdit)
			{
				case TolTableColumnStrIndex:
					originalVal = it->m_strIndex;
					if(originalVal != newCont)
					{
						it->m_strIndex = newCont;
						return true;
					}
					break;
				case TolTableColumnBaseMeasure:
					originalVal = it->m_strMeasure;
					if(originalVal != newCont)
					{
						it->m_strMeasure = newCont;
						return true;
					}
					break;
				case TolTableColumnTopTol:
					originalVal =  it->m_tolTop;
					if(originalVal != newCont)
					{
						it->m_tolTop = newCont;
						return true;
					}
					break;
				case TolTableColumnBottomTol:
					originalVal =  it->m_tolBottom;
					if(originalVal != newCont)
					{
						it->m_tolBottom = newCont;
						return true;
					}
					break;
				case TolTableColumnTolType:
					originalVal =  it->dimensionType;
					if(originalVal != newCont)
					{
						it->dimensionType = newCont;
						return true;
					}
					break;
				case TolTableColumnMemo:
					originalVal =  it->m_memo;
					if(originalVal != newCont)
					{
						it->m_memo = newCont;
						return true;
					}
					break;
				default:
					acutPrintf(_T("\n CGridContentManager::editCurRcdField error..."));
					return false;
					break;
			}
		}
	}
	return false;
}

bool CGridContentManager::isDigit(const CString strVal)
{
	//为空，返回false;
	if(strVal == _T(""))
	{
		return false;
	}
	//只能包含数字和.
	bool b1 = strVal.SpanIncluding(_T(".1234567890-")) == strVal;
	bool b2 = strVal.SpanIncluding(_T("1234567890-")) == strVal;
	//if(strVal.SpanIncluding(_T(".1234567890")) == strVal ||
	//	strVal.SpanIncluding(_T("1234567890")) == strVal)
	if(b1== false && b2 == false)
	{
		return false;
	}
	//如果第2位是.则开头不能是0
	if(strVal.GetAt(0) == _T('0'))
	{
		if(strVal.GetAt(1) != _T('.'))
		{
			return false;
		}
	}
	//开头不能是.
	if(strVal.GetAt(0) == _T('.'))
	{
		return false;
	}
	//不能有多个.
	if(strVal.Find(_T('.')) != strVal.ReverseFind(_T('.')))
	{
		return false;
	}
	return true;
}

bool CGridContentManager::isFieldNewValValid(const CString strVal)
{
	//检查几个字段是否为数字,其他不检查.
	switch(m_fieldToBeEdit)
	{
	case TolTableColumnStrIndex:
		return true; //不检查.
		break;
	case TolTableColumnBaseMeasure:
	case TolTableColumnTopTol:
	case TolTableColumnBottomTol:
		return m_objIsDigit.isdigit(strVal);
		break;
	case TolTableColumnTolType:
	case TolTableColumnMemo:
		return true; //不检查.
		break;
	default:
		acutPrintf(_T("\n CGridContentManager::isFieldNewValValid error..."));
		break;
	}
}

void CGridContentManager::locateCurRcdFiledToEdit(const CString filename,
												  const CString strHandle, 
												  const ETolTableField fieldIndex,
												  const CString strIndex)
{
	//保存起来.
	m_filenameOfCurRcdToEdit = filename;
	m_strHandleOfCurRcdToEdit = strHandle;
	m_fieldToBeEdit = fieldIndex;
	m_strIndexOfCurRcdToEdit = strIndex;
}

bool CGridContentManager::deleteRcdByStrIndex(const CString& strIndex)
{
	vector<StGridToleranceRecord>::iterator it = m_contRcdVec.begin();
	for(; it != m_contRcdVec.end(); it++)
	{
		if(it->m_strIndex == strIndex)
		{
			it = m_contRcdVec.erase(it);
			return true;
		}
	}
	return false; //到这里说明没找到记录.
}

bool CGridContentManager::isTheRcdExist(const StGridToleranceRecord& gridRowRcd)
{
	vector<StGridToleranceRecord>::const_iterator it = m_contRcdVec.begin();
	for(; it != m_contRcdVec.end(); it++)
	{
		if(it->m_filename == gridRowRcd.m_filename && it->m_csHandle == gridRowRcd.m_csHandle)
		{
			return true;
		}
	}
	return false;
}

bool CGridContentManager::saveGridContToTextFile(const CString csFilename)
{
	if(m_contRcdVec.empty())
	{
		return false;
	}
	//将grid内容存到text文件;
	//cstring to string
	string filename;
	cstringToString(csFilename,filename);
	//
	std::ofstream of;
	of.open(filename.c_str());
	if(of.is_open() == false)
	{
		of.close();
		acutPrintf(_T("\n not save,exit..."));
		return false;
	}

	vector<CString>  csContToSave;
	//save title
	collectTitleToSave(csContToSave);
	//grid content.
	collectGridContToSave(csContToSave);
	//cstring to string
	vector<string> strContToSave;
	WideCString2GbString(csContToSave,strContToSave);
	//	格式化
	//formatCsContToSave(csContToSave);
	formatStrContToSave(strContToSave);
	//
	std::string strCont;
	vector<std::string>::const_iterator it = strContToSave.begin();
	for(; it != strContToSave.end(); it++)
	{
		strCont = (*it);
		//cstringToString(csCont,strCont);
		//strCont = SomeFunction::GBToUTF8(strCont.c_str());
		of<<strCont;
	}
	of.flush();
	of.close();
	return true;
}

bool CGridContentManager::cstringToString(const CString csString,string& str)
{
	if(csString.IsEmpty() == true)
	{
		return false;
	}
	const wchar_t* ptr = LPCTSTR(csString);
	int csLen = wcstombs(NULL,ptr,0);
	char* tempStr = new char(csLen+1);
	wcstombs(tempStr,ptr,csLen+1);
	//const char* filename = tempStr;
	str = tempStr;
	delete tempStr;
	return true;
}

void CGridContentManager::collectTitleToSave(vector<CString> &csContToSave)
{
	//把titles前6个装进vector;
	int iCnt = 0;
	CString csCont;
	vector<CString>::const_iterator it = m_firstRowFields.begin();
	for(; it != m_firstRowFields.end(),iCnt < 6; it++)
	{
		csCont = (*it);
		csContToSave.push_back(csCont);
		iCnt++;
	}
}

void CGridContentManager::collectGridContToSave(vector<CString> &csContToSave)
{
	//grid内容装入csContToSave;
	int iCnt = 0;
	vector<StGridToleranceRecord>::const_iterator it = m_contRcdVec.begin();
	for(; it != m_contRcdVec.end(); it++)
	{
		csContToSave.push_back(it->m_strIndex);
		csContToSave.push_back(it->m_strMeasure);
		csContToSave.push_back(it->m_tolTop);
		csContToSave.push_back(it->m_tolBottom);
		csContToSave.push_back(it->dimensionType);
		csContToSave.push_back(it->m_memo);
	}
}

void CGridContentManager::formatCsContToSave(vector<CString> &csContToSave)
{
	//整理字符串，目的是使得输出到txt后比较整齐；
	//查找最长的长度;
	int iLenMax = 0;
	int csLen = 0;
	vector<CString>::const_iterator it = csContToSave.begin();
	for(; it != csContToSave.end(); it++)
	{
		csLen = it->GetLength();
		iLenMax = iLenMax > csLen? iLenMax:csLen;
	}
	//在每个字符串前面补空格，使得全部长度为iLenMax + 4;
	csLen = 0;
	vector<CString>::iterator itr = csContToSave.begin();
	for(itr = csContToSave.begin(); itr !=csContToSave.end(); itr++)
	{
		csLen = itr->GetLength();
		for(int i = 0; i < iLenMax - csLen; i++)
		{
			*itr = TEXT(" ") + *itr;
		}
	}
	//每隔6个，插入一个换行符.
	int iCnt = 0;
	CString newline = TEXT("\n");
	vector<CString>::const_iterator cnIt = csContToSave.begin();
	for(cnIt = csContToSave.begin(); cnIt !=csContToSave.end(); cnIt++,iCnt++)
	{
		if(iCnt % 6 == 0)
		{
			cnIt = csContToSave.insert(cnIt,newline);
			cnIt++;
		}
	}
	csContToSave.erase(csContToSave.begin());
}

void CGridContentManager::WideCString2GbString(const vector<CString> &csContToSave,
											   vector<std::string>& vecString)
{
	//把cstring换成string
	if(csContToSave.empty())
	{
		return;
	}
	vecString.clear();
	std::string tempStr;
	vector<CString>::const_iterator it = csContToSave.begin();
	for(; it != csContToSave.end(); it++)
	{
		if(it->IsEmpty())
		{
			tempStr = " ";
		}
		cstringToString(*it,tempStr);
		vecString.push_back(tempStr);
	}
}

void CGridContentManager::formatStrContToSave(vector<std::string> &strContToSave)
{
	//整理字符串，目的是使得输出到txt后比较整齐；
	//string里面存放的是gb码;
	//查找最长的长度;
	int iLenMax = 0;
	int csLen = 0;
	vector<std::string>::const_iterator it = strContToSave.begin();
	for(; it != strContToSave.end(); it++)
	{
		csLen = it->size();
		iLenMax = iLenMax > csLen? iLenMax:csLen;
	}
	//最长长度+3
	iLenMax += 1;
	//在每个字符串前面补空格，使得全部长度为iLenMax + 4;
	csLen = 0;
	vector<std::string>::iterator itr = strContToSave.begin();
	for(itr = strContToSave.begin(); itr !=strContToSave.end(); itr++)
	{
		csLen = itr->size();
		for(int i = 0; i < iLenMax - csLen; i++)
		{
			*itr = " " + *itr;
		}
	}
	//每隔6个，插入一个换行符.
	int iCnt = 0;
	std::string newline = "\n";
	vector<std::string>::const_iterator cnIt = strContToSave.begin();
	for(cnIt = strContToSave.begin(); cnIt !=strContToSave.end(); cnIt++,iCnt++)
	{
		if(iCnt % 6 == 0)
		{
			cnIt = strContToSave.insert(cnIt,newline);
			cnIt++;
		}
	}
	//结尾加一个\n
	strContToSave.push_back(newline);
	strContToSave.erase(strContToSave.begin());
}

bool CGridContentManager::switchOptOnToleranceRcd()
{
	//对形位公差记录的"切换"操作.
	//对当前选中的形位公差记录数值切换，*/如何切换？
	//若基本尺寸为0，直接把上公差填入“基本尺寸”栏;
	//若基本尺寸不为0,将之设置为0;
	CString csTopTol;
	vector<StGridToleranceRecord>::iterator it = m_contRcdVec.begin();
	for(; it != m_contRcdVec.end(); it++)
	{
		if(it->m_csHandle == m_strHandleOfCurRcdToEdit
			&& it->m_filename == m_filenameOfCurRcdToEdit
			&& it->m_strIndex == m_strIndexOfCurRcdToEdit)
		{
			csTopTol = it->m_tolTop;
			if(it->m_strMeasure.Compare(TEXT("0")) == 0)
			{
				it->m_strMeasure = csTopTol;
			}
			else
			{
				it->m_strMeasure = "0";
			}
			return true;
		}
	}
	return false; //运行到这里表示没找到记录.error.
}

//void CGridContentManager::editCurRcdField(const CString newCont)
//{
//	//获取原值;
//	CString orgVal = findValOfFileldToEdit();
//
//	//重写某个field的内容.
//	switch(m_fieldToBeEdit)
//	{
//	case TolTableColumnStrIndex:
//		break;
//	case TolTableColumnBaseMeasure:
//		break;
//	case TolTableColumnTopTol:
//		 break;
//	case TolTableColumnBottomTol:
//		break;
//	case TolTableColumnTolType:
//		break;
//	case TolTableColumnMemo:
//		break;
//	default:
//		acutPrintf(_T("\n CGridContentManager::editCurRcdField error..."));
//		break;
//	}
//}

void FilenameAndCode::setIndex(int idx)
{
	m_index = idx;
	prsStrCode();
}

void FilenameAndCode::prsIndex(deque<int>& idxDqu)
{
	int md = 0;
	int div = m_index;
	do
	{
		md = div % 26;
		div = div / 26;
		idxDqu.push_front(md);
	}while(div != 0);
}

void FilenameAndCode::prsStrCode()
{
	m_strCode.Empty();
	int i = 0;
	deque<int> idxDqu;
	idxDqu.clear();
	prsIndex(idxDqu);
	deque<int>::iterator it = idxDqu.begin();
	for(; it != idxDqu.end(); it++)
	{
		i = *it;
		m_strCode += (char)(i+ 65); //换成大写字母.
	}
}

FilenameAndCode::FilenameAndCode(const FilenameAndCode& fcStt)
{
	m_filename = fcStt.m_filename;
	m_index = fcStt.m_index;
	m_strCode = fcStt.m_strCode;
}

FilenameAndCode::FilenameAndCode()
{
}

CString AllCadFilenameRecord::prsStrCode(const CString filename)
{
	//查找是否已经存在该文件名;如果存在，返回编号;
	//如果不存在，添加它，并分配数字编号，返回字符串编号.
	CString strCode;
	strCode.Empty();
	if(isExistFileName(filename,strCode))
	{
		return strCode;
	}
	else
	{
		addFilenameRecord(filename,strCode);
		return strCode;
	}
}

bool AllCadFilenameRecord::isExistFileName(const CString filename,CString& strCode)
{
	vector<FilenameAndCode>::iterator it = m_filenameAndCode.begin();
	for(; it != m_filenameAndCode.end(); it++)
	{
		if(it->m_filename == filename)
		{
			strCode = it->m_strCode;
			return true;
		}
	}
	return false;
}

void AllCadFilenameRecord::addFilenameRecord(const CString filename,CString& strCode)
{
	//添加记录; 计算文件名的代号.
	int idx = m_filenameAndCode.size();
	FilenameAndCode fcSt;
	fcSt.m_filename = filename;
	fcSt.setIndex(idx); //计算了文件名代号.
	strCode = fcSt.m_strCode;

	m_filenameAndCode.push_back(fcSt);
}



bool sortByIndex(const StGridToleranceRecord& r1,const StGridToleranceRecord& r2)
{
	if(r1.m_strIndex < r2.m_strIndex)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ParseIsStringDigit::isdigit(const CString& strVal)
{
	if(isEmptyStr(strVal))
	{
		return false;
	}

	if(isOnlyDigitLetterIncluded(strVal) == false)
	{
		return false;
	}

	if(isValidWhenDotNotAtSecond(strVal) == false)
	{
		return false;
	}

	if(isDotAtFirst(strVal) == true)
	{
		return false;
	}

	if(isDotNotOnlyOne(strVal) == false)
	{
		return false;
	}
	if(isNegativeSymbolValid(strVal) == false)
	{
		return false;
	}

	return true;
}

bool ParseIsStringDigit::isEmptyStr(const CString& strVal)
{
	//Îª¿Õ£¬·µ»Øfalse;
	if(strVal == _T(""))
	{
		return false;
	}
}
bool ParseIsStringDigit::isNegativeSymbolValid(const CString& strVal) 
{
	if(isNegativeSymbolExist(strVal))
	{
		if(isNegativeSymbolOnlyOne(strVal))
		{
			if(isNegativeSymbolAtFirst(strVal))
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
	else
	{
		return true; //没有负号也是合理的.
	}
}
bool ParseIsStringDigit::isOnlyDigitLetterIncluded(const CString &strVal)
{
	//只能包含数字和.
	bool b1 = strVal.SpanIncluding(_T(".1234567890-")) == strVal;
	bool b2 = strVal.SpanIncluding(_T("1234567890-")) == strVal;
	//if(strVal.SpanIncluding(_T(".1234567890")) == strVal ||
	//	strVal.SpanIncluding(_T("1234567890")) == strVal)
	if(b1== false && b2 == false)
	{
		return false;
	}
}

bool ParseIsStringDigit::isValidWhenDotNotAtSecond(const CString &strVal)
{
	//如果第2位是.则开头不能是0或-
	if(strVal.GetAt(0) == _T('0'))
	{
		if(strVal.GetAt(1) != _T('.'))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if(strVal.GetAt(0) == _T('-') && strVal.GetAt(1) == _T('0'))
	{
		if(strVal.GetAt(2) != _T('.'))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool ParseIsStringDigit::isDotAtFirst(const CString &strVal)
{
	//¿ªÍ·²»ÄÜÊÇ.
	if(strVal.GetAt(0) == _T('.'))
	{
		return false;
	}
}

bool ParseIsStringDigit::isDotNotOnlyOne(const CString &strVal)
{
	//不能有多个.
	if(strVal.Find(_T('.')) != strVal.ReverseFind(_T('.')))
	{
		return false;
	}
	else
	{
		return true;
	}
}

//bool ParseIsStringDigit::isThereMoreThanOneNegativeSymbol(const CString& strVal)
//{
//	if(isNegativeSymbolExist(strVal) == false)
//	{
//		return false;
//	}
//	else 
//	{
//		if(isNegativeSymbolOnlyOne(strVal) == true)
//		{
//			return false;
//		}
//		else
//		{
//			return true;
//		}
//	}
//}

bool ParseIsStringDigit::isNegativeSymbolExist(const CString& strVal)
{
	//int idx = strVal.Find(_T('-'));
	if(strVal.Find(_T('-')) == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ParseIsStringDigit::isNegativeSymbolOnlyOne(const CString& strVal)
{
	if(isNegativeSymbolExist(strVal))
	{
		if(strVal.Find(_T('-')) == strVal.ReverseFind(_T('-')))
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
bool ParseIsStringDigit::isNegativeSymbolAtFirst(const CString& strVal)
{

	if(strVal.GetAt(0) == _T('-'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

