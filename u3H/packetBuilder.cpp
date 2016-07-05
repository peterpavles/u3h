#include "packetBuilder.h"


packetBuilder::packetBuilder(u_short cmd, u_short subCommand, u_short option, u_short suboption )
	: m_command(cmd), m_subCommand(subCommand), m_option(option), m_subOption(suboption)
{
	m_serializedBuffer = nullptr;	
	m_packet = new PACKET();
}




void packetBuilder::WriteData(char * data, int len, u_short dataType) const
{
	m_packet->dwDataSize = len;
	
	m_packet->dataType = dataType;
	switch (dataType)
	{
		case _string:
		{
			m_packet->data = static_cast<unsigned char *>(malloc(len + 1));
			strcpy((char*)m_packet->data, data);
			m_packet->dwDataSize++;
			
			break;
		}
		case _bytes:
		{
			m_packet->data = static_cast<unsigned char *>(malloc(len));
			memcpy(m_packet->data, data, len);
			break;
		}
		default:
		break;
	}
	
}

int packetBuilder::GetPacketSize() const
{
	return PACKET_SIZE_WDATA + m_packet->dwDataSize;
}


char * packetBuilder::GetSerializedPacket() const
{
	return m_serializedBuffer;
}

void packetBuilder::SerializePacket()
{
	int size = PACKET_SIZE_WDATA + m_packet->dwDataSize;
	m_serializedBuffer = static_cast<char*>(malloc(size));
	SecureZeroMemory(m_serializedBuffer, size);

	memcpy(m_serializedBuffer, m_packet, PACKET_SIZE_WDATA);
	if(m_packet->dwDataSize > 0)
	{
		switch (m_packet->dataType)
		{
			case _string:
			{
				//strcpy(m_serializedBuffer + PACKET_SIZE_WDATA, (char*)m_packet->data);
				memcpy(m_serializedBuffer + PACKET_SIZE_WDATA, m_packet->data, m_packet->dwDataSize);
				break;
			}
			case _bytes:
			{
				memcpy(m_serializedBuffer + PACKET_SIZE_WDATA, m_packet->data, m_packet->dwDataSize);
				break;
			}
		}
	}
}

void packetBuilder::FinalizePacket()
{
	m_packet->_header.Command = m_command;
	m_packet->_header.subCommand = m_subCommand;
	m_packet->_header.Options = m_option;
	m_packet->_header.subOption = m_subOption;
	SerializePacket();
}

packetBuilder::~packetBuilder()
{
	if (m_packet->dwDataSize > 0)
	{
		free(m_packet->data);
	}
	free(m_serializedBuffer);
	delete m_packet;
}
