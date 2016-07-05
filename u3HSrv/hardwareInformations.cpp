#include "socketClient.h"
#include "hardwareInformations.h"


hardwareInformations::hardwareInformations()
{
	m_sendSerializedPacketCallback = std::bind(&socketClient::SendSerializedPacket, socketClient::Get_instance(), std::placeholders::_1, std::placeholders::_2);
}


hardwareInformations::~hardwareInformations()
{
}
