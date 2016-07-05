#include "ClientInformations.h"
#include "RemoteShell.h"
#include "ProcessManager.h"
#include "FilesManager.h"
#include "CaptureScreen.h"

#include "u3hClientForm.h"


u3hClientForm::u3hClientForm(u3HClient * client, QWidget * parent) 
	: QMainWindow(parent), m_u3hClient(client)
{
	setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this);
	buildCustomUi();
	buildSignalSlots();

	m_isRemoteShellRunning = false;	
}

u3hClientForm::~u3hClientForm()
{
	delete RemoteShellMenuBar;
	delete ProcessManagerMenuBar;
	delete FilesManagerMenuBar;
	delete CaptureScreenMenuBar;
	delete ProcessMgrContextMenu;
	delete FilesMgrContextMenu;

	QList<FunctionBase*>::iterator i;
	int pos = 0;
	for (i = m_functionList.begin(); i != m_functionList.end(); ++i)
	{
		delete *i;
	}

}

void u3hClientForm::buildCustomUi()
{

	// Splitter FilesManager
	ui.splitter_3->setStretchFactor(0, 0);
	ui.splitter_3->setStretchFactor(1, 20);


	// Splitter ProcessManager
	ui.splitter_2->setStretchFactor(0, 20);
	ui.splitter_2->setStretchFactor(1, 0);

	// Remote Shell Menubar 
	RemoteShellMenuBar			= new QMenuBar();
	QMenu *menu					= new QMenu("Menu");
	QAction *startShell			= menu->addAction("Start");
	QAction *stopShell			= menu->addAction("Stop");
	QAction *restartShell		= menu->addAction("Restart");
	RemoteShellMenuBar->addMenu(menu);

	connect(startShell, SIGNAL(triggered()), this, SLOT(StartRemoteShell()));
	connect(stopShell, SIGNAL(triggered()), this, SLOT(StopRemoteShell()));
	connect(restartShell, SIGNAL(triggered()), this, SLOT(RestartRemoteShell()));

	ui.page_remoteShell->layout()->setMenuBar(RemoteShellMenuBar);

	//
	// Process Manager 
	//
	// allow custom context menu for qtreewidget
	ui.treeWidget_process->setContextMenuPolicy(Qt::CustomContextMenu);

	// Create and connect Process Manager Refresh Timer
	m_processMgrRefreshTimer = new QTimer(this);
	connect(m_processMgrRefreshTimer, SIGNAL(timeout()), this, SLOT(on_ProcMgrRefreshEvents()));

	// Set the Process Manager Qtreewidget column width
	ui.treeWidget_process->setColumnWidth(0, 300);

	// Create the Process Manager Panel Menu Bar
	ProcessManagerMenuBar					= new QMenuBar();
	QMenu *procmanmenu						= new QMenu("Menu");
	QAction *procmgr_act_getlist			= procmanmenu->addAction("Get Process List");
	QMenu	*procmanmenu_refresh			= procmanmenu->addMenu("Refresh");

	QActionGroup *procmgr_grpAction_refresh = new QActionGroup(procmanmenu_refresh);
	QAction *procmgr_act_refresh10sec		= procmanmenu_refresh->addAction("10 Sec");	
	QAction *procmgr_act_refresh5sec		= procmanmenu_refresh->addAction("5 Sec");
	QAction *procmgr_act_refresh2sec		= procmanmenu_refresh->addAction("2 Sec");
	QAction *procmgr_act_stoprefresh		= procmanmenu_refresh->addAction("Stop");
	procmgr_grpAction_refresh->addAction(procmgr_act_refresh10sec);
	procmgr_grpAction_refresh->addAction(procmgr_act_refresh5sec);
	procmgr_grpAction_refresh->addAction(procmgr_act_refresh2sec);
	procmgr_grpAction_refresh->addAction(procmgr_act_stoprefresh);
	procmgr_act_refresh10sec->setData(10000);
	procmgr_act_refresh5sec->setData(5000);
	procmgr_act_refresh2sec->setData(2000);
	procmgr_act_stoprefresh->setData(0);




	connect(procmgr_act_getlist, SIGNAL(triggered()), this, SLOT(on_ProcMgrRefreshEvents()));
	connect(procmgr_grpAction_refresh, SIGNAL(triggered(QAction*)), this, SLOT(on_setProcessManagerRefreshInterval(QAction*)));

	ProcessManagerMenuBar->addMenu(procmanmenu);
	ui.page_processManager->layout()->setMenuBar(ProcessManagerMenuBar);

	// Process Manager ContextmenuBar
	ProcessMgrContextMenu					= new QMenu("");
	QAction *proc_mgr_refreshList			= ProcessMgrContextMenu->addAction("Refresh List");

	QAction *procmgr_act_killProc			= ProcessMgrContextMenu->addAction("Kill");
	QAction *procmgr_act_newProc			= ProcessMgrContextMenu->addAction("New Process");
	QAction *procmgr_act_openInBrowser		= ProcessMgrContextMenu->addAction("Open in files Browser");
	connect(procmgr_act_killProc, SIGNAL(triggered()), this, SLOT(on_ProcMgrActionKillProcessTriggered()));
	connect(procmgr_act_newProc, SIGNAL(triggered()), this, SLOT(on_ProcMgrActionNewProcessTriggered()));
	connect(procmgr_act_openInBrowser, SIGNAL(triggered()), this, SLOT(on_ProcMgrActionOpenPathInBrowser()));

	// Files Manager Menu Bar
	FilesManagerMenuBar						= new QMenuBar();
	QMenu *filesMgrMenu						= new QMenu("Menu");
	QAction *filesMgr_act_getDrives			= filesMgrMenu->addAction("Get drives");
	FilesManagerMenuBar->addMenu(filesMgrMenu);
	ui.page_filesManager->layout()->setMenuBar(FilesManagerMenuBar);

	connect(filesMgr_act_getDrives, SIGNAL(triggered()), this, SLOT(FilesManagerGetDrivesList()));

	FilesMgrContextMenu						= new QMenu("File");
	QAction * filesMgr_act_file_delete		= FilesMgrContextMenu->addAction("Delete");
	connect(filesMgr_act_file_delete, SIGNAL(triggered()), this, SLOT(FilesManagerDeleteFile()));
	QAction * filesMgr_act_file_rename		= FilesMgrContextMenu->addAction("Rename");
	connect(filesMgr_act_file_rename, SIGNAL(triggered()), this, SLOT(FilesManagerRenameFile()));
	QAction * filesMgr_act_file_download	= FilesMgrContextMenu->addAction("Download");
	connect(filesMgr_act_file_download, SIGNAL(triggered()), this, SLOT(FilesManagerDownloadFile()));



	// Capture screen
	// - menu bar
	CaptureScreenMenuBar					= new QMenuBar();
	QMenu *captureScreenMenu				= new QMenu("Menu");

	QAction *captureScreen_act_getframe		= captureScreenMenu->addAction("Get Frame");


	CaptureScreenMenuBar->addMenu(captureScreenMenu);
	ui.page_capture_screen->layout()->setMenuBar(CaptureScreenMenuBar);

	connect(captureScreen_act_getframe, SIGNAL(triggered()), this, SLOT(on_captureScreen_act_getFrame()));
}

