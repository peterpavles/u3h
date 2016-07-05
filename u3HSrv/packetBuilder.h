#pragma once 

#include "packetDefinition.h"


class packetBuilder
{

public:
	packetBuilder(u_short cmd, u_short subCommand = 0, u_short option = 0, u_short suboption = 0);
	~packetBuilder();

	void WriteData(const char unsigned * data, int len, u_short dataType);
	void FinalizePacket();
	unsigned char * GetSerializedPacket();
	int GetPacketSize();

private:
	PACKET *m_packet;

	void SerializePacket();

	unsigned char * m_serializedBuffer;

	u_short m_command;
	u_short m_subCommand;
	u_short m_option;
	u_short m_subOption;
};
