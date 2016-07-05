#include "u3h.h"
#include "connexionsListener.h"



connexionsListener::connexionsListener(QObject *parent) : QTcpServer(parent) 
{ 
	m_listeningPort = -1;
	connect(this, SIGNAL(LogEvent(QString)), parent, SLOT(OnU3hEventLogged(QString)));
}
connexionsListener::~connexionsListener() { }

bool connexionsListener::StartListening(int port)
{
	if(!listen(QHostAddress::Any, 5050)) 
	{	
		return false;
	}
	m_listeningPort = port;
	LogEvent("Listening for connection on port : " + QString::number(port));
	return true;
}

bool connexionsListener::StopListening()
{
	if (!isListening())
	{
		return false;
	}

	// Arrete l'ecoute 
	close();

	// Apelle le slots OnMasterStopped de chaque u3h client, deconnect le socket, et suprime l'object . 
	emit on_stopListeningNewClient();
	
	// On vide le qmap .
	m_clientsMap.clear();

	LogEvent("Stop listening for connection on port : " + QString::number(m_listeningPort));

	// Renvoie true si le serveur n'ecoute plus.
	return isListening() ? false : true;
}

// Override de la fonction declancher lors de la reception d'un nouveau client. 
void connexionsListener::incomingConnection(qintptr handle)
{
	QThread *clientThread = new QThread(this);
	u3HClient *newClient = new u3HClient(qrand(), handle);

	// add client to the list
	m_clientsMap.insert(newClient->GetClientUniqueID(), newClient);

	// Deplace l'intance client dans le thread et connect le slot started . 
	newClient->moveToThread(clientThread);

	// Emeteur Thread
	connect(clientThread, SIGNAL(started()), newClient, SLOT(on_clientThread_Started())); /* Debute linitialisation du socket dans le thread */ 
	connect(clientThread, SIGNAL(finished()), clientThread, SLOT(deleteLater()));	/* // */

	// Emetteur this . Lors de l'arret du listening on deco tout les client + autodelete . 
	connect(this, SIGNAL(on_stopListeningNewClient()), newClient, SLOT(on_socketDisconnected()));

	// Emeteur Client 
	connect(newClient, SIGNAL(TerminateClient()), clientThread, SLOT(quit())); /* Clos le thread */
	connect(newClient, SIGNAL(TerminateClient()), newClient, SLOT(deleteLater())); /* Sassure que le thread est detruit uniquement apres sont arret */ 
	connect(newClient, SIGNAL(ClientFatalError(int,QString)), this, SLOT(on_fatalClientError(int,QString))); /* En cas d'erreur Fatal du client */
	connect(newClient, SIGNAL(ClientLeaved(u3HClient*)), parent(), SLOT(ClientLeaved(u3HClient *)));

	// byte tx / rx stats
	connect(newClient, SIGNAL(updateSendedBytesCount(unsigned long long)), parent(), SLOT(updateBytesSended(unsigned long long)));
	connect(newClient, SIGNAL(updateRetrieveBytesCount(unsigned long long)), parent(), SLOT(updateBytesRecevied(unsigned long long)));

	clientThread->start();


	

}
 

/* Slot appeler lors d'une erreur fatal du client (e.g n'a pas pus assigné le descripteur au socket) */
void connexionsListener::on_fatalClientError(int clientID, QString messageError)
{
	QMessageBox::critical(nullptr, tr("Client Fatal Error"), messageError, QMessageBox::Ok);
	if(m_clientsMap.contains(clientID)) 
	{
		m_clientsMap.remove(clientID);
	}
}