void u3hClientForm::buildSignalSlots() const
{
	// send back packet to be sent to u3hclient in a safe way
	connect(this, SIGNAL(SendPacket(packetBuilder*)), m_u3hClient, SLOT(SendPacket(packetBuilder*)));



	// connect packer dispatcher
	connect(m_u3hClient, SIGNAL(PacketReady(PACKET *)), this, SLOT(OnPacketReady(PACKET *)));
}

void u3hClientForm::on_treeWidget_selectPages_itemClicked(QTreeWidgetItem * item, int column) const
{
	if (item->text(0) == "# Base") 
	{
		ui.stackedWidget->setCurrentIndex(0);
	}
	if(item->text(0) == "Remote Shell") 
	{ 
		ui.stackedWidget->setCurrentIndex(1);
	}
	if (item->text(0) == "Client") 
	{
		ui.stackedWidget->setCurrentIndex(2);
	}
	if(item->text(0) == "Process") 
	{
		ui.stackedWidget->setCurrentIndex(3);
	}
	if (item->text(0) == "Filesystem") 
	{
		ui.stackedWidget->setCurrentIndex(4);
	}
	if (item->text(0) == "Screen") 
	{
		ui.stackedWidget->setCurrentIndex(5);
	}
}

FunctionBase *u3hClientForm::GetOrInitFunction(int funcIdent)
{
	FunctionBase *myfunc = nullptr;
	QList<FunctionBase*>::iterator i;
	int pos = 0;
	for (i = m_functionList.begin(); i != m_functionList.end(); ++i)  
	{
		if (funcIdent == static_cast<FunctionBase*>(*i)->FunctionIdentity()) 
		{
			return *i;
		}
		pos++;
	}

	switch (funcIdent)
	{
	case _clientInformation: 
	{
		myfunc = new ClientInformations(this);
		break;
	}
	case _remoteShell: 
	{
		myfunc = new RemoteShell(this);
		break;
	}
	case _processManager: 
	{
		myfunc = new ProcessManager(this);
		break;
	}
	case _filesManager: 
	{
		myfunc = new FilesManager(this);
		break;
	}
	case _captureScreen: 
	{
		myfunc = new CaptureScreen(this);
	}
	default:
		// unknown class
		break;
	}

	m_functionList.push_back(myfunc);
	return myfunc;

}

