#pragma once



#include "qt_Commons.h"
#include "FunctionBase.h"
#include "u3HClient.h"

//class u3h;
class u3hClientForm;

class ClientInformations : public FunctionBase
{
	Q_OBJECT

public:
	ClientInformations(u3hClientForm * clientPtr);
	~ClientInformations();

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;


signals:
	void on_newClientInformations(QTreeWidgetItem * item);
};

