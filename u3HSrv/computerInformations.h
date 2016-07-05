#pragma once



// common includes
#include "common.h"

#include <Lmcons.h>
#include <memory>
#include <Strsafe.h>
#include <Shlobj.h>


#define VISTA "Vista"
#define WIN7  "Windows 7"
#define Win2K8 "Windows Server 2008"
typedef void (WINAPI *PGETSYSTEMINFO)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGETPRODUCTINFO)(DWORD, DWORD, DWORD, DWORD, PDWORD);


class computerInformations
{
public:

	static string _ComputerName()
	{
		DWORD dw = MAX_COMPUTERNAME_LENGTH + 1;
		std::unique_ptr<char[]> buffer(new char[dw]);
		if (!GetComputerName(static_cast<LPSTR>(buffer.get()), &dw)) {
			strcpy_s(static_cast<LPSTR>(buffer.get()), 10, "Error");
		}
		return string(buffer.get());
	}

	static string _ComputerHardwarGUID()
	{
		HW_PROFILE_INFO hwProfilInfo;
		GetCurrentHwProfile(&hwProfilInfo);
		return hwProfilInfo.szHwProfileGuid;
	}


	static void _ComputerMemory(_Out_ ULONGLONG memory)
	{
		if(!GetPhysicallyInstalledSystemMemory(&memory))
		{
			memory = 0;
		}
	}

	static string _UserName()
	{
		DWORD dwSize = UNLEN + 1;
		std::unique_ptr<char[]> buffer(new char[dwSize]);
		if (!GetUserNameA(buffer.get(), &dwSize)) {
			strcpy_s(buffer.get(), dwSize, "Error");
		}
		return string(buffer.get());
	}

