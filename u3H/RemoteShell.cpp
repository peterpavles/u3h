#include "u3hClientForm.h"

#include "RemoteShell.h"


RemoteShell::RemoteShell(u3hClientForm * clientFormPtr) : FunctionBase(clientFormPtr)
{
}

RemoteShell::~RemoteShell()
{
}

int RemoteShell::FunctionIdentity() { return _remoteShell; }

void RemoteShell::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
		case startshell: {
			Start();
			break;
		}
		case stopshell: {
			Stop();
			break;
		}
		case restartshell: {
			Restart();
			break;
		}
		case command: {
			ProcessCommand(QString::fromLatin1((char*)packet->data));
			break;
		}
	}
}

void RemoteShell::Start() const
{	
	GetClientForm()->SetRemoteShellRunning(true);
}

void RemoteShell::Stop() const
{
	GetClientForm()->SetRemoteShellRunning(false);
	GetClientForm()->GetUI()->textEdit_remoteShell_cmdView->clear();
}

void RemoteShell::Restart() const
{
	GetClientForm()->SetRemoteShellRunning(true);
}

void RemoteShell::ProcessCommand(QString data) const
{
	GetClientForm()->GetUI()->textEdit_remoteShell_cmdView->append(data);
}
