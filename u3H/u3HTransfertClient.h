#pragma once

#include "qt_Commons.h"
#include "win_Commons.h"

class u3H;



class u3HTransfertClient : public QObject
{
	Q_OBJECT;
public:
	u3HTransfertClient(int clientID, int socketDescriptor);
	~u3HTransfertClient();

private:
	int m_clientID;
	int m_socketDescriptor;
	QTcpSocket *m_clientSocket;
	u3H *m_u3hPtr;

signals:
	void TerminateClient();

public slots:
	void on_clientThread_Started();
	void on_masterStropListening();

};