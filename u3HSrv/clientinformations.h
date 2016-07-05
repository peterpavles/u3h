#pragma once

//#include "Local.h"

#include "functionBase.h"

#include "computerInformations.h"


class clientinformations : public FunctionBase
{

public:
	clientinformations();
	~clientinformations();

	void SendBaseInformations();

	int FunctionIdentity() override;
	void ProcessPacket(PACKET *) override;

	string g_HwGUID;


	int FillHwGUID();




};

