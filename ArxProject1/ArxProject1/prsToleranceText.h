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
	Fgdt_p   //���깫���.
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
	GraphicSymbolType, //ͼ��.
	LineBreakType,              //���з�.
	VerticalSideOfFrameBoxType, //��Χ����ֱ���ţ��ָ���).
	CharStringValueType         //�ַ���.
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
	//������ʾͼ�α�ʶ���ַ���.
private:
	EToleranceSymbol m_symbol;
};
class BreakLineText:public ToleranceTextField
{
	//������ʾ���з����ַ���.

};
class NumericalValueText:public ToleranceTextField
{
	//������ʾ����ֵ���ַ���.
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
	//������ʾframe box��ֱ�ָ������ַ���.
private:
	int m_index; //�ڼ���.
};
class PrsToleranceText
{
	//������λ����ʵ����ַ�������ȡ������Ϣ.
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
		//�����ַ���;
		m_tolTextString = cstr;
		m_strlength = cstr.GetLength(); //��������Ѿ�ȥ����β'\0'.
		//��ȡ��ͬ�����ַ���;
		extractGraphicSymbol(cstr);
		extractNewline(cstr);
		extractVerticalSideOfFrmBox(cstr);
		extractNumericalValueText(cstr); //�������ǰ3��extract��.
		sortFieldVec(); //���ֶ��ڼ���������;
		sortNumericalValueTextField(); //����ֵ�ֶα��.
	}
	//bool rtnBaseMeasure();//��λ����û�л����ߴ磬ֻ�й���.
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
		//ȡ��Ϲ���ĵڶ�����λ����Ĺ���ֵ.
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
		//���ȡֵ�͸���һ��; �����������ֶ�ȡ��֮��;
		//�����ֶ�֮���ֵ������ֵ�ַ��������߱�ʾ��ֵ����ĸ�����ַ���);
		//sortFieldVec();
		//Ѱ�Ҽ����еĶ���֮���϶����Ϊ��ֵ�ַ���.
		vector<int> gapRcd;
		extractGapArr(gapRcd);
		//ͨ�����������ȡһ������ֵ�ַ����ֶ�.
		int i = 0;
		int j = 0;
		vector<int>::const_iterator it = gapRcd.begin();
		for(; it != gapRcd.end(); it++)
		{
			i = *it;
			it++;
			j = *it;
			if((j >= i) && (j < m_strlength)) //i����ʼλ�ã�j�ǽ���λ��.
			{				
				NumericalValueText* numericalTextObj = new NumericalValueText;
				//
				CString strMid = cstr.Mid(i,j-i +1);
				numericalTextObj->setText(strMid);
				//
				numericalTextObj->setStartAndEndPos(i,j);
				numericalTextObj->setType(CharStringValueType);
				//numericalTextObj->setIndex(iCnt);//���ﲻ��ţ��ڱ𴦱��.
				m_textArrObj.push_back(numericalTextObj);
			}
		}
	}

	void extractGapArr(vector<int>& gapRcd) 
	{
		//����.
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
		//�������ֶα��;
		int iVertSideCnt = 0; //��ֱ�ָ��������;
		int iNewlineCnt = 0; //���з������;
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
				//10����ǰ���10����ֱ�ָ���.
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