void u3hClientForm::OnPacketReady(PACKET *packet)
{
	FunctionBase *base = GetOrInitFunction(packet->_header.Command);
	base->ProcessPacket(packet);
}

#pragma region RemoteShell

void u3hClientForm::SetRemoteShellRunning(bool isRun)
{
	m_isRemoteShellRunning = isRun;
}

void u3hClientForm::StartRemoteShell()
{
	packetBuilder * packet = new packetBuilder(_remoteShell, startshell);
	packet->FinalizePacket();
	emit SendPacket(packet);

}

void u3hClientForm::StopRemoteShell()
{
	if (!m_isRemoteShellRunning) return;


	packetBuilder * packet = new packetBuilder(_remoteShell, stopshell);
	packet->FinalizePacket();
	emit SendPacket(packet);

}

void u3hClientForm::RestartRemoteShell()
{
	if (!m_isRemoteShellRunning) return;


	packetBuilder * packet = new packetBuilder(_remoteShell, restartshell);
	packet->FinalizePacket();
	emit SendPacket(packet);

}

void u3hClientForm::on_pushButton_remoteShell_sendCommand_clicked()
{
	if (!m_isRemoteShellRunning) 
	{
		QMessageBox m;
		m.setWindowTitle(tr("Remote Shell"));
		m.setText(tr("Remote Shell is not running"));
		m.show();
		return;
	}

	if (ui.lineEdit_remoteShell_sendCmd->text() == "") 
	{
		return;
	}

	QByteArray ba = ui.lineEdit_remoteShell_sendCmd->text().toLatin1();
	char *c_str2 = ba.data();

	packetBuilder * packet = new packetBuilder(_remoteShell, command);
	packet->WriteData(c_str2, strlen(c_str2), _string);
	packet->FinalizePacket();
	emit SendPacket(packet);
	ui.lineEdit_remoteShell_sendCmd->setText("");
}


#pragma endregion

#pragma region ProcessManager

void u3hClientForm::on_ProcMgrRefreshEvents()
{
	packetBuilder * packet = new packetBuilder(_processManager, processList);
	packet->FinalizePacket();
	emit SendPacket(packet);
}

void u3hClientForm::treeWidget_process_addItemArray(QList<uQTreeWidgetItem*> * processArray) const
{
	ui.treeWidget_process->clear();
	ui.treeWidget_process->addTopLevelItems(reinterpret_cast<QList<QTreeWidgetItem*>&>(*processArray));

}

void u3hClientForm::on_treeWidget_process_customContextMenuRequested(const QPoint & point) const
{
	ProcessMgrContextMenu->exec(QCursor::pos());
}

void u3hClientForm::on_ProcMgrQtreeViewItemClicked(QTreeWidgetItem * item, int column) { }

void u3hClientForm::ProcessManagerGetProcessBasicInfos(QString pid) { }

void u3hClientForm::on_setProcessManagerRefreshInterval(QAction* interval) const
{
	int refresh = interval->data().toInt();
	if (refresh == 0) 
	{
		m_processMgrRefreshTimer->stop();
	}
	else 
	{
		m_processMgrRefreshTimer->setInterval(refresh);
		if (!m_processMgrRefreshTimer->isActive())
			m_processMgrRefreshTimer->start(refresh);
	}

}

