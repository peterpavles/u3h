#pragma once 

#include "qt_Commons.h"
#include "FunctionBase.h"
#include "u3HClient.h"
#include "uQTreeWidgetItem.h"
#include "utils.h"

class u3hClientForm;

class FilesManager : public FunctionBase
{

	Q_OBJECT

public:
	FilesManager(u3hClientForm *clientForm);
	~FilesManager();

private:
	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;

	void GetDrivesList(QString data);
	void BrowsePath(QString data);



signals:
	void ListDrives(QList<uQTreeWidgetItem*> *);
	void BrowsePathReady(QList<QTreeWidgetItem*>*, QList<QTreeWidgetItem*>*);
	
};