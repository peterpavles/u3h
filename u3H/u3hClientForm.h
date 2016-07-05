#pragma once

#include "qt_Commons.h"
#include "ui_u3hClientForm.h"
#include "u3HClient.h"
#include "packetBuilder.h"

#include "uQTreeWidgetItem.h"

#include "FunctionBase.h"

#include "ScreenCapSettings.h"

#include "utils.h"

class ClientInformations;
class RemoteShell;
class ProcessManager;
class FilesManager;
class CaptureScreen;

class u3hClientForm : public QMainWindow
{

	Q_OBJECT

public:
	u3hClientForm(u3HClient * client, QWidget*parent=nullptr);
	~u3hClientForm();

	QMenuBar *RemoteShellMenuBar;
	QMenuBar *ProcessManagerMenuBar;
	QMenuBar *FilesManagerMenuBar;
	QMenuBar *CaptureScreenMenuBar;
	QMenu *ProcessMgrContextMenu;
	QMenu *FilesMgrContextMenu;
	
	u3HClient *U3hClient() const
	{ return m_u3hClient; }
	Ui::Form *GetUI() { return &ui; } 

	//Remote Shell
	void SetRemoteShellRunning(bool isRun);


private:
	Ui::Form ui;
	u3HClient *m_u3hClient;

	enum StackedWidgetPageName
	{
		_BlankPage = 0x00,
		_RemoteShellPage = 0x01,
		_ClientPage = 0x02,
		_ProcessManagerPage = 0x03
	};


	void buildSignalSlots() const;
	void buildCustomUi();


	FunctionBase *GetOrInitFunction(int funcIdent);
	QList<FunctionBase*> m_functionList;
	RemoteShell *m_remoteShell;
	ProcessManager *m_processManager;
	ClientInformations *m_incommingClientInf;	

    // Remote Shell
	bool m_isRemoteShellRunning;

	// process manager
	QTimer *m_processMgrRefreshTimer;
	void ProcessManagerGetProcessBasicInfos(QString pid);


public slots:
	void on_treeWidget_selectPages_itemClicked(QTreeWidgetItem * item, int column) const;
	void on_stackedWidget_currentChanged(int index) const;

	void OnPacketReady(PACKET *packet);


	// Remote Shell
	void on_pushButton_remoteShell_sendCommand_clicked();
	void StartRemoteShell();
	void StopRemoteShell();
	void RestartRemoteShell();	
	// Process Manager
	void on_treeWidget_process_customContextMenuRequested(const QPoint & point) const;
	void on_setProcessManagerRefreshInterval(QAction* interval) const;
	void treeWidget_process_addItemArray(QList<uQTreeWidgetItem*> * itemArray) const;
	void on_ProcMgrRefreshEvents();
	void on_ProcMgrQtreeViewItemClicked(QTreeWidgetItem * item, int column);
	void on_ProcMgrActionKillProcessTriggered();
	void on_ProcMgrActionNewProcessTriggered();
	void on_ProcMgrActionOpenPathInBrowser();
	// Files Manager
	void on_treeWidget_filesManager_files_customContextMenuRequested(const QPoint & point) const;
	void on_treeWidget_filesManager_dirs_itemClicked(QTreeWidgetItem * item, int column);
	void FilesManagerGetDrivesList();
	void on_filesmgr_browsePathReady(QList<QTreeWidgetItem*>* dirsArray, QList<QTreeWidgetItem*>* filesArray) const;
	void treeWidget_filesMgr_folders_addItemArray(QList<uQTreeWidgetItem*>* itemArray) const;
	void treeWidget_filesMgr_files_addItemArray(QList<uQTreeWidgetItem*>* itemArray) const;
	void FilesManagerRenameFile();
	void FilesManagerDeleteFile();
	void FilesManagerDownloadFile();
	QString FilesManagerExtractSelectedFilePath();

	// Capture screen;
	void on_pushButton_CaptureScreen_Settings_clicked();
	void on_captureScreen_label_addpixmap(QPixmap * image);
	void on_captureScreen_act_getFrame();
	void CaptureScreenUpdateSettings(QString quality, QString scaling, QString refresh);

signals:
	void WindowsClosed(u3hClientForm * client);
	//void SendPacket(char * data, int size);
	void SendPacket(packetBuilder *bld);

protected:
	 void closeEvent(QCloseEvent *eventk) override;

};

