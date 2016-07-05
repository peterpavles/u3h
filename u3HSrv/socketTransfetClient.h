#pragma once

#include "common.h"

#include  "SingletonTemplate.h"
#include "packetDefinition.h"

#include "packetBuilder.h"


class socketTransfertClient : public Singleton<socketTransfertClient>
{
public:
	socketTransfertClient();
	~socketTransfertClient();

	void StartClient(char *ip, int port);
	function<void(PACKET*)> PacketReadyCallback;
	void SendPacket(packetBuilder * bld);
	void DisconnectSocket();

private:
	char *m_peerIp;
	int m_peerPort;

	fd_set m_readSet;
	bool m_isConnected;
	WSADATA m_wsaData;
	SOCKET m_clientSocket;

	bool Connect(char *ipaddress, int port);
	void MasterDisconnect();
	bool InitializeWinsock();
	void SocketProc();
	void DeserializeData(char * data);
	void SendSerializedPacket(unsigned char * data, int len);
	int inet_ptonx(int af, const char *src, void *dst);

};