void u3hClientForm::on_ProcMgrActionKillProcessTriggered()
{

	if (ui.treeWidget_process->selectedItems().count() > 0)
	{
		std::string pid = ui.treeWidget_process->selectedItems()[0]->text(1).toStdString();
		packetBuilder * packet = new packetBuilder(_processManager, killprocess);
		packet->WriteData((char*)pid.c_str(), pid.length(), _string);
		packet->FinalizePacket();
		emit SendPacket(packet);
	}
}

void u3hClientForm::on_ProcMgrActionNewProcessTriggered()
{

}

void u3hClientForm::on_ProcMgrActionOpenPathInBrowser()
{

}


#pragma endregion

#pragma region FileBrowser

void u3hClientForm::on_treeWidget_filesManager_dirs_itemClicked(QTreeWidgetItem * item, int column)
{
	QString path = item->text(0);
	
	// current item
	QTreeWidgetItem * current = item;
	if (current->parent() != nullptr) 
	{
		// while curent is not null
		while (current != nullptr) 
		{
			// tmp is the parrent of current
			QTreeWidgetItem *tmp = current->parent();
			// if no parent, we are at the root object, wich assuming ending with \ 
			if (tmp != nullptr)
			{
				path = tmp->text(0) + "\\" + path;
			}
			current = tmp;
		}
	}


	packetBuilder * packet = new packetBuilder(_filesManager, browsePath);
	packet->WriteData(const_cast<char*>(path.toStdString().c_str()), strlen(path.toStdString().c_str()), _string);
	packet->FinalizePacket();
	emit SendPacket(packet);

}

void u3hClientForm::FilesManagerGetDrivesList()
{
	packetBuilder * packet = new packetBuilder(_filesManager, getdrives);
	packet->FinalizePacket();
	emit SendPacket(packet);
}

void u3hClientForm::treeWidget_filesMgr_folders_addItemArray(QList<uQTreeWidgetItem*>* itemArray) const
{
	ui.treeWidget_filesManager_dirs->clear();

	ui.treeWidget_filesManager_dirs->addTopLevelItems(reinterpret_cast<QList<QTreeWidgetItem*>&>(*itemArray));
	ui.treeWidget_filesManager_dirs->expandToDepth(0);
}

void u3hClientForm::treeWidget_filesMgr_files_addItemArray(QList<uQTreeWidgetItem*>* itemArray) const
{
	ui.treeWidget_filesManager_files->clear();
	ui.treeWidget_filesManager_files->addTopLevelItems(reinterpret_cast<QList<QTreeWidgetItem*>&>(*itemArray));
	
}

void u3hClientForm::on_filesmgr_browsePathReady(QList<QTreeWidgetItem*>* dirsArray, QList<QTreeWidgetItem*>* filesArray) const
{
	if (ui.treeWidget_filesManager_dirs->selectedItems().count() > 0) {
		QTreeWidgetItem *selectedItem = ui.treeWidget_filesManager_dirs->selectedItems()[0];

		// remove childs item if present (e.g re-click on same folder)
		if (selectedItem->childCount() > 0) {
			QList<QTreeWidgetItem*> lstSub = selectedItem->takeChildren();
			QList<QTreeWidgetItem*>::iterator i;
			for (i = lstSub.begin(); i != lstSub.end(); ++i) {
				delete *i;
			}
		}
		selectedItem->addChildren(reinterpret_cast<QList<QTreeWidgetItem*>&>(*dirsArray));
		selectedItem->setExpanded(true);
	}

	ui.treeWidget_filesManager_files->clear();
	ui.treeWidget_filesManager_files->addTopLevelItems(reinterpret_cast<QList<QTreeWidgetItem*>&>(*filesArray));	
}

void u3hClientForm::FilesManagerDeleteFile()
{

	QString filePath = FilesManagerExtractSelectedFilePath();
	if (filePath != "")
	{
		packetBuilder * packet = new packetBuilder(_filesManager, deleteFile);
		packet->WriteData(const_cast<char*>(filePath.toStdString().c_str()), strlen(filePath.toStdString().c_str()), _string);
		packet->FinalizePacket();
		emit SendPacket(packet);

	}
}

