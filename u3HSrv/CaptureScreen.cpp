#include "CaptureScreen.h"


CaptureScreen::CaptureScreen()
{
	pBits = nullptr;

	// Default color bits
	m_qualityBits = 24;
	// Get the screen metrics
	m_screenWidth = GetSystemMetrics(SM_CXSCREEN);
	m_screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// default values
	m_scaledWidht = m_screenWidth;
	m_scaledHeight = m_screenHeight;



	// File Bitmap info structure
	ZeroMemory(&m_bmpInfo, sizeof(BITMAPINFO));
	m_bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmpInfo.bmiHeader.biBitCount = m_qualityBits;
	m_bmpInfo.bmiHeader.biCompression = BI_RGB;
	m_bmpInfo.bmiHeader.biWidth = m_scaledWidht;
	m_bmpInfo.bmiHeader.biHeight = m_scaledHeight;
	m_bmpInfo.bmiHeader.biPlanes = 1;
	m_bmpInfo.bmiHeader.biSizeImage = abs(m_bmpInfo.bmiHeader.biHeight) * m_bmpInfo.bmiHeader.biWidth * m_bmpInfo.bmiHeader.biBitCount / 8;

	ZeroMemory(&m_bmpFileHeader, sizeof(BITMAPFILEHEADER));




	// Desktop windows hwnd
	m_hwndDesktop = GetDesktopWindow();
	// Device windows context
	m_hDesktopDC = GetDC(m_hwndDesktop);

	// creates a memory device context (DC) compatible with the specified device
	m_hCompatibleDC = CreateCompatibleDC(m_hDesktopDC);
	// creates a bitmap compatible with the device that is associated with the specified device context
	m_CompatibleBitmap = CreateDIBSection(m_hDesktopDC, &m_bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
	// selects an object into the specified device context (DC). The new object replaces the previous object of the same type
	SelectObject(m_hCompatibleDC, m_CompatibleBitmap);
}

CaptureScreen::~CaptureScreen()
{

	DeleteDC(m_hCompatibleDC);
	DeleteObject(m_CompatibleBitmap);
}

int CaptureScreen::FunctionIdentity() 
{ 
	return _captureScreen;
}

void CaptureScreen::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
		case getframe:
		{
			SendCapture();
			break;
		}
		case setOptions:
		{
			SetCaptureParam(packet);
		}
		default:
		{
			break;
		}
	}
	freePacket(packet);
}

