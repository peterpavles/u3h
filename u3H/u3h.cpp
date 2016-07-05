
#include "u3h.h"

u3H::u3H(QWidget *parent) : QMainWindow(parent)
{


	connect(this, SIGNAL(LogU3hEvent(QString)), this, SLOT(OnU3hEventLogged(QString)));

	setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this); 
	m_coListener = nullptr;
	show();

	// Step 0 : Build GUI 
	BuildGui();

	// Step 1 : Load Config 
	m_configHandler = configHandler::Init_instance(this);
	m_configHandler->LoadConfig();

	m_totalBytesRecevied = 0;
	m_totalBytesSended = 0;

}

u3H::~u3H() 
{
	configHandler::destroy_instance();
	delete m_treeClients_customContextMenu;
}





void u3H::BuildGui()
{
	
	// Setting page
	QStringList styles = QStyleFactory::keys();
	QActionGroup *grpActionTheme = new QActionGroup(ui.menuTheme);
	for (int i = 0; i < styles.size(); i++)
	{
		QAction * kk = ui.menuTheme->addAction(styles.at(i));
		kk->setData(styles.at(i));
		grpActionTheme->addAction(kk);
	}

	// them selector action menu
	connect(grpActionTheme, SIGNAL(triggered(QAction*)), this, SLOT(on_setThemGrpActionTriggered(QAction*)));

	m_treeClients_customContextMenu = new QMenu("Client Menu");
	QActionGroup * clientTree_customMenu_act_grp = new QActionGroup(m_treeClients_customContextMenu);
	
	QAction * clientSettings_act = m_treeClients_customContextMenu->addAction("Client Settings");
	clientTree_customMenu_act_grp->addAction(clientSettings_act);
	QAction * clientPanel_act = m_treeClients_customContextMenu->addAction("u3h Panel");
	clientTree_customMenu_act_grp->addAction(clientPanel_act);
	connect(clientPanel_act, SIGNAL(triggered()), this, SLOT(OnU3hPanelClientActionTriggered()));
	QAction * clientDisconnect = m_treeClients_customContextMenu->addAction("Kill");
	clientTree_customMenu_act_grp->addAction(clientDisconnect);
	connect(clientDisconnect, SIGNAL(triggered()), this, SLOT(OnDisconnectClientActionTriggered()));

	//connect(clientTree_customMenu_act_grp, SIGNAL(triggered(QAction*)), this, SLOT(on_setProcessManagerRefreshInterval(QAction*)));

	LogU3hEvent("u3h - Remote access alpha\n");
}

u3hClientForm * u3H::GetClientUi(u3HClient * client) const
{
	for(int i = 0; i < RegisteredClientUiList().count(); i++) 
	{
		u3HClient *clPtr = RegisteredClientUiList().at(i)->U3hClient();
		if(clPtr == client) 
		{
			return RegisteredClientUiList().at(i);
		}
	}
	return nullptr;
}

Ui::u3HClass *u3H::Gui()
{
	return &ui;
}

void u3H::on_setThemGrpActionTriggered(QAction*action) const
{
	QString style = action->data().toString();
	QApplication::setStyle(style);
}

void u3H::on_treeWidget_clients_customContextMenuRequested(const QPoint & poin)
{
	/*int itemCount = ui.treeWidget_clients->selectedItems().count();
	if (itemCount <= 0)
	{
		return;
	}

	QTreeWidgetItem *selected = ui.treeWidget_clients->selectedItems()[0];
	m_treeClients_customContextMenu->exec(QCursor::pos());*/


	QTreeWidgetItem * selected = ui.treeWidget_clients->itemAt(poin);
	if (selected != nullptr)
	{
		m_treeClients_customContextMenu->exec(QCursor::pos());
	}

}

void u3H::OnHelloClient(u3HClient * client, PACKET * packet) const
{
	QString data((char*)packet->data);
	QStringList dataList = data.split("|#|");
	dataList.insert(1, client->GetClientEndPointString());


	QTreeWidgetItem *item = new QTreeWidgetItem();

	for (int i = 0; i < dataList.count(); i++) 
	{
		item->setText(i, dataList.at(i));
	}



	//item->setData(0, 0x1001, client->GetClientUniqueID());
	item->setData(0, 0x1001, VPtr<u3HClient>::asQVariant(client));


	item->setText(6, QString::number(client->GetClientUniqueID()));
	QTime time = QTime::currentTime();
	qsrand(static_cast<uint>(time.msec()) + reinterpret_cast<uint>(client));
	QString randomHex = "0x";

	for (int i = 0; i < 8; i++) 
	{
		int n = qrand() % 16;
		randomHex.append(QString::number(n, 16).toUpper());
	}

	//item->setText(0, randomHex);
	ui.treeWidget_clients->addTopLevelItem(item);
	

	// don't forget to free packet (on the right place .. )
	free(packet->data);
	delete packet;
}

void u3H::on_treeWidget_settings_itemClicked(QTreeWidgetItem * item, int column) const
{

}

