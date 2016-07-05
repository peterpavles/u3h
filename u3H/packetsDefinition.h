#pragma once

#include <windows.h>


#define HEADER_SIZE sizeof(_HEADER)
#define PACKET_SIZE_WDATA sizeof(_HEADER) + sizeof(DWORD) + sizeof(u_short)

#define _KillRemoteObj 0x302

enum _dataTpes
{
	_string				= 0x01,
	_bytes				= 0x02
};

enum PACKET_COMMAND
{
	_hello				= 0xFF,

	_clientMgmt			= 0xAA,

	_clientInformation	= 0x01,
	_remoteShell		= 0x02,
	_processManager		= 0x03,
	_filesManager		= 0x04,
	_captureScreen		= 0x05
};


enum _clientMgmt_SUBCMD
{
	disconnect			= 0x01
};

enum _captureScreen_SUBCMD
{
	getframe			= 0x01,
	setOptions			= 0x02
};

enum _clientINFOS_SUBCMD
{
	newclient			= 0x01,
	updateInfos			= 0x02
};

enum _fileManager_SUBCMD
{
	getdrives			= 0x01,
	browsePath			= 0x02,
	deleteFile			= 0x03,
	renameFile			= 0x04,
	downloadFile		= 0x05
};

enum _remoteSHELL_SUBCMD
{
	startshell			= 0x01,
	stopshell			= 0x02,
	restartshell		= 0x03,
	command				= 0x04
};

enum _processManager_SUBCMD
{
	processList			= 0x01,
	processBasicInfos	= 0x02,
	killprocess			= 0x03,
	newprocess			= 0x04
};

#pragma pack(push, 1)
struct _HEADER
{
	short Command;
	short subCommand;
	short Options;
	short subOption;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PACKET
{
	_HEADER _header;
	DWORD dwDataSize;
	u_short dataType;
	unsigned char * data;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct _HEADER_DL
{
	DWORD clientID;
	DWORD filenameSize;
	DWORD filePathSize;
	DWORD filebytesSize;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct PACKET_DL
{
	_HEADER_DL _header;
	unsigned char * filename;
	unsigned char * filepath;
	unsigned char * filedata;
};
#pragma pack(pop)

