#include "u3h.h"
#include "u3hClientForm.h"
#include "ClientInformations.h"



ClientInformations::ClientInformations(u3hClientForm * client) : FunctionBase(client) 
{	
	u3H * pinst = u3H::Get_instance();
	connect(this, SIGNAL(on_newClientInformations(QTreeWidgetItem*)), pinst, SLOT(treeWidget_clients_addItem(QTreeWidgetItem*)), Qt::AutoConnection);
}
ClientInformations::~ClientInformations() { }






int ClientInformations::FunctionIdentity()
{
	return _clientInformation;
}

void ClientInformations::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
		case newclient: {
			
			break;
		}
		case updateInfos: {
			break;
		}
	}
	freePacket(packet);
}

