//#include "prsToleranceText.h"
//
//void PrsToleranceText::inputText(const CString cstr)
//{
//	m_tolStr = cstr;
//}
#pragma  once
#include "tchar.h"
#include <vector>
#include <cstdlib>
#include "stdlib.h"
#include "atlstr.h"
//#include "selectAndParseDimension.h"
using namespace std;

class ParseIsStringDigit
{
public:
	bool isdigit(const CString& cs);
private:
	bool isDotNotOnlyOne(const CString &strVal);
	bool isDotAtFirst(const CString &strVal);
	bool isValidWhenDotNotAtSecond(const CString &strVal);
	bool isOnlyDigitLetterIncluded(const CString &strVal);
	bool isEmptyStr(const CString& strVal);
	//bool isThereMoreThanOneNegativeSymbol(const CString& strVal);
	bool isNegativeSymbolValid(const CString& strVal);
	bool isNegativeSymbolExist(const CString& strVal);
	bool isNegativeSymbolOnlyOne(const CString& strVal);
	bool isNegativeSymbolAtFirst(const CString& strVal);
};



enum EToleranceSymbol
{
	Fgdt_j,
	Fgdt_r,
	Fgdt_i,
	Fgdt_f,
	Fgdt_b,
	Fgdt_a,
	Fgdt_g,
	Fgdt_c,
	Fgdt_e,
	Fgdt_u,
	Fgdt_d,
	Fgdt_k,
	Fgdt_h,
	Fgdt_t,
	Fgdt_n,
	Fgdt_m,
	Fgdt_l,
	Fgdt_s,
	Fgdt_p   //延申公差带.
};
//enum ENumericalTextType
//{
//	ToleranceOneOne,
//	ToleranceOneTwo,
//	BaseOneOne,
//	BaseOneTwo,
//	BaseOneThree,
//	ToleranceTwoOne,
//	ToleranceTwoTwo,
//	BaseTwoOne,
//	BaseTwoTwo,
//	BaseTwoThree,
//	HeigthValue,
//	BaseSymbolSign
//};
enum EAcDbFcfTextBlockType
{
	GraphicSymbolType, //图列.
	LineBreakType,              //换行符.
	VerticalSideOfFrameBoxType, //包围框竖直符号（分隔符).
	CharStringValueType         //字符串.
};

