#pragma once


#include "qt_Commons.h"
#include "win_Commons.h"

#include "packetBuilder.h"

class u3H;


class u3HClient : public QObject
{
	Q_OBJECT


public:
	u3HClient(int clientID, int socketDescriptor);
	~u3HClient();

	QString GetClientEndPointString() const;
	int GetClientUniqueID() const
	{
		return m_clientID;
	}

	
	
private:
	int m_clientID;
	int m_socketDescriptor;
	QTcpSocket *m_clientSocket;
	void DeserializeData(QByteArray & arrayRef);
	u3H *m_u3hPtr;
	void SendPacketData(char * data, int len);

signals:
	void ClientFatalError(int clientID, QString error);
	void TerminateClient();
	
	void PacketReady(PACKET *);
	void HelloClient(u3HClient * , PACKET *);
	void ClientLeaved(u3HClient*);
	void updateRetrieveBytesCount(unsigned long long size);
	void updateSendedBytesCount(unsigned long long size);


	void LogEvent(QString message);

public slots:
	void on_clientThread_Started();
	void on_clientSocket_DataReady();

	void on_socketDisconnected();
	void DisconnectClient();

	void SendPacket(packetBuilder *packet);

};

