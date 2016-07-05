#pragma once

#pragma once 

//#include "Local.h"

#include "functionBase.h"

#include "string_utils.h"

class socketClient;

class ClientMgmt : public FunctionBase
{
public:
	ClientMgmt();
	~ClientMgmt();

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;


private:
};