class ToleranceTextField
{
public:
	virtual bool setStartAndEndPos(const int startPos,const int endPos)
	{
		m_startPos = startPos;
		m_endPos = endPos;
		return true;
	}
	virtual bool getStartAmdEndPos(int& startPos, int& endPos) const
	{
		startPos = m_startPos;
		endPos = m_endPos;
		return true;
	}
	virtual bool setIndex(const int idx)
	{
		m_index = idx;
		return true;
	}
	virtual bool setText(const CString cs)
	{
		m_strCont = cs;
		return true;
	}
	virtual bool setType(const EAcDbFcfTextBlockType et)
	{
		m_stringType = et;
		return true;
	}
	virtual EAcDbFcfTextBlockType rtnType() const 
	{
		return m_stringType;
	}
	virtual CString rtnStrCont()
	{
		return	m_strCont;
	}
	virtual int rtnIndex()
	{
		return m_index;
	}
public:
	CString m_strCont;
	int m_startPos;
	int m_endPos;
	int m_index;
	EAcDbFcfTextBlockType m_stringType;
};
class GraphicSymbolText:public ToleranceTextField
{
	//用来表示图形标识的字符串.
private:
	EToleranceSymbol m_symbol;
};
class BreakLineText:public ToleranceTextField
{
	//用来表示换行符的字符串.

};
class NumericalValueText:public ToleranceTextField
{
	//用来表示测量值的字符串.
public:
	bool setIndex(const int idx)
	{
		m_index = idx;
		return true;
	}
private:
	//ENumericalTextType m_txtType;	
};
class VerticalSideOfFrameBoxText:public ToleranceTextField
{
	//用来表示frame box竖直分隔符的字符串.
private:
	int m_index; //第几个.
};
class PrsToleranceText
{
	//分析形位公差实体的字符串，提取公差信息.
public:
	PrsToleranceText(){};
	~PrsToleranceText()
	{
		releaseTextObj();
	}
	void releaseTextObj()
	{
		vector<ToleranceTextField*>::const_iterator it = m_textArrObj.begin();
		for(; it != m_textArrObj.end(); it++)
		{
			ToleranceTextField* ttfObj = (*it);
			if(ttfObj != NULL)
			{
				delete ttfObj;
				ttfObj = NULL;
			}
		}
		m_textArrObj.clear();
	}
	void inputText(const CString cstr)
	{
		//clear
		releaseTextObj();
		//分析字符串;
		m_tolTextString = cstr;
		m_strlength = cstr.GetLength(); //这个函数已经去掉结尾'\0'.
		//提取不同类型字符串;
		extractGraphicSymbol(cstr);
		extractNewline(cstr);
		extractVerticalSideOfFrmBox(cstr);
		extractNumericalValueText(cstr); //必须放在前3个extract后.
		sortFieldVec(); //各字段在集合里排序;
		sortNumericalValueTextField(); //给数值字段编号.
	}
	//bool rtnBaseMeasure();//形位公差没有基本尺寸，只有公差.
	bool rtnFirstAndSecondTol(double& firstTol,double& secondTol)
	{
		bool b1 = rtnTolByIndex(firstTol,1);
		bool b2 = rtnTolByIndex(secondTol,2);
		return b1&&b2;
	}
	bool rtnFirstTol(double& firstTol)
	{
		bool b1 = rtnTolByIndex(firstTol,1);
		return b1;
	}
	bool rtnSecondTol(double& secondTol)
	{
		//取组合公差的第二个形位公差的公差值.
		bool b2 = rtnTolByIndex(secondTol,6); //
		return b2;
	}
private:
	bool rtnTolByIndex(double& firstTol,const int numTol)
	{
		vector<ToleranceTextField*>::const_iterator it = m_textArrObj.begin();
		for(; it != m_textArrObj.end(); it++)
		{
			if((*it)->rtnType() == CharStringValueType && (*it)->rtnIndex() == numTol)
			{
				if(m_prsIsDigit.isdigit((*it)->rtnStrCont()))
				{
					firstTol = _wtof((*it)->rtnStrCont().GetBuffer());
					return true;
				}
				else 
				{
					firstTol = 0;
					return false;
				}
			}
		}
		firstTol = 0;
		return false;
	}
	void extractGraphicSymbol(const CString& cstr)
	{
		CString csTemp;
		int iIndex = 0;
		int searchStartPos = 0;
		int idx = 0;
		while(idx >= 0)
		{
			idx = cstr.Find(_T("Fgdt;"),searchStartPos);
			if(idx >= 0)
			{
				GraphicSymbolText* graphicSymObj = new GraphicSymbolText;
				//
				graphicSymObj->setStartAndEndPos(idx -2,idx+6);
				graphicSymObj->setType(GraphicSymbolType);
				//
				//graphicSymObj->se
				//
				csTemp.Empty();
				csTemp = cstr.Mid(idx-2,idx+6);
				graphicSymObj->setText(csTemp);
				//
				graphicSymObj->setIndex(iIndex);
				iIndex++;
				//
				m_textArrObj.push_back(graphicSymObj);
				//
				searchStartPos = idx+7;
			}
		}
	}
	void extractNewline(const CString& cstr)
	{
		CString csTemp;
		int searchStartPos = 0;
		int idx = 0;
		while(idx >= 0)
		{
			idx = cstr.Find(_T("\n"),searchStartPos);
			if(idx >= 0)
			{
				BreakLineText* breaklineObj = new BreakLineText;
				breaklineObj->setStartAndEndPos(idx,idx);
				breaklineObj->setType(LineBreakType);
				m_textArrObj.push_back(breaklineObj);
				searchStartPos = idx+1;
			}
		}
	}
	void extractVerticalSideOfFrmBox(const CString& cstr)
	{
		int searchStartPos = 0;
		int idx = 0;
		while(idx >= 0)
		{
			idx = cstr.Find(_T("%%v"),searchStartPos);
			if(idx >= 0)
			{
				VerticalSideOfFrameBoxText* verticalSideObj = new VerticalSideOfFrameBoxText;
				verticalSideObj->setStartAndEndPos(idx,idx+2);
				verticalSideObj->setType(VerticalSideOfFrameBoxType);
				m_textArrObj.push_back(verticalSideObj);
				searchStartPos = idx+3;
			}
		}
	}
	void extractNumericalValueText(const CString& cstr)
	{
		//这个取值就复杂一点; 必须在其他字段取完之后;
		//其他字段之间的值就是数值字符串（或者表示数值的字母代号字符串);
		//sortFieldVec();
		//寻找集合中的对象之间空隙，即为数值字符串.
		vector<int> gapRcd;
		extractGapArr(gapRcd);
		//通过上面数组获取一个个数值字符串字段.
		int i = 0;
		int j = 0;
		vector<int>::const_iterator it = gapRcd.begin();
		for(; it != gapRcd.end(); it++)
		{
			i = *it;
			it++;
			j = *it;
			if((j >= i) && (j < m_strlength)) //i是起始位置，j是结束位置.
			{				
				NumericalValueText* numericalTextObj = new NumericalValueText;
				//
				CString strMid = cstr.Mid(i,j-i +1);
				numericalTextObj->setText(strMid);
				//
				numericalTextObj->setStartAndEndPos(i,j);
				numericalTextObj->setType(CharStringValueType);
				//numericalTextObj->setIndex(iCnt);//这里不编号，在别处编号.
				m_textArrObj.push_back(numericalTextObj);
			}
		}
	}

