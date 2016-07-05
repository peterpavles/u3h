#pragma once

#include "common.h"
#include "SingletonTemplate.h"
#include "WMIPoller.h"



class WMIPoller : public Singleton<WMIPoller>
{
public:
	WMIPoller();
	~WMIPoller();
	bool InitializeWMI();
	
	IEnumWbemClassObject* MakeWmiQuery(std::string query);

private:
	IWbemLocator *m_pLoc;
	IWbemServices * m_pSvc;
	HRESULT m_hRes;

};

