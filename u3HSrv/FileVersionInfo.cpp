#pragma once

#include "FileVersionInfo.h"


FileVersionInfo::FileVersionInfo() : m_pLang(nullptr), m_pVersionData(nullptr)
{

}


FileVersionInfo::~FileVersionInfo()
{
	delete[] m_pVersionData;
}


void FileVersionInfo::SetFilePath(string filepath)
{
	m_filePath = filepath;
}

string FileVersionInfo::GetFilePath() const
{
	return m_filePath;
}

bool FileVersionInfo::GetInfo()
{
	// Handle of version
	DWORD dwHandle = 0;

	// Get version size
	const  DWORD dwSize = GetFileVersionInfoSize(m_filePath.c_str(), &dwHandle);
	if (!dwSize) {
		// error
		return false;
	}


	// Version information 
	if (m_pVersionData) 
	{
		// free ptr array
		
	}

	// Allocate version data buffer
	m_pVersionData = new unsigned char[dwSize];
	if (!m_pVersionData) {
		// alloc failed
		return false;
	}

	if (!GetFileVersionInfo(m_filePath.c_str(), dwHandle, dwSize, m_pVersionData)) 
	{
		// free ptr array 
		delete[] m_pVersionData;
		return false;
	}

	// Extract languages list
	if (!GetLanguageList()) 
	{
		return false;
	}
	return true;
}


bool FileVersionInfo::GetLanguageList()
{
	// verify version
	if (!m_pVersionData) {
		// error
	}

	// Language block
	string block = "\\VarFileInfo\\Translation";

	// Get Lenght
	UINT ulenght = 0;
	const bool bresult = (VerQueryValue(m_pVersionData, block.c_str(), reinterpret_cast<void**>(&m_pLang), &ulenght) && ulenght);
	return bresult;
}

bool FileVersionInfo::ExtractInfo(LPCTSTR lpctszBlockString_i, LPVOID* ppvData_io) const
{
	if (!lpctszBlockString_i || !ppvData_io || !m_pVersionData || !m_pLang) 
	{
		return false;
	}



	//string block;
	std::unique_ptr<char[]> buffer(new char[MAX_PATH]);

	sprintf_s(buffer.get(), MAX_PATH, "\\StringFileInfo\\%04X%04X\\%s", m_pLang[0].wLanguage, m_pLang[0].wCodePage, lpctszBlockString_i);

	UINT uVersionDataLenght = 0;

	const bool bResult = VerQueryValue(m_pVersionData, buffer.get(), ppvData_io, &uVersionDataLenght);
	return uVersionDataLenght != 0 && bResult != 0;
}

bool FileVersionInfo::ExtractStringInfo(LPCTSTR lpctszBlockString_o, string& csInfo_o) const
{
	LPTSTR lptszInfo = nullptr;

	// Extraxt description information
	if (!ExtractInfo(lpctszBlockString_o, reinterpret_cast<LPVOID*>(&lptszInfo))) {
		return false;
	}

	csInfo_o = lptszInfo;
	return true;
}


bool FileVersionInfo::GetProductVersion(string& csVersion_o) const
{
	return ExtractStringInfo("ProductVersion", csVersion_o);
}


bool FileVersionInfo::GetFileVersion(string& csVersion_o) const
{
	return ExtractStringInfo("FileVersion", csVersion_o);
}



bool FileVersionInfo::GetFileDescription(string& csFileDesc_o) const
{
	return ExtractStringInfo("FileDescription", csFileDesc_o);
}


bool FileVersionInfo::GetCompanyName(string& csCompanyName_o) const
{
	return ExtractStringInfo("CompanyName", csCompanyName_o);
}



bool FileVersionInfo::GetProductName(string& csProductName_o) const
{
	return ExtractStringInfo("ProductName", csProductName_o);
}



bool FileVersionInfo::GetComments(string& csComments_o) const
{
	return ExtractStringInfo("Comments", csComments_o);
}



bool FileVersionInfo::GetInternalName(string& csInternalName_o) const
{
	return ExtractStringInfo("InternalName", csInternalName_o);
}



bool FileVersionInfo::GetLegalCopyright(string& csLegalCopyright_o) const
{
	return ExtractStringInfo("LegalCopyright", csLegalCopyright_o);
}



bool FileVersionInfo::GetLegalTrademarks(string& csLegalTrademarks_o) const
{
	return ExtractStringInfo("LegalTrademarks", csLegalTrademarks_o);
}


bool FileVersionInfo::GetPrivateBuild(string& csPrivateBuild_o) const
{
	return ExtractStringInfo("PrivateBuild", csPrivateBuild_o);
}


bool FileVersionInfo::GetOriginalFileName(string& csOriginalFileName_o) const
{
	return ExtractStringInfo("OrignalFileName", csOriginalFileName_o);
}



bool FileVersionInfo::GetSpecialBuild(string& csSpecialBuild_o) const
{
	return ExtractStringInfo("SpecialBuild", csSpecialBuild_o);
}
