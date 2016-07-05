#include "socketClient.h"
#include "ClientMgmt.h"

ClientMgmt::ClientMgmt()
{

}

ClientMgmt::~ClientMgmt()
{

}

int ClientMgmt::FunctionIdentity(){ return _clientMgmt; }

void ClientMgmt::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
		case _clientMgmt_SUBCMD::disconnect:
		{
			socketClient * c = socketClient::Get_instance();
			c->DisconnectSocket();

			break;
		}
	}


	freePacket(packet);

}