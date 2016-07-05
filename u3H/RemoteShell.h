#pragma once

#include "FunctionBase.h"

class u3hClientForm;

class RemoteShell : public FunctionBase
{
public:
	RemoteShell(u3hClientForm * clientFormPtr);
	~RemoteShell();
	void Start() const;
	void Stop() const;
	void Restart() const;
	void ProcessCommand(QString data) const;

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;


};

