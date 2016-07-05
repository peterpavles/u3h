#pragma once

#include "qt_Commons.h"
#include "u3HClient.h"
class u3H;

class connexionsListener : public QTcpServer
{

	Q_OBJECT

public:
	connexionsListener(QObject *parent = nullptr);
	~connexionsListener();
	bool StartListening(int port);
	bool StopListening();
	QMap<int, u3HClient*> * ClientMap() { return &m_clientsMap; }

private:
    QMap<int, u3HClient*> m_clientsMap;
	int m_listeningPort;

protected:
	void incomingConnection(qintptr handle) override;

public slots:
	void on_fatalClientError(int clientID, QString messageError);

signals:
	void on_stopListeningNewClient();

	void LogEvent(QString message);
	

};