void CaptureScreen::SendCapture()
{
	// Set the strech method
	SetStretchBltMode(m_hCompatibleDC, HALFTONE);
	// Stretch src bitmap to target bitmap
	StretchBlt(m_hCompatibleDC, 0, 0, m_scaledWidht, m_scaledHeight, m_hDesktopDC, 0, 0, m_screenWidth, m_screenHeight, SRCCOPY);


	
	LPVOID	pBuf = NULL;

	// If the lpvBits parameter is NULL and GetDIBits successfully fills the BITMAPINFO structure, the return value is nonzero.
	if (GetDIBits(m_hCompatibleDC, m_CompatibleBitmap, 0, 0, NULL, &m_bmpInfo, DIB_RGB_COLORS) != 0)
	{
		if (m_bmpInfo.bmiHeader.biSizeImage <= 0)
		{
			m_bmpInfo.bmiHeader.biSizeImage = m_bmpInfo.bmiHeader.biWidth * abs(m_bmpInfo.bmiHeader.biHeight) * (m_bmpInfo.bmiHeader.biBitCount + 7) / 8;
		}

		m_bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_bmpInfo.bmiHeader.biSizeImage;
		m_bmpFileHeader.bfType = 'MB';
		m_bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		int pos = 0;

		
		

		unique_ptr<char[]> buffer(new char[m_bmpFileHeader.bfSize]);
		memcpy(buffer.get(), &m_bmpFileHeader, sizeof(BITMAPFILEHEADER));
		pos += sizeof(BITMAPFILEHEADER);
		memcpy(buffer.get() + pos, &m_bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER));
		pos += sizeof(BITMAPINFOHEADER);
		//memcpy(buffer.get() + pos, pBuf, m_bmpInfo.bmiHeader.biSizeImage);
		GetDIBits(m_hCompatibleDC, m_CompatibleBitmap, 0, m_bmpInfo.bmiHeader.biHeight, buffer.get() + pos, &m_bmpInfo, DIB_RGB_COLORS);

		packetBuilder *bld = new packetBuilder(_captureScreen, getframe);
		bld->WriteData((unsigned char*)buffer.get(), m_bmpFileHeader.bfSize, _bytes);
		bld->FinalizePacket();
		//m_sendSerializedPacketCallback(bld->GetSerializedPacket(), bld->GetPacketSize());
		SendPacketCallback(bld);
		buffer.reset();
		//delete bld;

	}

	
	/*if (m_bmpInfo.bmiHeader.biSizeImage <= 0)  
	{
		m_bmpInfo.bmiHeader.biSizeImage = m_bmpInfo.bmiHeader.biWidth * abs(m_bmpInfo.bmiHeader.biHeight) * (m_bmpInfo.bmiHeader.biBitCount + 7) / 8;
	}


	if ((pBuf = malloc(m_bmpInfo.bmiHeader.biSizeImage)) == NULL) 
	{
		MessageBox(NULL, "Unable to Allocate Bitmap Memory", "Error", MB_OK | MB_ICONERROR);	
	}


	GetDIBits(m_hCompatibleDC, m_CompatibleBitmap, 0, m_bmpInfo.bmiHeader.biHeight, pBuf, &m_bmpInfo, DIB_RGB_COLORS);

	m_bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_bmpInfo.bmiHeader.biSizeImage;
	m_bmpFileHeader.bfType = 'MB';
	m_bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


	int pos = 0;
	std::unique_ptr<char[]> buffer(new char[m_bmpFileHeader.bfSize]);

	memcpy(buffer.get(), &m_bmpFileHeader, sizeof(BITMAPFILEHEADER));
	pos += sizeof(BITMAPFILEHEADER);
	memcpy(buffer.get() + pos, &m_bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER));
	pos += sizeof(BITMAPINFOHEADER);
	memcpy(buffer.get() + pos, pBuf, m_bmpInfo.bmiHeader.biSizeImage);

	packetBuilder *bld = new packetBuilder(_captureScreen, getframe);
	bld->WriteData((unsigned char*)buffer.get(), m_bmpFileHeader.bfSize, _bytes);
	bld->FinalizePacket();
	m_sendSerializedPacketCallback(bld->GetSerializedPacket(), bld->GetPacketSize());

	// deleting buffer
	buffer.reset();
	*/

}

void CaptureScreen::SetCaptureParam(PACKET *p)
{
	string param((char*)p->data);
	vector<string> paramArray = String_Utils::split(param, '|');

	if (paramArray.size() != 3)
	{
		return;
	}

	int quality = stoi(paramArray.at(0));
	if (quality == 32 || quality == 24 || quality == 16 || quality == 8)
	{
		m_qualityBits = quality;
	}

	int scaling = stoi(paramArray.at(1));
	if (scaling == 100 || scaling == 75 || scaling == 50 || scaling == 25)
	{
		m_scaledHeight = m_screenHeight * scaling / 100;
		m_scaledWidht = m_screenWidth * scaling / 100;

		m_bmpInfo.bmiHeader.biWidth = m_scaledWidht;
		m_bmpInfo.bmiHeader.biHeight = m_scaledHeight;

	}

	int refresh = stoi(paramArray.at(2));
	if (refresh == 30 || refresh == 20 || refresh == 15 || refresh == 10 || refresh == 5 || refresh == 3 || refresh == 2 || refresh == 1 || refresh == 0)
	{
		m_refreshMs = refresh * 1000;
	}

	
}

