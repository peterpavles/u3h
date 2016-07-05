//#include "WMIPoller.h"
#include "ProcessManager.h"

#include "misc_utils.h"

ProcessManager::ProcessManager()
{
	HMODULE hDLL = LoadLibrary("kernel32.dll");
	fpQueryFullProcessImageName = (LPQueryFullProcessImageName)GetProcAddress(hDLL, "QueryFullProcessImageNameA"); //ANSI version
}


ProcessManager::~ProcessManager()
{
}

int ProcessManager::FunctionIdentity()
{ 
	return _processManager;
}

void ProcessManager::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
		case processList:
		{
			char * v[] = { "Name", "ProcessId", };
			vector<string> vv(v, end(v));
			SendProcessList(vv);
			break;
		}
		default:
		{
			break;
		}
	}
	freePacket(packet);
}

void ProcessManager::SendProcessList(vector<string> queryValues) 
{
	string procArray;

	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;


	// Take snapshot of all processes on the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		// error
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve informations about the first process,
	// and exit if unsuccessful.
	if (!Process32First(hProcessSnap, &pe32)) {
		// error
	}

	

	while (Process32Next(hProcessSnap, &pe32)) {
		string procName = pe32.szExeFile;
		string pid = to_string(pe32.th32ProcessID);
		string owner;
		string mem;
		string description;

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
		if (hProcess == INVALID_HANDLE_VALUE) {
			owner = "<error>";
			mem = "<error>";
		}
		else {
			owner = GetProcessOwner(hProcess);
			mem = GetProcessMemoryUsage(hProcess);
		}

		FileVersionInfo *info = new FileVersionInfo();
		info->SetFilePath(GetProcessImageFullPath(hProcess));
		if (info->GetInfo())
		{
			info->GetFileDescription(description);
		}
		delete info;

		string process;

		process += procName;
		process += "|";
		process += owner;
		process += "|";
		process += pid;
		process += "|";
		process += mem;
		process += "|";
		process += description;

		if (procArray == "") {
			procArray += process;
		}
		else {
			procArray += "#";
			procArray += process;
		}
	}

	packetBuilder *bld = new packetBuilder(_processManager, processList);
	bld->WriteData((unsigned char*)procArray.c_str(), procArray.length(), _string);
	bld->FinalizePacket();
	//m_sendSerializedPacketCallback(bld->GetSerializedPacket(), bld->GetPacketSize());
	SendPacketCallback(bld);
	CloseHandle(hProcessSnap);



}

void ProcessManager::KillProcessFromPID(string pid)
{
	int ipid = atoi(pid.c_str());
	
}

void ProcessManager::SendBasicProcessInfos(int pid, vector<string> queryValues) 
{
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

	// Get process image fullepath
	string processImagePath = GetProcessImageFullPath(hProcess);
	
	

}

string ProcessManager::GetProcessImageFullPath(HANDLE hProcess)
{
	string processImagePath;
	if (hProcess != INVALID_HANDLE_VALUE) 
	{
		unique_ptr<char[]> buffer(new char[MAX_PATH]);

		DWORD dw = MAX_PATH;
		

		if (!fpQueryFullProcessImageName)
		{
			if (GetModuleFileNameEx(hProcess, nullptr, buffer.get(), MAX_PATH))
			{
				processImagePath = buffer.get();
			}
		}
		else if ((*fpQueryFullProcessImageName)(hProcess, 0, buffer.get(), &dw))
		{
			processImagePath = buffer.get();
		}
		else
		{
			processImagePath = "<error>";
		}
		buffer.reset();
	}
	
	return processImagePath;
}

string ProcessManager::GetProcessImageDescription(string imagePath)
{
	return "";
}

string ProcessManager::GetProcessOwner(HANDLE hProcess)
{
	string ProcessOwner;
	HANDLE hProcessToken = nullptr;
	// Get Process token
	if (!OpenProcessToken(hProcess, TOKEN_READ, &hProcessToken) || !hProcessToken) 
	{
		if (ProcessOwner == "")
		{
			ProcessOwner = "<undefined>";
		}
		return ProcessOwner;
	}

	OutputDebugString(GetLastErrorString().c_str());


	DWORD dwProcessTokenInfoAllocSize = 0;
	GetTokenInformation(hProcessToken, TokenUser, nullptr, 0, &dwProcessTokenInfoAllocSize);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
	{
		// Allocate pUserToken
		PTOKEN_USER pUserToken = reinterpret_cast<PTOKEN_USER>(new BYTE[dwProcessTokenInfoAllocSize]);
		if (pUserToken != nullptr) 
		{
			// Get user info
			if (GetTokenInformation(hProcessToken, TokenUser, pUserToken, dwProcessTokenInfoAllocSize, &dwProcessTokenInfoAllocSize)) 
			{
				SID_NAME_USE				snuSIDNameUser;
				unique_ptr<char[]>		UserName(new char[MAX_PATH]);
				DWORD						dwUserNameLenght = MAX_PATH;
				unique_ptr<char[]>		Domain(new char[MAX_PATH]);
				DWORD						dwDomainLenght = MAX_PATH;

				if (LookupAccountSid(nullptr, pUserToken->User.Sid, UserName.get(), &dwUserNameLenght, Domain.get(), &dwDomainLenght, &snuSIDNameUser)) 
				{
					ProcessOwner += Domain.get();
					ProcessOwner += "\\";
					ProcessOwner += UserName.get();
					CloseHandle(hProcessToken);
					
				}
				UserName.reset();
				Domain.reset();
			}
			delete[] pUserToken;
		}
	}
	else
	{
		OutputDebugString(GetLastErrorString().c_str());
	}


	if (ProcessOwner == "")
	{
		ProcessOwner = "<error>";
	}

	return ProcessOwner;
}

string ProcessManager::GetProcessMemoryUsage(HANDLE hProcess) 
{
	string memusage;
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(pmc);
	GetProcessMemoryInfo(hProcess, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	memusage = to_string(virtualMemUsedByMe);
	return memusage;
}
