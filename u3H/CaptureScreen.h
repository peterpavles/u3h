#pragma once



#include "qt_Commons.h"
#include "FunctionBase.h"
#include "u3HClient.h"

//class u3h;
class u3hClientForm;

class CaptureScreen : public FunctionBase
{
	Q_OBJECT
public:

	CaptureScreen(u3hClientForm * clientPtr);
	~CaptureScreen();

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;

signals:
	void GetFrame(QPixmap *image);

};