#pragma  once

#include <iostream>
#include <streambuf>
#include <string>
#include <vector>
#include <fstream>
#include "Shlwapi.h"
//#include "winbase.h"
#include "CJsonObject.hpp"
using namespace std;
namespace DccDimInfoEx
{
	const CString DccDimAppJsonName = TEXT("tolInforExtractAppSettings.json");
	const double Pi = 3.14159265358979323846;
}
class SomeFunction
{
public:
	static string UTF8ToGB(const char* str)
	{
		string result;
		WCHAR *strSrc;
		LPSTR szRes;

		//获得临时变量的大小
		int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
		strSrc = new WCHAR[i + 1];
		MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

		//获得临时变量的大小
		i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
		szRes = new CHAR[i + 1];
		WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

		result = szRes;
		delete[]strSrc;
		delete[]szRes;

		return result;
	}

	static char* GB2312ToUtf8(const char* gb2312)
	{
		//函数：GB2312ToUtf8
		//功能：GB2312转换为UTF-8
		int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
		len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
		if (wstr) delete[] wstr;
		return str;
	}
	static string GBToUTF8(const char* str)
	{
		char* rsl = GB2312ToUtf8(str);
		string strUtf8 = rsl;
		delete[] rsl;
		return strUtf8;
	}
	static CString selectFilePathByUser(const CString filterStr) /* ?匮≡竦奈募?*/
	{
		CString strFile = _T("");

		//CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.txt)|(*.json)|All Files (*.*)|*.*||"), NULL);
		CAcUiFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY,filterStr,NULL);
			//_T("Describe Files (*.txt)|(*.json)|All Files (*.*)|*.*||"), NULL);

		if (dlgFile.DoModal())
		{
			strFile = dlgFile.GetPathName();
		}

