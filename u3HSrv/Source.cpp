
#include "u3HSrv.h"


u3HSrv * u3hserver = nullptr;


int ActivateSeDebugPrivilege(void)
{
	HANDLE hToken;
	LUID Val;
	TOKEN_PRIVILEGES tp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(GetLastError());

	if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &Val))
		return(GetLastError());

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = Val;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
		return(GetLastError());

	CloseHandle(hToken);

	return 1;
}





int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* argv, int nShowCmd)
{

	ActivateSeDebugPrivilege();
	u3hserver = new u3HSrv();
	delete u3hserver;
}


