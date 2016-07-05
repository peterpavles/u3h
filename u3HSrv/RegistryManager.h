#pragma once


#include "functionBase.h"


#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383


class RegistryManager : public FunctionBase
{
public:
	RegistryManager();
	~RegistryManager();

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;

private:
	void BrowseNode(int typeTree, string node);


};