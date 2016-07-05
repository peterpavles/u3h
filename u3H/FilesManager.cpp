#include "u3hClientForm.h"

#include "FilesManager.h"

FilesManager::FilesManager(u3hClientForm *clientForm) : FunctionBase(clientForm)
{
	connect(this, SIGNAL(ListDrives(QList<uQTreeWidgetItem*>*)), GetClientForm(), SLOT(treeWidget_filesMgr_folders_addItemArray(QList<uQTreeWidgetItem*>*)));
	connect(this, SIGNAL(BrowsePathReady(QList<QTreeWidgetItem*>*, QList<QTreeWidgetItem*>*)), GetClientForm(), SLOT(on_filesmgr_browsePathReady(QList<QTreeWidgetItem*>*, QList<QTreeWidgetItem*>*)));
}

FilesManager::~FilesManager()
{

}

int FilesManager::FunctionIdentity() { return _filesManager; }

void FilesManager::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
	case processList:
		{
			if (packet->dwDataSize > 0) 
			{
				GetDrivesList(QString::fromLatin1((char*)packet->data));
			}
			break;
		}
	case browsePath:
		{
			if (packet->dwDataSize > 0) 
			{
				BrowsePath(QString::fromLatin1((char*)packet->data));
			}
			break;
		}
		default:
		{
			break;
			
		}
	}
	freePacket(packet);
}

void FilesManager::GetDrivesList(QString data)
{

	QList<uQTreeWidgetItem*> itemLst;

	QList<QString> drives = data.split("|#|");
	QList<QString>::iterator i;
	for (i = drives.begin(); i != drives.end(); ++i)
	{
		QString dr = *i;
		QList<QString> o = dr.split("|");
		int ii;
		uQTreeWidgetItem *item = new uQTreeWidgetItem();
		for (ii = 0; ii != o.count() -1; ++ii) 
		{
			item->setText(ii, o.at(ii));
		}

		if (item->text(1) == "Hard Disk")  {
			item->setIcon(0, QIcon(":/u3hClientForm/u3hClientFormrsc/drive.png"));
		}
		else if (item->text(1) == "Network Drive")  {
			item->setIcon(0, QIcon(":/u3hClientForm/u3hClientFormrsc/drive_network.png"));
		}
		else if (item->text(1) == "CD-DVD Rom") {
			item->setIcon(0, QIcon(":/u3hClientForm/u3hClientFormrsc/drive_cd.png"));
		}
		else if (item->text(1) == "Removable Drive") {
			item->setIcon(0, QIcon(":/u3hClientForm/u3hClientFormrsc/drive.png"));
		}

		
		item->setText(0, o.at(0));
		item->setText(1, o.at(1));		

		itemLst.append(item);
	}

	emit(ListDrives(&itemLst));

}


void FilesManager::BrowsePath(QString data)
{
	QList<uQTreeWidgetItem*> folderLst;
	QList<uQTreeWidgetItem*> itemsLst;

	QList<QString> A = data.split("?");

	QString foldersString = A.at(0);
	QString filesString = A.at(1);

	if (foldersString.length() > 0) {
		QList<QString> foldersArray = foldersString.split("*");
		QList<QString>::iterator i;
		for (i = foldersArray.begin(); i != foldersArray.end(); ++i) {
			uQTreeWidgetItem *fo = new uQTreeWidgetItem();
			fo->setText(0, *i);
			fo->setIcon(0, QIcon(":/u3hClientForm/u3hClientFormrsc/folder.png"));
			folderLst.append(fo);
		}
	}

	if (filesString.length() > 0) {
		QList<QString> filesArray = filesString.split("*");
		QList<QString>::iterator ii;
		for (ii = filesArray.begin(); ii != filesArray.end(); ++ii) {
			uQTreeWidgetItem *it = new uQTreeWidgetItem();
			QString ob = *ii;
			QList<QString> obx = ob.split(":");
			it->setText(0, obx.at(0));
			it->setText(1, utils::stringBytetoHumanReadable(obx.at(1)));
			it->setData(1, BytesSizeRole, obx.at(1));
			it->setIcon(0, QIcon(":/u3hClientForm/u3hClientFormrsc/files.png"));
			itemsLst.append(it);
		}
	}

	emit BrowsePathReady((QList<QTreeWidgetItem*>*)&folderLst, (QList<QTreeWidgetItem*>*)&itemsLst);
}


