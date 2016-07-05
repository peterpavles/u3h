#pragma once

#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


// standard C++ headers
#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <functional>
#include <memory>

// standard C headers
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cstdarg>

// namespaces
using namespace std;

// standard windows headers

#include <windows.h>
#include <intrin.h>
#include <shlwapi.h>
#include <comdef.h>
#include <WbemIdl.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <dirent.h>
#include <objidl.h>
#include <gdiplus.h>

//using namespace Gdiplus;
//#pragma comment (lib,"Gdiplus.lib")

#pragma comment(lib, "version.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "psapi")
#pragma comment(lib, "comsuppw.lib")
