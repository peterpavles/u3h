#pragma once

//#include "u3h.h"
#include "qt_Commons.h"
#include "FunctionBase.h"
#include "u3HClient.h"
#include "uQTreeWidgetItem.h"

class u3hClientForm;

class ProcessManager : public FunctionBase
{

	Q_OBJECT

public:
	ProcessManager(u3hClientForm * clientPtr);
	~ProcessManager();

	void ProcessList(QString processList);
	void ProcessDetail(QString data);

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;

signals :
	void NewProcessList(QList<uQTreeWidgetItem*> *);
};

