#include "u3hClientForm.h"

#include "ProcessManager.h"


ProcessManager::ProcessManager(u3hClientForm * clientPtr) :  FunctionBase(clientPtr)
{
	connect(this, SIGNAL(NewProcessList(QList<uQTreeWidgetItem*>*)), GetClientForm(), SLOT(treeWidget_process_addItemArray(QList<uQTreeWidgetItem*>*)));
}


ProcessManager::~ProcessManager() { }

int ProcessManager::FunctionIdentity() { return _processManager; }

void ProcessManager::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
		case processList:
		{
			ProcessList(QString::fromLatin1((char*)packet->data));
			break;
		}
	}
	freePacket(packet);
}



void ProcessManager::ProcessList(QString processList)
{
	QList<QString> processArray = processList.split("#");
	QList<QString>::iterator i;

	QList<uQTreeWidgetItem*> itemsList;

	for (i = processArray.begin(); i != processArray.end(); ++i) {
		uQTreeWidgetItem * item = new uQTreeWidgetItem();
		item->setIcon(0, QIcon(":/u3hClientForm/u3hClientFormrsc/kprocess.png"));
		QString process = *i;
		QList<QString> procObj = process.split("|");
		QList<QString>::iterator j;
		int pos = 0;
		for (j = procObj.begin(); j != procObj.end(); ++j) {
			item->setText(pos, *j);
			pos++;
		}
		itemsList.push_back(item);
	}
	emit NewProcessList(&itemsList);
}

void ProcessManager::ProcessDetail(QString data)
{
	
}
