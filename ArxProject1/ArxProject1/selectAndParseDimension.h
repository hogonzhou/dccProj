#pragma once

//#include <stdlib.h>
//#include <ctype.h>
//#include <aced.h>
//#include <rxregsvc.h>
//#include <adscodes.h>
//#include <dbsymtb.h>
//#include <dblayout.h>
//#include <dbapserv.h>
//#include <acdblmgr.h>
//#include <dbents.h>
//#include <dbpl.h>
//#include <dbid.h>
//#include <acutads.h>
//#include <string.h>
//#include <stdio.h>
//#include <acedads.h>
//#include <dbmain.h>
//#include <geplin2d.h>
//#include <math.h>
//#include <acarray.h>
//#include <dbdict.h>
//#include <dbgroup.h>
//#include <rxobject.h>
//#include <gelnsg2d.h>
//#include <gevec2d.h>

#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include "prsToleranceText.h"
#include "someFunc.h"
using namespace std;

enum ETolTableField
{
	TolTableColumnStrIndex,
	TolTableColumnBaseMeasure,
	TolTableColumnTopTol,
	TolTableColumnBottomTol,
	TolTableColumnTolType,
	TolTableColumnMemo,
	TolTableColumnFilename,
	TolTableColumnDimEntHandle,
	TolTableColumnDimUnkown
};

struct FilenameAndCode
{
	//文件名;
	CString m_filename;
	//序号;
	unsigned int m_index;
	//按序号转换成字母，比如1对应A，2对应B，26对应Z，27对应AA，etc;
	CString m_strCode;
public:
	void setIndex(int idx);
	FilenameAndCode();
	FilenameAndCode(const FilenameAndCode& fcStt);
private:
	void prsIndex(deque<int>& idxDqu);
	void prsStrCode();
};
class AllCadFilenameRecord
{
public:
	CString prsStrCode(const CString filename);
private:
	bool isExistFileName(const CString filename,CString& strCode);
	void addFilenameRecord(const CString filename,CString& strCode);
private:
	vector<FilenameAndCode> m_filenameAndCode;
};



struct StGridToleranceRecord
{
	//文件名代号+序号 = 编号,须保证其唯一性.
	CString m_strIndex;
	CString m_strMeasure;
	CString m_tolTop;
	CString m_tolBottom;
	CString dimensionType;
	CString m_memo;
	//全路径文件名.
	CString m_filename; 
	//文件名的代号（唯一);
	CString m_filenameCode;
	 //记录handle,避免重复提取标注实体信.
	CString m_csHandle;
	//分配的序号:在相同文件名的所有记录中的序号,因为中途可能有记录删除，序号并不要求连续.
	int m_index; 
	//排序用.
	friend bool sortByIndex(const StGridToleranceRecord& r1,const StGridToleranceRecord& r2);
	//friend bool sortByMeasure();
};
struct cmpByStrIndex
{
	bool operator()(const StGridToleranceRecord& r1,const StGridToleranceRecord& r2)
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
};
class CGetDimensionInfo
{
public:
	CGetDimensionInfo();
	~CGetDimensionInfo();
	//bool getAcDbRotatedDimensionInfo(ads_name unitSS,StGridToleranceRecord& dimInfoVec);
	bool getAcDbDimensionInfo(ads_name unitSS,StGridToleranceRecord& dimInfoVec);
	bool getAcDbFcfTolInfo(ads_name unitSS,
							StGridToleranceRecord& dimInfoVec,
							vector<StGridToleranceRecord>& tolRcdVect);


	CString getAcDbHandleCstr(const AcDbEntity* pEnt);
private:
	bool isAcDbRotatedDimension(ads_name unitSS,AcDbRotatedDimension*& pAlignDim);
	bool isAcDbDimension(ads_name unitSS,AcDbDimension*& pAcdbDim);
	bool isAcDbFcf(ads_name unitSS,AcDbFcf*& pAcdbDim);
	bool prsAcDbDimensionType(const AcDbDimension* acdbDimPtr,StGridToleranceRecord& dimInfoVec);
	void cutAffixZeroChar(CString& cs);
	double  angleToRadian(const double& ang);
	double  radianToAngle(const double& ang);
	double  degreeToMinute(const double& fDegree);
	bool isZero(const double& fVal);
	bool isGreaterThanZero(const double& fVal);
	void fillWithFcfTolVal(StGridToleranceRecord &dimInfoVec, const double firstTol);
private:
	AcDbDimStyleTableRecord* m_pDimStyleRcd;
	PrsToleranceText m_prsTolTextFieldObj;
};

//class ParseIsStringDigit
//{
//public:
//	bool isdigit(const CString& cs);
//private:
//	bool isDotNotOnlyOne(const CString &strVal);
//	bool isDotAtFirst(const CString &strVal);
//	bool isValidWhenDotNotAtSecond(const CString &strVal);
//	bool isOnlyDigitLetterIncluded(const CString &strVal);
//	bool isEmptyStr(const CString& strVal);
//	bool isNegativeSymbolExistAndAtFirst(const CString& strVal);
//	bool isNegativeSymbolExist(const CString& strVal);
//	bool isNegativeSymbolOnlyOne(const CString& strVal);
//
//};
class CGridContentManager
{
	//grid ctrl内容管理.
public:
	CGridContentManager();
	void  getFirstRowFields(vector<CString>& firstRowFieldsVec);
	bool getColumnCount(int& nColumnCount);
	void insertRowRcd(const StGridToleranceRecord& gridRowRcd);
	bool isTheRcdExist(const StGridToleranceRecord& gridRowRcd);
	void deleteRowRcd(const CString strCode);
	const vector<StGridToleranceRecord>& rtnAllRcds();
	int calStrIndex(const CString& filename);
	bool deleteRcdByStrIndex(const CString& strIndex);
	//void editCurRcdField(const CString newCont);
	void locateCurRcdFiledToEdit(const CString filename,
		const CString strHandle,		
		const ETolTableField fieldIndex,
		const CString strIndex);
	bool editCurRcdField(const CString newCont);
	bool switchOptOnToleranceRcd();
	bool saveGridContToTextFile(const CString filename);
private:
	bool isFieldNewValValid(const CString strVal);
	bool cstringToString(const CString csString,string& str);
	void collectGridContToSave(vector<CString> &csContToSave);
	void WideCString2GbString(const vector<CString> &csContToSave,vector<std::string>& vecString);
	void collectTitleToSave(vector<CString> &csContToSave);
	void formatCsContToSave(vector<CString> &csContToSave);
	void formatStrContToSave(vector<std::string> &strContToSave);
	bool isDigit(const CString strVal);

private:
	vector<CString> m_firstRowFields;
	vector<StGridToleranceRecord> m_contRcdVec;
	//用来定位当前正在被修改的记录及字段.
	CString m_filenameOfCurRcdToEdit;
	CString m_strHandleOfCurRcdToEdit;
	ETolTableField m_fieldToBeEdit;
	CString m_strIndexOfCurRcdToEdit;
	//
	ParseIsStringDigit m_objIsDigit;
};