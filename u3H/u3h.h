#pragma once

#include "qt_Commons.h"

#include "ui_u3h.h"
#include "singletonTemplate.h"
#include "connexionsListener.h"
#include "u3hClientForm.h"
#include "configHandler.h"
#include "u3HClient.h"
#include "packetsDefinition.h"
#include "utils.h"




class u3H : public QMainWindow, public Singleton<u3H>
{ 
	Q_OBJECT

public:
	friend class Singleton<u3H>;	
	u3hClientForm * GetClientUi(u3HClient * client) const;


	Ui::u3HClass *Gui();

	QList<u3hClientForm*> RegisteredClientUiList() const
	{ return m_registeredClientUiList; }
	void DeleteAndunregisterAll();


private:
	u3H(QWidget *parent = nullptr);
	~u3H();

	Ui::u3HClass ui;
	connexionsListener *m_coListener;
	configHandler *m_configHandler;
	void BuildGui();
	QList<u3hClientForm*> m_registeredClientUiList;

	QMenu *m_treeClients_customContextMenu;

	unsigned long long m_totalBytesRecevied;
	unsigned long long m_totalBytesSended;

signals:
	void LogU3hEvent(QString message);


public slots:
	void on_setThemGrpActionTriggered(QAction*action) const;

    void treeWidget_clients_addItem(QTreeWidgetItem * item) const;
	
	// QMetaObject::connectSlotsByName
	void on_treeWidget_settings_itemClicked(QTreeWidgetItem * item, int column) const;
	void on_pushButton_start_listeners_clicked();
	void on_pushButton_stop_listeners_clicked();
	void on_treeWidget_clients_itemDoubleClicked(QTreeWidgetItem * item, int column);
	void on_treeWidget_clients_customContextMenuRequested(const QPoint & poin);

	void RegisterClientUI(u3hClientForm* client);
	void UnregisterClientUI(u3hClientForm * client);
	void ClientLeaved(u3HClient *client);

	void OnHelloClient(u3HClient * client, PACKET * packet) const;

	// bytes rx / tx stats
	void updateBytesRecevied(unsigned long long size);
	void updateBytesSended(unsigned long long size);

	void WriteEventLabel(QString data);

	void OnU3hEventLogged(QString message);

	// contextmenu treewidget clients
	void OnDisconnectClientActionTriggered();
	void OnU3hPanelClientActionTriggered();


};


