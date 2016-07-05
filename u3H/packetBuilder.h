#pragma once

#include "packetsDefinition.h"



class packetBuilder
{


public:
	explicit packetBuilder(u_short cmd, u_short subCommand = 0, u_short option = 0, u_short suboption = 0);
	
	~packetBuilder();

	void WriteData(char * data, int len, u_short dataType) const;
	void FinalizePacket();
	char * GetSerializedPacket() const;
	int GetPacketSize() const;
	
private:
	PACKET *m_packet;

	void SerializePacket();

	char * m_serializedBuffer;

	u_short m_command;
	u_short m_subCommand;
	u_short m_option;
	u_short m_subOption;
};
