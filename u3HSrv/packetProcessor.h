#pragma once

//#include "Local.h"

#include "common.h"

#include "singletonTemplate.h"

#include "functionBase.h"

class clientinformations;
class RemoteShell;
class ProcessManager;
class FilesManager;
class CaptureScreen;
class ClientMgmt;


class packetProcessor : public Singleton<packetProcessor>
{
public:
	packetProcessor();
	~packetProcessor();

	void ProcessPacket(PACKET * packet);
	clientinformations *ClientInformationsClass;
	RemoteShell *RemoteShellClass;
	ProcessManager * ProcessManagerClass;
	FilesManager * FilesManagerClass;
	CaptureScreen * CaptureScreenClass;


private:
	vector<FunctionBase*> m_functionList;
	FunctionBase *GetOrInitFunction(int funcid);
		


};

