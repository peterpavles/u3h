#include "socketTransfetClient.h"

socketTransfertClient::socketTransfertClient() :
	m_peerIp(nullptr), m_peerPort(-1), m_isConnected(false), m_clientSocket(0)
{

}

socketTransfertClient::~socketTransfertClient(){ }

void socketTransfertClient::StartClient(char *ip, int port)
{
	m_peerIp = ip;
	m_peerPort = port;

	while (m_isConnected == false)
	{
		Sleep(1000);
		if (!Connect(ip, port))
			continue;
		SocketProc();
	}
}

bool socketTransfertClient::InitializeWinsock()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		WSACleanup();
		return false;
	}
	return true;
}

bool socketTransfertClient::Connect(char *ipaddress, int port)
{
	if (!InitializeWinsock())
		return false;


	struct sockaddr_in ipv4addr;
	ipv4addr.sin_family = AF_INET;
	ipv4addr.sin_port = htons(port);
	inet_ptonx(AF_INET, ipaddress, &ipv4addr.sin_addr);
	if (connect(m_clientSocket, reinterpret_cast<struct sockaddr*>(&ipv4addr), sizeof(ipv4addr)) != 0)
	{
		WSACleanup();
		return false;
	}
	m_isConnected = true;
	u_long blockingmode = 1;
	ioctlsocket(m_clientSocket, FIONBIO, &blockingmode);
	return true;
}


void socketTransfertClient::DisconnectSocket()
{
	closesocket(m_clientSocket);
	WSACleanup();
}


void socketTransfertClient::SocketProc()
{
	char buffer[8192];
	WSABUF wsaBuffer;
	DWORD dwFlag = 0;
	DWORD dwReceviedBytes = 0;

	while (m_isConnected)
	{
		FD_ZERO(&m_readSet);
		FD_SET(m_clientSocket, &m_readSet);

		if (select(0, &m_readSet, nullptr, nullptr, nullptr) == SOCKET_ERROR)
		{
			return;
		}

		if (FD_ISSET(m_clientSocket, &m_readSet))
		{
			FD_CLR(m_clientSocket, &m_readSet);
			wsaBuffer.buf = buffer;
			wsaBuffer.len = 8192;

			if (WSARecv(m_clientSocket, &wsaBuffer, 1, &dwReceviedBytes, &dwFlag, nullptr, nullptr) == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error == WSAEWOULDBLOCK)
				{
					continue;
				}
				else if (error == WSAECONNRESET)
				{
					m_isConnected = false;
					MasterDisconnect();
					break;
				}
			}
			else
			{
				if (dwReceviedBytes > 0)
				{
					DeserializeData(wsaBuffer.buf);
				}
				else if (dwReceviedBytes == 0)
				{
					m_isConnected = false;
					MasterDisconnect();
					break;
				}
			}

		}
	}
	free(buffer);
}


void socketTransfertClient::MasterDisconnect()
{
	WSACleanup();
	StartClient(m_peerIp, m_peerPort);
}

void socketTransfertClient::DeserializeData(char * data)
{
	PACKET * packet = new PACKET();
	// copy the packet header
	memcpy(packet, data, PACKET_SIZE_WDATA);

	// process packet data
	if (packet->dwDataSize > 0)
	{
		// allocate data 
		packet->data = static_cast<unsigned char*>(malloc(packet->dwDataSize));
		switch (packet->dataType)
		{
		case _string:
		{
			data[PACKET_SIZE_WDATA + packet->dwDataSize] = '\0';
			memcpy(packet->data, data + PACKET_SIZE_WDATA, packet->dwDataSize);
			//strcpy_s(packet->data, packet->dwDataSize  + 1, data + PACKET_SIZE_WDATA);
			break;
		}
		case _bytes:
		{
			memcpy(packet->data, data + PACKET_SIZE_WDATA, packet->dwDataSize);
			break;
		}
		}
	}

	PacketReadyCallback(packet);
}


void socketTransfertClient::SendSerializedPacket(unsigned char * data, int len) 
{
	int result;
	result = send(m_clientSocket, (char*)data, len, 0);
	if (result == SOCKET_ERROR)
	{
		// assert error
	}
}


void socketTransfertClient::SendPacket(packetBuilder *bld)
{
	SendSerializedPacket(bld->GetSerializedPacket(), bld->GetPacketSize());
	delete bld;
}

int socketTransfertClient::inet_ptonx(int af, const char *src, void *dst)
{
	struct sockaddr_storage ss;
	int size = sizeof(ss);
	char src_copy[INET6_ADDRSTRLEN + 1];

	ZeroMemory(&ss, sizeof(ss));
	/* stupid non-const API */
	strncpy(src_copy, src, INET6_ADDRSTRLEN + 1);
	src_copy[INET6_ADDRSTRLEN] = 0;

	if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0)
	{
		switch (af)
		{
		case AF_INET:
			*(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
			return 1;
		case AF_INET6:
			*(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
			return 1;
		}
	}
	return 0;
}