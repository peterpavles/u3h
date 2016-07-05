#pragma once

//#include "Local.h"
#include "functionBase.h"
#include "common.h"
#include "string_utils.h"

class FilesManager : public FunctionBase
{

public:
	FilesManager();
	~FilesManager();

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;

	static bool hasEnding(string const &fullString, string const &ending); 

private:
	void SendDrivesList() const;
	void SendBrowsePath(string pathT) const;
	
	void RenameFileFromPath(string data);
	void DeleteFileFromPath(string data);
	void DownloadFileFromPath(string data);


};