	static string _OperatingSystem()
	{

		DWORD dwSize = 150;
		
		OSVERSIONINFOEX osvi;
		SYSTEM_INFO si;
		PGETSYSTEMINFO pGetSystemInfo;
		PGETPRODUCTINFO pGetProductInfo;
		DWORD dwType;

		std::unique_ptr<char[]> buffer(new char[dwSize]);

		ZeroMemory(&si, sizeof(SYSTEM_INFO));
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		
		#pragma warning(push)
		#pragma warning(disable : 4996)
		if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&osvi))) {
			strcpy_s(buffer.get(), dwSize, "error");
			return string(buffer.get());
		}
		#pragma warning(pop) 
		
		pGetSystemInfo = reinterpret_cast<PGETSYSTEMINFO>(GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo"));
		if (NULL != pGetSystemInfo) {
			pGetSystemInfo(&si);
		}
		else {
			GetSystemInfo(&si);
		}

		if (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && osvi.dwMajorVersion > 4)
		{
			StringCchCopy(buffer.get(), dwSize, "Microsoft ");
			if (osvi.dwMajorVersion == 6) 
			{
				if (osvi.dwMinorVersion == 0) 
				{
					if (osvi.wProductType == VER_NT_WORKSTATION) 
					{
						StringCchCat(buffer.get(), dwSize, "Windows Vista ");
					}
					else {
						StringCchCat(buffer.get(), dwSize, "Windows Server 2008 ");
					}
				}
				if (osvi.dwMinorVersion == 1) {
					if (osvi.wProductType == VER_NT_WORKSTATION) {
						StringCchCat(buffer.get(), dwSize, "Windows 7 ");
					}
					else {
						StringCchCat(buffer.get(), dwSize, "Windows Server 2008 R2 ");
					}
				}

				pGetProductInfo =    reinterpret_cast<PGETPRODUCTINFO>(GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo"));
				pGetProductInfo(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

				switch (dwType) {
				case PRODUCT_ULTIMATE: {
					StringCchCat(buffer.get(), dwSize, "Ultimate Edition");
					break;
				}
				case 0x00000030: {
					StringCchCat(buffer.get(), dwSize, "Professional");
					break;
				}
				case PRODUCT_HOME_PREMIUM: {
					StringCchCat(buffer.get(), dwSize, "Home Premium Edition");
					break;
				}
				case PRODUCT_HOME_BASIC: {
					StringCchCat(buffer.get(), dwSize, "Home Basic Edition");
					break;
				}
				case PRODUCT_ENTERPRISE: {
					StringCchCat(buffer.get(), dwSize, "Enterprise Edition");
					break;
				}
				case PRODUCT_BUSINESS: {
					StringCchCat(buffer.get(), dwSize, "Business Edition");
					break;
				}
				case PRODUCT_STARTER: {
					StringCchCat(buffer.get(), dwSize, "Starter Edition");
					break;
				}
				case PRODUCT_CLUSTER_SERVER: {
					StringCchCat(buffer.get(), dwSize, "Cluster Server Edition");
					break;
				}
				case PRODUCT_DATACENTER_SERVER: {
					StringCchCat(buffer.get(), dwSize, "Datacenter Edition");
					break;
				}
				case PRODUCT_DATACENTER_SERVER_CORE: {
					StringCchCat(buffer.get(), dwSize, "Datacenter Edition (core installation)");
					break;
				}
				case PRODUCT_ENTERPRISE_SERVER: {
					StringCchCat(buffer.get(), dwSize, "Enterprise Edition");
					break;
				}
				case PRODUCT_ENTERPRISE_SERVER_CORE: {
					StringCchCat(buffer.get(), dwSize, "Enterprise Edition (core installation)");
					break;
				}
				case PRODUCT_ENTERPRISE_SERVER_IA64: {
					StringCchCat(buffer.get(), dwSize, "Enterprise Edition for Itanium-based Systems");
					break;
				}
				case PRODUCT_SMALLBUSINESS_SERVER: {
					StringCchCat(buffer.get(), dwSize, "Small Business Server");
					break;
				}
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM: {
					StringCchCat(buffer.get(), dwSize, "Small Business Server Premium Edition");
					break;
				}
				case PRODUCT_STANDARD_SERVER: {
					StringCchCat(buffer.get(), dwSize, "Standard Edition");
					break;
				}
				case PRODUCT_STANDARD_SERVER_CORE: {
					StringCchCat(buffer.get(), dwSize, "Standard Edition (core installation)");
					break;
				}
				case PRODUCT_WEB_SERVER: {
					StringCchCat(buffer.get(), dwSize, "Web Server Edition");
					break;
				}
				}
			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
				if (GetSystemMetrics(SM_SERVERR2)) {
					StringCchCat(buffer.get(), dwSize, "Windows Server 2003 R2, ");
				}
				else if (osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER) {
					StringCchCat(buffer.get(), dwSize, "Windows Storage Server 2003");
				}
				else if (osvi.wSuiteMask & 0x00008000) {
					StringCchCat(buffer.get(), dwSize, "Windows Home Server");
				}
				else if (osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
					StringCchCat(buffer.get(), dwSize, "Windows XP Professional x64 Edition");
				}
				else  {
					StringCchCat(buffer.get(), dwSize, "Windows Server 2003, ");
				}

				// Test for the server type.
				if (osvi.wProductType != VER_NT_WORKSTATION) {
					if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER) {
							StringCchCat(buffer.get(), dwSize, "Datacenter Edition for Itanium-based Systems");
						}
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) {
							StringCchCat(buffer.get(), dwSize, "Enterprise Edition for Itanium-based Systems");
						}
					}

					else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER) {
							StringCchCat(buffer.get(), dwSize, "Datacenter x64 Edition");
						}
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) {
							StringCchCat(buffer.get(), dwSize, "Enterprise x64 Edition");
						}
						else {
							StringCchCat(buffer.get(), dwSize, "Standard x64 Edition");
						}
					}
					else {
						if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER)
							StringCchCat(buffer.get(), dwSize, "Compute Cluster Edition");
						else if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							StringCchCat(buffer.get(), dwSize, "Datacenter Edition");
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							StringCchCat(buffer.get(), dwSize, "Enterprise Edition");
						else if (osvi.wSuiteMask & VER_SUITE_BLADE)
							StringCchCat(buffer.get(), dwSize, "Web Edition");
						else StringCchCat(buffer.get(), dwSize, "Standard Edition");
					}
				}
			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {
				StringCchCat(buffer.get(), dwSize, "Windows XP ");
				if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
					StringCchCat(buffer.get(), dwSize, "Home Edition");
				else StringCchCat(buffer.get(), dwSize, "Professional");
			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
				StringCchCat(buffer.get(), dwSize, "Windows 2000 ");
				if (osvi.wProductType == VER_NT_WORKSTATION) {
					StringCchCat(buffer.get(), dwSize, "Professional");
				}
				else {
					if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
						StringCchCat(buffer.get(), dwSize, "Datacenter Server");
					else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						StringCchCat(buffer.get(), dwSize, "Advanced Server");
					else StringCchCat(buffer.get(), dwSize, "Server");
				}
			}

			// Include service pack (if any) and build number.

			if (strlen(osvi.szCSDVersion) > 0) {
				StringCchCat(buffer.get(), dwSize, " ");
				StringCchCat(buffer.get(), dwSize, osvi.szCSDVersion);
			}

			TCHAR buf[80];
			StringCchPrintf(buf, 80, " (build %d)", osvi.dwBuildNumber);
			StringCchCat(buffer.get(), dwSize, buf);

			if (osvi.dwMajorVersion >= 6) {
				if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					StringCchCat(buffer.get(), dwSize, ", 64-bit");
				else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
					StringCchCat(buffer.get(), dwSize, ", 32-bit");
			}
		}
		else {
			strcpy_s(buffer.get(), dwSize, "error");
			return string(buffer.get());
		}

		return string(buffer.get());

	}

	static string _UserPrivileges()
	{
		DWORD dwSize = 20;
		std::unique_ptr<char[]> buffer(new char[dwSize]);
		if (IsUserAnAdmin())
		{
			strcpy_s(buffer.get(), dwSize, "Administrator");
		}
		else
		{
			strcpy_s(buffer.get(), dwSize, "User");
		}
		return string(buffer.get());
	}
};