void u3H::on_pushButton_start_listeners_clicked()
{
	// Test si les entrées sont bien au format numeric 
 	bool isInt = false;
 	if(ui.lineEdit_connexionPort->text() == "" || (ui.lineEdit_connexionPort->text().toInt(&isInt), isInt == false) &&
		ui.lineEdit_transfertsPort->text() == "" || ui.lineEdit_transfertsPort->text().toInt(&isInt), isInt == false) {
 		QMessageBox::warning(nullptr, "Error", "Port number not provided or not in numeric format.", QMessageBox::StandardButton::Ok, NULL);
 		return;
 	}

	if(m_coListener != nullptr) {
		QMessageBox::warning(nullptr, "warning", "Listeners already listening", QMessageBox::Ok);
		return;
	}

	m_coListener = new connexionsListener(this);
 	if(!m_coListener->StartListening(ui.lineEdit_connexionPort->text().toInt()))
 	{
 		QMessageBox::warning(nullptr, "Error", "Unable to listen on port : " + ui.lineEdit_connexionPort->text(), QMessageBox::StandardButton::Ok, NULL);
 		OutputDebugString(m_coListener->errorString().toStdWString().c_str());
    }
	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setText(0, "Connexion listener");
	item->setText(1, ui.lineEdit_connexionPort->text());
	item->setText(2, "Listening");
	item->setIcon(0, QIcon(":/u3H/u3hrsc/bullet_blue.png"));
	ui.treeWidget_listeners->addTopLevelItem(item);
}

void u3H::on_pushButton_stop_listeners_clicked()
{
	if(!m_coListener->isListening()) {
		return;
	}

	// On ferme les ui client avant tout . 
	DeleteAndunregisterAll();
	// On suprime tout les qtreewidgetitem de la vue client . 
	for(int i = 0; i < ui.treeWidget_clients->topLevelItemCount(); i++) {
		QTreeWidgetItem * itm = ui.treeWidget_clients->topLevelItem(i);
		delete itm;
	}

	if(!m_coListener->StopListening()) {
		QMessageBox m;
		m.setText(tr("Unable to stop listeners"));
		m.setWindowTitle(tr("error"));
		m.show();
		return;
	}

	// On suprime le listener 
	delete m_coListener;
	m_coListener = nullptr;
	ui.treeWidget_listeners->clear();

}

void u3H::on_treeWidget_clients_itemDoubleClicked(QTreeWidgetItem * item, int column)
{
	int id = item->text(6).toInt();
	u3HClient * client = m_coListener->ClientMap()->value(id);
	u3hClientForm * frm = new u3hClientForm(client);

	RegisterClientUI(frm);	
	frm->setAttribute(Qt::WA_DeleteOnClose);  
	
	connect(frm, SIGNAL(WindowsClosed(u3hClientForm *)), this, SLOT(UnregisterClientUI(u3hClientForm *)), Qt::AutoConnection);

	QString title;
	title.append(item->text(1));
	title.append(" / [");
	title.append(item->text(3));
	title.append("@");
	title.append(item->text(2));
	title.append("]");
	frm->setWindowTitle(title);
	frm->show();	
}

void u3H::treeWidget_clients_addItem(QTreeWidgetItem * item) const
{
	ui.treeWidget_clients->addTopLevelItem(item);
}

void u3H::RegisterClientUI(u3hClientForm * client)
{
	if (m_registeredClientUiList.contains(client)) {
		return;
	}
	m_registeredClientUiList.append(client);
}

void u3H::UnregisterClientUI(u3hClientForm * client)
{
	if (m_registeredClientUiList.contains(client)) {
		m_registeredClientUiList.removeOne(client);
	}
}

void u3H::ClientLeaved(u3HClient *client)
{
	// delete and clear any register ui and close it 
	for (int i = 0; i < m_registeredClientUiList.count(); i++) 
	{
		u3hClientForm * frm = m_registeredClientUiList.at(i);
		if (frm->U3hClient()->GetClientUniqueID() == client->GetClientUniqueID())
		{
			UnregisterClientUI(frm);
			// todo: prompt user
			frm->close();
		}
	}


	for (int i = 0; i < ui.treeWidget_clients->topLevelItemCount(); i++)
	{
		QTreeWidgetItem *current = ui.treeWidget_clients->topLevelItem(i);
		if (!current->data(0, 0x1001).isNull())
		{
			if (VPtr<u3HClient>::asPtr(current->data(0, 0x1001)) == client)
			{
				current = ui.treeWidget_clients->takeTopLevelItem(i);
				delete current;
			}
		}
	}

	emit client->TerminateClient();
}

void u3H::DeleteAndunregisterAll()
{
	for (int i = 0; i < m_registeredClientUiList.count(); i++) {
		u3hClientForm * frm = m_registeredClientUiList.at(i);
		UnregisterClientUI(frm);
		frm->close();
	}
}

void u3H::updateBytesRecevied(unsigned long long size)
{
	m_totalBytesRecevied = m_totalBytesRecevied + size;
	ui.label_dashboard_downloaded->setText(utils::stringBytetoHumanReadable(QString::number(m_totalBytesRecevied)));
}

void u3H::updateBytesSended(unsigned long long size)
{
	m_totalBytesSended = m_totalBytesSended + size;
	ui.label_dashboard_uploaded->setText(utils::stringBytetoHumanReadable(QString::number(m_totalBytesSended)));
}

void u3H::WriteEventLabel(QString data)
{
	
}

void u3H::OnU3hEventLogged(QString message)
{
	ui.textEdit_eventsLogs->append(QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate)  + "	" + message);
}

void u3H::OnDisconnectClientActionTriggered()
{
	if (ui.treeWidget_clients->selectedItems().count() > 0)
	{
		QTreeWidgetItem * o = ui.treeWidget_clients->selectedItems()[0];
		int id = o->text(6).toInt();
		u3HClient * client = m_coListener->ClientMap()->value(id);
		QMetaObject::invokeMethod(client, "DisconnectClient", Qt::QueuedConnection);
	}

}

void u3H::OnU3hPanelClientActionTriggered()
{

}