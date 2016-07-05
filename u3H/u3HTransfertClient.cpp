#include "u3h.h"
#include "u3HTransfertClient.h"

u3HTransfertClient::u3HTransfertClient(int clientID, int socketDescriptor) : m_clientID(clientID), m_socketDescriptor(socketDescriptor)
{
	m_u3hPtr = nullptr;
}

u3HTransfertClient::~u3HTransfertClient()
{

}

void u3HTransfertClient::on_clientThread_Started()
{
	m_clientSocket = new QTcpSocket(this);
	if (m_clientSocket->setSocketDescriptor(m_socketDescriptor))
	{
		//connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(on_)
	}
	else
	{
		emit TerminateClient();
	}
}

void u3HTransfertClient::on_masterStropListening()
{
	m_clientSocket->abort();
	m_clientSocket->close();
	emit TerminateClient();
}


