#include "u3h.h"
#include "transfertListener.h"



transfertListener::transfertListener(QObject *parent) : QTcpServer(parent) 
{
	m_listeningPort = -1;
	connect(this, SIGNAL(LogEvent(QString)), parent, SLOT(OnU3hEventLogged(QString)));
}
transfertListener::~transfertListener() { }

bool transfertListener::StartListening(int port)
{
	if (!listen(QHostAddress::Any, 5060))
	{
		return false;
	}
	m_listeningPort = port;
	LogEvent("Listening for transfert on port : " + QString::number(port));
	return true;
}

bool transfertListener::StopListening()
{
	if (!isListening())
	{
		return false;
	}

	close();

	emit on_stopListeningNewClient();


	m_clientMap.clear();

	LogEvent("Stop listening for transfert on port : " + QString::number(m_listeningPort));

	return isListening() ? false : true;

}

void transfertListener::incomingConnection(qintptr handle)
{
	QThread *transfertThread = new QThread(this);
	u3HTransfertClient *newClient = new u3HTransfertClient(qrand(), handle);
}


void transfertListener::on_fatalClientError(int clientID, QString messageError)
{
	QMessageBox::critical(nullptr, tr("Client Fatal Error"), messageError, QMessageBox::Ok);
	if (m_clientMap.contains(clientID)) 
	{
		m_clientMap.remove(clientID);
	}
}