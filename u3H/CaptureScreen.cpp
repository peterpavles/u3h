#include "u3h.h"
#include "u3hClientForm.h"
#include "CaptureScreen.h"


CaptureScreen::CaptureScreen(u3hClientForm * client) : FunctionBase(client)
{
	connect(this, SIGNAL(GetFrame(QPixmap*)), GetClientForm(), SLOT(on_captureScreen_label_addpixmap(QPixmap *)));
}


CaptureScreen::~CaptureScreen()
{

}



int CaptureScreen::FunctionIdentity() { return _captureScreen; }

void CaptureScreen::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
		case getframe:
		{
			
			QPixmap p;
			
			if (p.loadFromData((uchar*)packet->data, packet->dwDataSize) )
			{
				emit GetFrame(&p);
			}
			break;
		}
	}
	freePacket(packet);
}