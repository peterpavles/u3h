#pragma once

//#include "Local.h"


//class WMIPoller;



#include "functionBase.h"

#include "FileVersionInfo.h"


typedef BOOL(WINAPI *LPQueryFullProcessImageName)(HANDLE hProcess, DWORD dwFlags, LPSTR lpExeName, PDWORD lpdwSize);


class ProcessManager : public FunctionBase
{
public:
	ProcessManager();
	~ProcessManager();

	
	void SendBasicProcessInfos(int pid, vector<string> queryValues);

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;

private:
	LPQueryFullProcessImageName fpQueryFullProcessImageName;


	void SendProcessList(vector<string> queryValues);
	void KillProcessFromPID(string pid);


	string GetProcessOwner(HANDLE hProcess);
	string GetProcessImageFullPath(HANDLE hProcess);
	string GetProcessImageDescription(string imagePath);
	string GetProcessMemoryUsage(HANDLE hProcess);

};

