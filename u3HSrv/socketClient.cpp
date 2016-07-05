
#include "socketClient.h"


socketClient::socketClient() : 
	m_peerIp(nullptr), m_peerPort(-1), m_isConnected(false), m_clientSocket(0)
{

}


socketClient::~socketClient()
{
}



void socketClient::StartClient(char * ip, int port)
{
	m_peerIp = ip;
	m_peerPort = port;

	while (m_isConnected == false)
	{
		Sleep(1000);

		if(!Connect(ip, port))
			continue;
		SocketProc();
	}
}

bool socketClient::InitializeWinsock()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) 
	{
		WSACleanup(); 
		return false;
	} 
	return true;
}

bool socketClient::Connect(char *ipAdress, int port)
{
	if(!InitializeWinsock())
		return false;

	m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_clientSocket == INVALID_SOCKET)
		return false;


	struct sockaddr_in ipv4addr;
	ipv4addr.sin_family = AF_INET;
	ipv4addr.sin_port = htons(port);
	inet_ptonx(AF_INET, ipAdress, &ipv4addr.sin_addr);
	if(connect(m_clientSocket, reinterpret_cast<struct sockaddr*>(&ipv4addr), sizeof(ipv4addr)) != 0) 
	{
		WSACleanup();
		return false;
	}

	m_isConnected = true;
	u_long blockingMode = 1; 
	ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);

	return true;

}

void socketClient::DisconnectSocket()
{
	closesocket(m_clientSocket);
	WSACleanup();
}

void socketClient::SocketProc()
{
	
	char buffer[8192];
	WSABUF wsaBuffer;
	DWORD dwFlag = 0;
	DWORD dwrevcBytes = 0;

	while (m_isConnected)
	{
		FD_ZERO(&m_readSet);
		FD_SET(m_clientSocket, &m_readSet);

		
		if (select(0, &m_readSet, nullptr, nullptr, nullptr) == SOCKET_ERROR) {
			return;
		}

		if(FD_ISSET(m_clientSocket, &m_readSet))
		{
			FD_CLR(m_clientSocket, &m_readSet);
			wsaBuffer.buf = buffer;
			wsaBuffer.len = 8192;

			if(WSARecv(m_clientSocket, &wsaBuffer, 1 , &dwrevcBytes, &dwFlag, nullptr, nullptr) == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if(error == WSAEWOULDBLOCK) 
				{
					continue;
				}
				// Si reset du master . 
				else if(error == WSAECONNRESET)
				{ 
					m_isConnected = false;
					MasterDisconnect();
					break;
				}
			} 
			else
			{				
				if (dwrevcBytes > 0)
				{
					DeserializeData(wsaBuffer.buf);	
				}
				// Connexion fermé .
				else if(dwrevcBytes == 0)
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

void socketClient::MasterDisconnect()
{
	WSACleanup();
	StartClient(m_peerIp, m_peerPort);	
}

/* Deserialise le char array et construit le PACKET 
   Apelle la method ProcessPacket */
void socketClient::DeserializeData(char * data) const
{
	PACKET * packet = new PACKET();

	// copy the packet header
	memcpy(packet, data, PACKET_SIZE_WDATA); 

	// process packet data
	if(packet->dwDataSize > 0) 
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

void socketClient::SendSerializedPacket(unsigned char * data, int len) const
{
	int result;
	result = send(m_clientSocket, (char*)data, len, 0);
	if(result == SOCKET_ERROR)
	{
		// assert error
	}
}


void socketClient::SendPacket(packetBuilder *bld)
{
	SendSerializedPacket(bld->GetSerializedPacket(), bld->GetPacketSize());
	delete bld;
}

int socketClient::inet_ptonx(int af, const char *src, void *dst)
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