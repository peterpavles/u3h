#include "WMIPoller.h"


WMIPoller::WMIPoller() : 
	m_pLoc(nullptr)
{
}


bool WMIPoller::InitializeWMI()
{
	m_hRes = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if(FAILED(m_hRes)) {
		return false;
		// Init com failed
	}

	m_hRes = CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);
	if(FAILED(m_hRes)) {
		// Init sec failed
		CoUninitialize();
		return false;
	}

	m_hRes = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID*>(&m_pLoc));
	if(FAILED(m_hRes)) {
		CoUninitialize();
		return false;
	}

	m_hRes = m_pLoc->ConnectServer(_bstr_t("ROOT\\CIMV2"), nullptr, nullptr, 0, NULL, 0, 0, &m_pSvc);
	if(FAILED(m_hRes)) {
		m_pLoc->Release();
		CoUninitialize();
		return false;
	}

	m_hRes = CoSetProxyBlanket(m_pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if(FAILED(m_hRes)) {
		m_pSvc->Release();
		m_pLoc->Release();
		CoUninitialize();
		return false;
	}
	return true;
}


IEnumWbemClassObject * WMIPoller::MakeWmiQuery(std::string query)
{
	IEnumWbemClassObject *ob = nullptr;
	m_hRes = m_pSvc->ExecQuery(bstr_t("WQL"), bstr_t(query.c_str()), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &ob);
	return ob;
}

WMIPoller::~WMIPoller()
{
}
