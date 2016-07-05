#pragma once

#pragma once

#include "qt_Commons.h"
#include "u3HTransfertClient.h"
class u3H;

class transfertListener : public QTcpServer
{

	Q_OBJECT

public:
	transfertListener(QObject *parent = nullptr);
	~transfertListener();

	bool StartListening(int port);
	bool StopListening();
	QMap<int, u3HTransfertClient*> *ClientMap() { return &m_clientMap; };

private:
	QMap<int, u3HTransfertClient*> m_clientMap;
	int m_listeningPort;
protected:
	void incomingConnection(qintptr handle) override;

public slots:
	void on_fatalClientError(int clientID, QString messageError);
signals :
	void on_stopListeningNewClient();

	void LogEvent(QString message);

};