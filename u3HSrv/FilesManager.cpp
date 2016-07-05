#include "FilesManager.h"


FilesManager::FilesManager()
{
}

FilesManager::~FilesManager()
{
}


int FilesManager::FunctionIdentity() 
{ 
	return _filesManager;
}

void FilesManager::ProcessPacket(PACKET * packet)
{
	switch (packet->_header.subCommand)
	{
		case drivesList:
		{
			SendDrivesList();
			break;
		}
		case browsePath:
		{
			SendBrowsePath((char*)packet->data);
			break;
		}
		case renameFile:
		{
			RenameFileFromPath((char*)packet->data);
			break;
		}
		case deleteFile:
		{
			DeleteFileFromPath((char*)packet->data);
			break;
		}
		default:
		{
			break;
		}
	}
	freePacket(packet);
}

void FilesManager::SendDrivesList() const
{
	string drivesListstr;


	int dr_type;
	char dr_avail[256];
	char *temp = dr_avail;

	/* 1st we fill the buffer */
	GetLogicalDriveStrings(256, dr_avail);
	while (*temp != NULL)
	{
		string drive;
		drive += temp;
		drive += "|";
		dr_type = GetDriveType(temp);

		switch (dr_type)
		{
		case DRIVE_UNKNOWN:
			drive += "Unknown Drive type";
			break;
		case DRIVE_NO_ROOT_DIR: 
			drive += "Drive is invalid";
			break;
		case DRIVE_REMOVABLE: 
			drive += "Removable Drive";
			break;
		case DRIVE_FIXED: 
			drive += "Hard Disk";
			break;
		case DRIVE_REMOTE: 
			drive += "Network Drive";
			break;
		case DRIVE_CDROM: 
			drive += "CD-DVD Rom";
			break;
		case DRIVE_RAMDISK: 
			drive += "Ram Drive";
			break;
		}

		unique_ptr<char[]> volumeName(new char[MAX_PATH]);
		unique_ptr<char[]> filesystem(new char[MAX_PATH]);
		DWORD serialNumber = 0;
		DWORD maxComponentLen = 0;
		DWORD fileSystemFlags = 0;

		if (!GetVolumeInformation(temp, volumeName.get(), MAX_PATH, &serialNumber, &maxComponentLen, &fileSystemFlags, filesystem.get(), MAX_PATH)) {
			strcpy_s(volumeName.get(), 10, "error");
			strcpy_s(filesystem.get(), 10, "error");
		}

		drive += "|";
		drive += volumeName.get();
		drive += "|";
		drive += filesystem.get();

		temp += lstrlen(temp) + 1; // incriment the buffer
		if (drivesListstr == "") {
			drivesListstr += drive;
		}
		else {
			drivesListstr += "|#|";
			drivesListstr += drive;
		}

		packetBuilder *bld = new packetBuilder(_filesManager, drivesList);
		bld->WriteData((unsigned char*)drivesListstr.c_str(), drivesListstr.length(), _string);
		bld->FinalizePacket();
//		m_sendSerializedPacketCallback(bld->GetSerializedPacket(), bld->GetPacketSize());
		SendPacketCallback(bld);

		volumeName.reset();
		filesystem.reset();
//		delete bld;
	}
}

void FilesManager::SendBrowsePath(string pathT) const
{
	string directoryArray;
	string filesArray;

	string browseInfos;

	WIN32_FIND_DATA w32fd;
	LARGE_INTEGER filesize;
	//std::unique_ptr<char[]> szDir(new char[MAX_PATH]);
	HANDLE hFind;
	DWORD dwError;

	string toBrowse = pathT;
	if (hasEnding(toBrowse, "\\")) 
	{
		toBrowse += "*";
	}
	else 
	{
		toBrowse += "\\*";
	}

	if (pathT.length() > MAX_PATH - 3)
		return;


	hFind = FindFirstFile(toBrowse.c_str(), &w32fd);

	if (hFind == INVALID_HANDLE_VALUE)
		return;


	do
	{
		if (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			string filNa = w32fd.cFileName;

			if (!filNa.compare(".") || !filNa.compare(".."))
			{
				continue;
			}
			if (directoryArray.length() != 0) 
			{
				directoryArray += "*";
			}
			directoryArray += w32fd.cFileName;
		}
		else 
		{
			filesize.LowPart = w32fd.nFileSizeLow;
			filesize.HighPart = w32fd.nFileSizeHigh;
			// File + size

			if (filesArray.length() != 0)  
			{
				filesArray += "*";
			}

			string file;
			file += w32fd.cFileName;
			file += ":";
			file += to_string(filesize.QuadPart);

			filesArray += file;
		}
	}
	while (FindNextFile(hFind, &w32fd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		// error ? 
	}

	FindClose(hFind);


	if (directoryArray.length() > 0 || filesArray.length() > 0) 
	{
		browseInfos += directoryArray;
		browseInfos += "?";
		browseInfos += filesArray;
	}

	packetBuilder *bld = new packetBuilder(_filesManager, browsePath);
	bld->WriteData((unsigned char*)browseInfos.c_str(), browseInfos.length(), _string);
	bld->FinalizePacket();
//	m_sendSerializedPacketCallback(bld->GetSerializedPacket(), bld->GetPacketSize());
	SendPacketCallback(bld);

	//delete bld;

}

bool FilesManager::hasEnding(std::string const &fullString, std::string const &ending) 
{
	if (fullString.length() >= ending.length()) 
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else 
	{
		return false;
	}
}

void FilesManager::RenameFileFromPath(string data)
{
	vector<string> a = String_Utils::split(data, '|');
	if (a.size() == 2)
	{
		MoveFile(a.at(0).c_str(), a.at(1).c_str());
	}
}

void FilesManager::DeleteFileFromPath(string data)
{
	DeleteFile(data.c_str());
}

void FilesManager::DownloadFileFromPath(string data)
{
	if (!std::ifstream(data)) {
		// file not exist 
		return;
	}



}