	void extractGapArr(vector<int>& gapRcd) 
	{
		//排序.
		sortFieldVec();
		//
		gapRcd.clear();
		int strlength = m_strlength;
		int startPos;
		int endPos;
		//vector<int> gapRcd;
		gapRcd.push_back(0);
		vector<ToleranceTextField*>::const_iterator it = m_textArrObj.begin();
		for(; it != m_textArrObj.end(); it++)
		{
			(*it)->getStartAmdEndPos(startPos,endPos);
			gapRcd.push_back(startPos-1);
			gapRcd.push_back(endPos+1);
		}
		gapRcd.push_back(strlength-1);//
	}
	void sortNumericalValueTextField()
	{
		//给文字字段编号;
		int iVertSideCnt = 0; //竖直分隔符的序号;
		int iNewlineCnt = 0; //换行符的序号;
		int iNumericTextFieldCnt = 0;
		vector<ToleranceTextField*>::iterator it = m_textArrObj.begin();
		for(; it != m_textArrObj.end(); it++)
		{
			if((*it)->rtnType() == VerticalSideOfFrameBoxType)
			{
				iVertSideCnt++;
				iNumericTextFieldCnt++;
			}
			if((*it)->rtnType() == LineBreakType)
			{
				iNewlineCnt++;
				//10代表前面的10个竖直分隔符.
				if(iNewlineCnt == 1)
				{
					iNumericTextFieldCnt = 5 * iNewlineCnt;
				}
				else if(iNewlineCnt == 2)
				{
					iNumericTextFieldCnt = 5*iNewlineCnt + 1;
				}
				else if(iNewlineCnt == 3)
				{
					iNumericTextFieldCnt = 5*2 + 2;
				}
			}
			if((*it)->rtnType() == CharStringValueType)
			{
				(*it)->setIndex(iNumericTextFieldCnt);
			}
		}
	}
	static bool sortCmpFun(const ToleranceTextField* f1,const ToleranceTextField* f2)
	{
		int strF1;
		int endF1;
		int strF2;
		int endF2;
		f1->getStartAmdEndPos(strF1,endF1);
		f2->getStartAmdEndPos(strF2,endF2);
		if(strF1 < strF2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool sortFieldVec()
	{
		std::sort(m_textArrObj.begin(),m_textArrObj.end(),PrsToleranceText::sortCmpFun);
		return true;
	}
private:
	vector<ToleranceTextField*> m_textArrObj;
	CString m_tolTextString;
	int m_strlength;
	ParseIsStringDigit m_prsIsDigit;
};


//#include "prsToleranceText.h"
//
//void PrsToleranceText::inputText(const CString cstr)
//{
//	m_tolStr = cstr;
//}