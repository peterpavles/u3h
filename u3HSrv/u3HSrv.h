#pragma once


#include "common.h"
//#include "Local.h"
#include "u3HSrv.h"
#include "Conf.h"

class packetProcessor;
class socketClient;




class u3HSrv
{
public:
	u3HSrv();
	~u3HSrv();

	std::string g_HwGUID;

	bool Wmi_poller_ready;

private:
	socketClient * m_socketCLient;
	packetProcessor * m_packetProcessor;

	int FillHwGUID();
	


};

