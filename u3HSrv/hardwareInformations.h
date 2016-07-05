#pragma once


class socketClient;


#include "packetBuilder.h"
#include "Conf.h"
#include <Lmcons.h>
#include <string>
#include <functional>


class hardwareInformations
{
public:
	hardwareInformations();
	~hardwareInformations();

private:
	std::function<void(char*,int)> m_sendSerializedPacketCallback;
};

