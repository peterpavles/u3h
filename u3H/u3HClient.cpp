#include "u3h.h"
#include "u3HClient.h"


u3HClient::u3HClient(int clientID, int socketDescriptor) : m_clientID(clientID), m_socketDescriptor(socketDescriptor)
{
	m_u3hPtr = nullptr;
	connect(this, SIGNAL(HelloClient(u3HClient*, PACKET*)), u3H::Get_instance(), SLOT(OnHelloClient(u3HClient*, PACKET*)));
	connect(this, SIGNAL(LogEvent(QString)), u3H::Get_instance(), SLOT(OnU3hEventLogged(QString)));
}

u3HClient::~u3HClient()
{

}


QString u3HClient::GetClientEndPointString() const
{
	return m_clientSocket->peerAddress().toString();
}

void u3HClient::on_clientThread_Started()
{
	m_clientSocket = new QTcpSocket(this);
	if(m_clientSocket->setSocketDescriptor(m_socketDescriptor)) 
	{
		// when data ready to be read
		connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(on_clientSocket_DataReady()));
		connect(m_clientSocket, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));

		packetBuilder *b = new packetBuilder(_hello, newclient);
		b->FinalizePacket();
		SendPacket(b);
	}
	else 
	{
		emit ClientFatalError(m_clientID, tr("Can't set descriptor on socket"));
		emit TerminateClient();
	}

	LogEvent("New client from : " + GetClientEndPointString());

}

void u3HClient::on_socketDisconnected()
{
	LogEvent("Client disconnected from : " + GetClientEndPointString());
	m_clientSocket->abort();
	m_clientSocket->close();
	emit ClientLeaved(this);
}

void u3HClient::SendPacketData(char * data, int len)
{	
	if (m_clientSocket->isWritable()) 
	{
		m_clientSocket->write(data, len);	
		m_clientSocket->flush();
	}
	emit updateSendedBytesCount(len);
}

void u3HClient::SendPacket(packetBuilder *packet)
{
	SendPacketData(packet->GetSerializedPacket(), packet->GetPacketSize());
	delete packet;
}

void u3HClient::on_clientSocket_DataReady()
{
	if (!m_clientSocket->isReadable())
	{
		return;
	}

	PACKET *packet = new PACKET();
	QByteArray packetRaw = m_clientSocket->read(PACKET_SIZE_WDATA);

	if (packetRaw.size() < PACKET_SIZE_WDATA) 
	{

	}

	memcpy(packet, packetRaw.data(), PACKET_SIZE_WDATA);
	QByteArray dataRaw;
	if (packet->dwDataSize > 0) 
	{
		packet->data = static_cast<unsigned char *>(calloc(sizeof(unsigned char), packet->dwDataSize));
		
		while (dataRaw.size() < packet->dwDataSize)
		{
			dataRaw.append(m_clientSocket->readAll());
			if (dataRaw.size() < packet->dwDataSize)
				m_clientSocket->waitForReadyRead();
		}

		memcpy(packet->data, dataRaw.data(), packet->dwDataSize);
	}


	emit updateRetrieveBytesCount(packetRaw.size() + dataRaw.size());

	if (packet->_header.Command == _hello) 
	{
		emit HelloClient(this, packet);
	}
	else 
	{
		emit PacketReady(packet);
	}

}

void u3HClient::DeserializeData(QByteArray & arrayRef)
{
	char * dataPtr = arrayRef.data();
	PACKET *packet = new PACKET();
	memcpy(packet, dataPtr, PACKET_SIZE_WDATA);
	if(packet->dwDataSize > 0) 
	{
		packet->data = static_cast<unsigned char *>(calloc(sizeof(unsigned char), packet->dwDataSize));
		switch (packet->dataType) 
		{
			case _string: 
			{
				dataPtr[PACKET_SIZE_WDATA + packet->dwDataSize ]  = '\0';
				strcpy((char*)packet->data, dataPtr + PACKET_SIZE_WDATA);
				break;
			}
			case _bytes: 
			{
				memcpy(packet->data, dataPtr + PACKET_SIZE_WDATA, packet->dwDataSize);
				break;
			}
		}
	}


	if (packet->_header.Command == _hello) 
	{
		emit HelloClient(this, packet);
	}
	else 
	{
		emit PacketReady(packet);
	}

}

void u3HClient::DisconnectClient()
{
	packetBuilder *b = new packetBuilder(_clientMgmt, _clientMgmt_SUBCMD::disconnect);
	b->FinalizePacket();
	SendPacket(b);
}