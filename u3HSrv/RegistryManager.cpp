#include "RegistryManager.h"

RegistryManager::RegistryManager()
{

}

RegistryManager::~RegistryManager()
{

}

int RegistryManager::FunctionIdentity()
{
	return _registryManager;
}


void RegistryManager::ProcessPacket(PACKET * packet)
{

}

void RegistryManager::BrowseNode(int typeTree, string node)
{
	HKEY hKey;
	switch (typeTree)
	{
	case 1:
		hKey = HKEY_CLASSES_ROOT;
		break;
	case 2:
		hKey = HKEY_CURRENT_USER;
		break;
	case 3:
		hKey = HKEY_LOCAL_MACHINE;
		break;
	case 4:
		hKey = HKEY_USERS;
		break;
	case 5:
		hKey = HKEY_CURRENT_USER;
		break;
	}


	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys = 0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 


	// Enumerate the subkeys, until RegEnumKeyEx fails.
	if (cSubKeys)
	{
		printf("\nNumber of subkeys: %d\n", cSubKeys);
		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				//_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
			}
		}
	}
}

