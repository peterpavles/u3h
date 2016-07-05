#pragma once


#include "qt_Commons.h"
#include "win_Commons.h"

#include "SingletonTemplate.h"

class u3H;

#define CONFIG_FILENAME "u3H.ini"

class configHandler : public Singleton<configHandler>
{
	friend class Singleton<configHandler>;
public:
	static bool ConfigFileExist();
	void LoadConfig();
	void LoadBaseConfig();
	void LoadConfigFromFile();
	void WriteConfig() const;

private:
	bool m_configFileExist;
	configHandler(u3H *);
	~configHandler();
	QSettings *m_iniConfig;
	std::string m_configFilePath;
	u3H *m_u3h;
};

