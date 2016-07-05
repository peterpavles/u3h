#include "CryptoEngine.h"


CryptoEngine::CryptoEngine()
{
}


CryptoEngine::~CryptoEngine()
{
}


bool CryptoEngine::DecryptData(QByteArray data)
{
	return true;
}

int CryptoEngine::xtea_encryption(UCHAR buf[], u_long length, UCHAR key[], UCHAR** outbuf)
{
	if ((buf == nullptr) || (key == nullptr)) {
		return -1;
	}
     
    u_long newsize = ((length + 1) / 8 + 1) * 8; // round up by 8
    *outbuf = new UCHAR[newsize + 2];
     
    u_long delta = 0x9e3779b9;  /* a key schedule constant */
    u_long sum;
     
    int n = 0;
    while (n < length)
    {
		sum = 0;
        u_long v0 = *reinterpret_cast<u_long*>(buf + n);
        u_long v1 = *reinterpret_cast<u_long*>(buf + n + 4);
     
        for(int i = 0; i < 32; i++)
        {
            v0 += ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + reinterpret_cast<u_long*>(key)[sum & 3]);
            sum += delta;
            v1 += ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + reinterpret_cast<u_long*>(key)[sum>>11 & 3]);
        }
     
        *reinterpret_cast<u_long*>(*outbuf+n+2) = v0;
        *reinterpret_cast<u_long*>(*outbuf+n+6) = v1;
     
        n += 8;
      }   
      // put length back in the package
      (*outbuf)[0] = newsize & 0xFF;
      (*outbuf)[1] = (newsize >> 8) & 0xFF;

      return newsize+2;
}


unsigned char* CryptoEngine::xtea_decryption(UCHAR buf[], int length, UCHAR key[])
{
	if ((buf == nullptr) || (key == nullptr))
		return nullptr;

	unsigned long delta = 0x9e3779b9;                   /* a key schedule constant */
	unsigned long sum;

	unsigned char* retbuf = new unsigned char[length];

	int n = 0;
	while (n < length)
	{
		sum = 0xC6EF3720;
		unsigned long v0 = *reinterpret_cast<unsigned long*>(buf+n);
		unsigned long v1 = *reinterpret_cast<unsigned long*>(buf+n+4);

		for(int i=0; i<32; i++)
		{
			v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + reinterpret_cast<unsigned long*>(key)[sum>>11 & 3]);
			sum -= delta;
			v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + reinterpret_cast<unsigned long*>(key)[sum & 3]);
		}

		*reinterpret_cast<unsigned long*>(retbuf+n)   = v0;
		*reinterpret_cast<unsigned long*>(retbuf+n+4) = v1;

		n += 8;
	}

	return retbuf;
}