void u3hClientForm::FilesManagerRenameFile()
{
	QTreeWidgetItem *selectedFile = ui.treeWidget_filesManager_files->selectedItems()[0];
	ui.treeWidget_filesManager_files->editItem(selectedFile, 0);
	QString filePath = FilesManagerExtractSelectedFilePath();
	if (filePath != "")
	{
		packetBuilder * packet = new packetBuilder(_filesManager, renameFile);
		packet->WriteData(const_cast<char*>(filePath.toStdString().c_str()), strlen(filePath.toStdString().c_str()), _string);
		packet->FinalizePacket();
		emit SendPacket(packet);

	}
}

void u3hClientForm::FilesManagerDownloadFile()
{
	QString filePath = FilesManagerExtractSelectedFilePath();
	if (filePath != "")
	{
		packetBuilder * packet = new packetBuilder(_filesManager, downloadFile);
		packet->WriteData(const_cast<char*>(filePath.toStdString().c_str()), strlen(filePath.toStdString().c_str()), _string);
		packet->FinalizePacket();
		emit SendPacket(packet);

	}
}


QString u3hClientForm::FilesManagerExtractSelectedFilePath()
{
	if (ui.treeWidget_filesManager_dirs->selectedItems().count() == 0 || ui.treeWidget_filesManager_files->selectedItems().count() == 0) {
		return "";
	}

	QString file = ui.treeWidget_filesManager_files->selectedItems()[0]->text(0);
	QTreeWidgetItem *rootPathSelected = ui.treeWidget_filesManager_dirs->selectedItems()[0];
	QString path = rootPathSelected->text(0);
	// current item
	QTreeWidgetItem * current = rootPathSelected;
	if (current->parent() != nullptr)
	{
		// while curent is not null
		while (current != nullptr)
		{
			// tmp is the parrent of current
			QTreeWidgetItem *tmp = current->parent();
			// if no parent, we are at the root object, wich assuming ending with \ 
			if (tmp != nullptr)
			{
				QString pp = tmp->text(0);
				if (utils::hasEnding(pp.toStdString(), "\\"))
				{
					path = tmp->text(0) + path;
				}
				else
				{
					path = tmp->text(0) + "\\" + path;
				}		
			}
			current = tmp;		
		}
	}

	QString fileCompletePath = path + "\\" + file;
	return fileCompletePath;
}


void u3hClientForm::on_treeWidget_filesManager_files_customContextMenuRequested(const QPoint & point) const
{
	QTreeWidgetItem * selected = ui.treeWidget_process->itemAt(point);
	if (selected != nullptr)
	{
		FilesMgrContextMenu->exec(QCursor::pos());
	}
}

#pragma endregion

#pragma region CaptureScreen

void u3hClientForm::on_captureScreen_label_addpixmap(QPixmap * image)
{
	int w = ui.label_capture_screen_view->width();
	int h = ui.label_capture_screen_view->height();

	// set a scaled pixmap to a w x h window keeping its aspect ratio 
	ui.label_capture_screen_view->setPixmap(image->scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	//ui.label_capture_screen_view->setPixmap(*image);
}

void u3hClientForm::on_captureScreen_act_getFrame()
{
	char * h = "hello";
	packetBuilder * packet = new packetBuilder(_captureScreen, getframe);
	packet->WriteData(h, strlen(h), _string);
	packet->FinalizePacket();
	emit SendPacket(packet);


}

void u3hClientForm::on_pushButton_CaptureScreen_Settings_clicked()
{
	ScreenCapSetting *s = new ScreenCapSetting();

	connect(s, SIGNAL(ApplyOptions(QString,QString,QString)), this, SLOT(CaptureScreenUpdateSettings(QString, QString, QString)));

	s->setAttribute(Qt::WA_DeleteOnClose);
	s->show();
}

void u3hClientForm::CaptureScreenUpdateSettings(QString quality, QString scaling, QString refresh)
{
	QString a;
	a += quality;
	a += "|";
	a += scaling;
	a += "|";
	a += refresh;


	packetBuilder * packet = new packetBuilder(_captureScreen, setOptions);
	packet->WriteData((char*)a.toStdString().c_str(), a.length(), _string);
	packet->FinalizePacket();
	emit SendPacket(packet);


}

#pragma endregion

void u3hClientForm::on_stackedWidget_currentChanged(int index) const
{
	switch(index)
	{
		case _ProcessManagerPage: {
			//on_ProcMgrRefreshEvents();
		}
	}
}

void u3hClientForm::closeEvent(QCloseEvent *eventp)
{
	emit WindowsClosed(this);
    eventp->accept();
}