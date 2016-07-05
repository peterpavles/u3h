#include "socketClient.h"

#include "functionBase.h"

FunctionBase::FunctionBase() 
{
	//m_sendSerializedPacketCallback = bind(&socketClient::SendSerializedPacket, socketClient::Get_instance(), placeholders::_1, placeholders::_2);
	SendPacketCallback = bind(&socketClient::SendPacket, socketClient::Get_instance(), placeholders::_1);
}



FunctionBase::~FunctionBase()
{

}

void FunctionBase::freePacket(PACKET * p)
{
	if (p->dwDataSize > 0)
	{
		free(p->data);
	}
	delete p;
}

int FunctionBase::FunctionIdentity(){ return -1; }
void FunctionBase::ProcessPacket(PACKET *p){ }


