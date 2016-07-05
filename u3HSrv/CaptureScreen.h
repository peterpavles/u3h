#pragma once 

//#include "Local.h"

#include "functionBase.h"

#include "string_utils.h"



class CaptureScreen : public FunctionBase
{
public:
	CaptureScreen();
	~CaptureScreen();
	void SendCapture();
	void SetCaptureParam(PACKET *p);

	int FunctionIdentity() override;
	void ProcessPacket(PACKET * packet) override;

private:


	void initGDI();

	HWND m_hwndDesktop;
	HDC m_hDesktopDC;

	HDC m_hCompatibleDC;
	HBITMAP m_CompatibleBitmap;

	int m_screenHeight;
	int m_screenWidth;


	int m_scaledHeight;
	int m_scaledWidht;
	int m_qualityBits;
	int m_refreshMs;
	
	PVOID pBits;
	BITMAPINFO m_bmpInfo;
	BITMAPFILEHEADER m_bmpFileHeader;
};