		return strFile;
	} 
	void bakSentances()
	{
		//备份用的，本函数并无实用.
		// Add your code for command hugoGridDialog1._findDccPath here
		const char* filename = "D:/A图形数据处理/tolInforExtractSettings.json";
		std::string str((std::istreambuf_iterator<char>(std::ifstream(filename, std::ios::in | std::ios::binary).rdbuf())),std::istreambuf_iterator<char>());
		neb::CJsonObject oJson(str);
		std::string strA = oJson["linkCadPath1"].ToString();

		std::string strB = SomeFunction::UTF8ToGB(strA.c_str());

		//std::string strC = SomeFunction::UTF8ToGB(strB.c_str());

		//WinExec(strB.c_str(), SW_SHOW);

		//std::wstring wContext = L"";
		//int len = MultiByteToWideChar(CP_ACP, 0, strA.c_str(), strA.size(), NULL, 0);
		//WCHAR* buffer = new WCHAR[len + 1];
		//MultiByteToWideChar(CP_ACP, 0, strA.c_str(), strA.size(), buffer, len);
		//buffer[len] = '\0';
		//wContext.append(buffer);
		//delete[] buffer;
		//acutPrintf(wContext.c_str());
		//select file path dialog:
		//CString fullpath = SomeFunction::selectFilePathByUser();
		//acutPrintf(fullpath);

		CString dllPath = SomeFunction::findDllFullPath();
		acutPrintf(dllPath);

		SomeFunction::getDefaultTextSaveFullPath();
	}
	//string GetCurrentModule()
	//{  
	//	HMODULE hModule = NULL;
	//	char DLLPATH[MAX_PATH + 1] = { 0 };
	//	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
	//		(LPCTSTR)GetCurrentModule,
	//		&hModule);

	//	//----
	//	::GetModuleFileName(hModule, DLLPATH, MAX_PATH); 
	//	string DllPath = DLLPATH; 
	//	return DllPath;
	//}
	static HMODULE GetSelfModuleHandle()
	{
		MEMORY_BASIC_INFORMATION mbi;
		return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
	}

	static CString findDllFullPath()
	{
		static  WCHAR szDestPath[_MAX_PATH] = { 0 };
		if (!GetModuleFileName(GetSelfModuleHandle(), szDestPath, _MAX_PATH))
		{
			return TEXT("");
		}
		else
		{
			CString csRsl = szDestPath;
			return szDestPath;
		}
	}
	static CString findJsonFullPath()
	{
		CString dllFullPath = findDllFullPath();
		//去掉末尾文件名;
		int iPos = dllFullPath.ReverseFind('\\');
		dllFullPath = dllFullPath.Mid(0,iPos+1);
		//替换'\'为'/'
		dllFullPath.Replace('\\','/');
		dllFullPath += DccDimInfoEx::DccDimAppJsonName;
		return dllFullPath;
	}
	static std::string getDccExeFullPath()
	{
		//从json文件中获取默认text保存路径.
		CString jsonFullPathName = findJsonFullPath();
		const wchar_t* ptr = LPCTSTR(jsonFullPathName);

		char tempStr[256] = {};
		wcstombs(tempStr,ptr,256);
		//const char* filename = (LPCSTR)ptr;
		const char* filename = tempStr;
		//std::string filename = ptrB;
		std::string str((std::istreambuf_iterator<char>(std::ifstream(filename, std::ios::in | std::ios::binary).rdbuf())),std::istreambuf_iterator<char>());
		neb::CJsonObject oJson(str);
		//std::string strA = oJson["ToleranceInfoTextFilePath"].ToString();
		std::string strA;
		oJson.Get("DccExeFilePath",strA);

		strA = SomeFunction::UTF8ToGB(strA.c_str());
		return strA;
	}
	static CString getDefaultTextSaveFullPath()
	{
		//从json文件中获取默认text保存路径.
		CString jsonFullPathName = findJsonFullPath();
		const wchar_t* ptr = LPCTSTR(jsonFullPathName);
		
		char tempStr[256] = {};
		wcstombs(tempStr,ptr,256);
		//const char* filename = (LPCSTR)ptr;
		const char* filename = tempStr;
		//std::string filename = ptrB;
		std::string str((std::istreambuf_iterator<char>(std::ifstream(filename, std::ios::in | std::ios::binary).rdbuf())),std::istreambuf_iterator<char>());
		neb::CJsonObject oJson(str);
		//std::string strA = oJson["ToleranceInfoTextFilePath"].ToString();
		std::string strA;
		oJson.Get("ToleranceInfoTextFilePath",strA);

		strA = SomeFunction::UTF8ToGB(strA.c_str());

		CString defautlSavePath = strA.c_str();
		return defautlSavePath;
	}
	//static CString getDefaultDccExeFullPath()
	//{
	//	//从json文件中获取默认text保存路径.
	//	CString jsonFullPathName = findJsonFullPath();
	//	const wchar_t* ptr = LPCTSTR(jsonFullPathName);

	//	char tempStr[256] = {};
	//	wcstombs(tempStr,ptr,256);
	//	//const char* filename = (LPCSTR)ptr;
	//	const char* filename = tempStr;
	//	//std::string filename = ptrB;
	//	std::string str((std::istreambuf_iterator<char>(std::ifstream(filename, std::ios::in | std::ios::binary).rdbuf())),std::istreambuf_iterator<char>());
	//	neb::CJsonObject oJson(str);
	//	//std::string strA = oJson["ToleranceInfoTextFilePath"].ToString();
	//	std::string strA;
	//	oJson.Get("DccExeFilePath",strA);

	//	strA = SomeFunction::UTF8ToGB(strA.c_str());

	//	CString defautlSavePath = strA.c_str();
	//	return defautlSavePath;
	//}
	static void saveTextFullPathToJson(const CString filePath)
	{
		//从json文件中获取默认text保存路径.
		CString jsonFullPathName = findJsonFullPath();
		const wchar_t* ptr = LPCTSTR(jsonFullPathName);
		char tempStr[256] = {};
		wcstombs(tempStr,ptr,256);

		//const char* filename = (LPCSTR)ptr;
		const char* filename = tempStr;
		std::string str((std::istreambuf_iterator<char>(std::ifstream(filename, std::ios::in | std::ios::binary).rdbuf())),std::istreambuf_iterator<char>());
		neb::CJsonObject oJson(str);

		//cstring to string;
		const wchar_t* csPath = LPCTSTR(filePath);
		char tempStrA[256] = {};
		wcstombs(tempStrA,csPath,256);
		std::string  newPath = tempStrA;
		//修改json文件
		newPath = SomeFunction::GBToUTF8(newPath.c_str());
		//oJson["ToleranceInfoTextFilePath"] = newPath;
		oJson.Replace("ToleranceInfoTextFilePath",newPath);
		//save to json.
		std::string newCont = oJson.ToString();
		
		std::ofstream of;
		of.open(filename, std::ios::out);
		of.write(newCont.data(),newCont.size());
		of.flush();
		of.close();
	}
	static void saveDccExeFullPathToJson(const CString filePath)
	{
		//从json文件中获取默认text保存路径.
		CString jsonFullPathName = findJsonFullPath();
		const wchar_t* ptr = LPCTSTR(jsonFullPathName);
		char tempStr[256] = {};
		wcstombs(tempStr,ptr,256);

		//const char* filename = (LPCSTR)ptr;
		const char* filename = tempStr;
		std::string str((std::istreambuf_iterator<char>(std::ifstream(filename, std::ios::in | std::ios::binary).rdbuf())),std::istreambuf_iterator<char>());
		neb::CJsonObject oJson(str);

		//cstring to string;
		const wchar_t* csPath = LPCTSTR(filePath);
		char tempStrA[256] = {};
		wcstombs(tempStrA,csPath,256);
		std::string  newPath = tempStrA;
		//修改json文件
		newPath = SomeFunction::GBToUTF8(newPath.c_str());
		//oJson["ToleranceInfoTextFilePath"] = newPath;
		oJson.Replace("DccExeFilePath",newPath);
		//save to json.
		std::string newCont = oJson.ToString();

		std::ofstream of;
		of.open(filename, std::ios::out);
		of.write(newCont.data(),newCont.size());
		of.flush();
		of.close();
	}
	static bool createDir(const CString dirPath)
	{
		if (PathIsDirectoryW(dirPath) == false)
		{
			int bFlag = CreateDirectoryW(dirPath, 0); //
			if(bFlag == 1)
			{
				return true;
			}
			else
			{
				acutPrintf(_T("\n 创建目录 %s 失败..."),dirPath);
				return false;
			}
		}
		else //
		{
			return true;
		}
	}
};

