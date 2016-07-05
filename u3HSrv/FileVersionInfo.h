#pragma once 

//#include "Local.h"
#include "common.h"


class  FileVersionInfo
{
public:
	FileVersionInfo();
	~FileVersionInfo();
	void SetFilePath(string filepath);
	string GetFilePath() const;

	bool GetInfo();
	bool GetProductVersion(string& csVersion_o) const;
	bool GetFileVersion(string& csVersion_o) const;
	bool GetFileDescription(string& csFileDesc_o) const;
	bool GetCompanyName(string& csCompanyName_o) const;
	bool GetProductName(string& csProductName_o) const;
	bool GetComments(string& csComments_o) const;
	bool GetInternalName(string& csInternalName_o) const;
	bool GetLegalCopyright(string& csLegalCopyright_o) const;
	bool GetLegalTrademarks(string& csLegalTrademarks_o) const;
	bool GetPrivateBuild(string& csPrivateBuild_o) const;
	bool GetOriginalFileName(string& csOriginalFileName_o) const;
	bool GetSpecialBuild(string& csSpecialBuild_o) const;


private:

	typedef struct LANG_CP_t
	{
		WORD wLanguage;
		WORD wCodePage;
	} * PLANG_CP_t;

	bool GetLanguageList();
	bool ExtractInfo(LPCTSTR lpctszBlockString_i, LPVOID* ppvData_io) const;
	bool ExtractStringInfo(LPCTSTR lpctszBlockString_o, string& csInfo_o) const;

	string m_filePath;
	PLANG_CP_t m_pLang;
	unsigned char * m_pVersionData;

};


