#include "u3h.h"
#include "u3hClientForm.h"
#include "FunctionBase.h"

FunctionBase::FunctionBase(u3hClientForm * clientFormPtr) : m_clientForm(clientFormPtr)
{
	
}

FunctionBase::~FunctionBase() { }


u3hClientForm *FunctionBase::GetClientForm() const
{
	return m_clientForm;
}

void FunctionBase::ProcessPacket(PACKET *) { }

int FunctionBase::FunctionIdentity()
{ 
	return -1; 
}


void FunctionBase::freePacket(PACKET * p)
{
	if (p->dwDataSize > 0)
	{
		free(p->data);
	}
	delete p;
}
