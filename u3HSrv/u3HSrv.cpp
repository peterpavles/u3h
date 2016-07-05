#include "socketClient.h"
#include "packetProcessor.h"


#include "u3HSrv.h"





u3HSrv::u3HSrv() 
{
	m_socketCLient = socketClient::Init_instance();	
	m_packetProcessor = packetProcessor::Init_instance();



	m_socketCLient->PacketReadyCallback = bind(&packetProcessor::ProcessPacket, m_packetProcessor, placeholders::_1);
	m_socketCLient->StartClient(CONNEXION_IP, CONNEXION_CO_PORT);
}


u3HSrv::~u3HSrv()
{
	packetProcessor::destroy_instance();
	socketClient::destroy_instance();
}



