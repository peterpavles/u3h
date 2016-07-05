#pragma once

#include <windows.h>
#include <QByteArray>

class CryptoEngine 
{

	

public:
	CryptoEngine();
	~CryptoEngine();

	static bool DecryptData(QByteArray data);

private:
	static int xtea_encryption(unsigned char buf[], unsigned long length, unsigned char key[], unsigned char** outbuf);
	static unsigned char* xtea_decryption(unsigned char buf[], int length, unsigned char key[]);

};

