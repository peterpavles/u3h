#pragma once

#include "qt_Commons.h"
//#include "u3h.h"

#include "packetsDefinition.h"

class u3h;
class u3hClientForm;





class FunctionBase : public QObject
{

	Q_OBJECT

public:
	FunctionBase(u3hClientForm * clientFormPtr);
	~FunctionBase();

	virtual void ProcessPacket(PACKET *);
	virtual int  FunctionIdentity();

	//u3HClient *Getu3hClient();
	u3hClientForm *GetClientForm() const;
	void freePacket(PACKET * p);

private:
	u3hClientForm* m_clientForm;
	//u3HClient *m_client;



};