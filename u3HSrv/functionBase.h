#pragma once

#include "common.h"
#include "packetBuilder.h"

class socketClient;

class FunctionBase
{
public:
	FunctionBase();
	~FunctionBase();

	//function<void(unsigned char*, int)> m_sendSerializedPacketCallback;
	function<void(packetBuilder *)> SendPacketCallback;

	void freePacket(PACKET * p);

	virtual int FunctionIdentity();
	virtual void ProcessPacket(PACKET*);

};