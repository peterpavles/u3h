#include "configHandler.h"

#include "u3h.h"


configHandler::configHandler(u3H * u3h) : m_u3h(u3h)
{
	m_iniConfig = nullptr;

	char selffullpath[MAX_PATH];
	GetModuleFileNameA(nullptr, selffullpath, MAX_PATH);
	std::string fullpath = selffullpath;

	unsigned int found = fullpath.find_last_of("\\");
	m_configFilePath = fullpath.substr(0, found);
	m_configFilePath.append("\\").append(CONFIG_FILENAME);


	std::ifstream ifile(m_configFilePath.c_str());
	if(!ifile)  
	{
		std::ofstream outfile(m_configFilePath.c_str());
		outfile << "[u3h_conf_listener]\nconnexion_port=5050\ntransfert_port=5060\nautostart=false\n[u3h_conf_logger]\nclient_connexion=true\nclient_deconnexion=true\nerrors=true\npacket_in=true\npacket_out=true";
		outfile.close();

	}
	ifile.close();
}

configHandler::~configHandler()
{
	delete m_iniConfig;
}



bool configHandler::ConfigFileExist()
{
	configHandler *pthis = configHandler::Get_instance();
	return pthis->m_configFileExist;
}



void configHandler::LoadConfig()
{
	LoadConfigFromFile();	
}



void configHandler::WriteConfig() const
{
	if(m_iniConfig == nullptr)
		return;

	if(m_u3h->Gui()->lineEdit_connexionPort->text() != "")
		m_iniConfig->setValue("u3h_conf_listener/connexion_port", m_u3h->Gui()->lineEdit_connexionPort->text());
	if(m_u3h->Gui()->lineEdit_transfertsPort->text() != "")
		m_iniConfig->setValue("u3h_conf_listener/transfert_port", m_u3h->Gui()->lineEdit_transfertsPort->text());
	
	m_iniConfig->setValue("u3h_conf_listener/autostart", m_u3h->Gui()->checkBox_autostartlisteners->isChecked() ? "true" : "false");

}

void configHandler::LoadConfigFromFile()
{
	m_iniConfig = new QSettings(m_configFilePath.c_str(), QSettings::IniFormat);

	int connexionPort = m_iniConfig->value("u3h_conf_listener/connexion_port", 5000).toInt();
	int transfertPort = m_iniConfig->value("u3h_conf_listener/transfert_port", 5001).toInt();
	bool autoStartListeners = m_iniConfig->value("u3h_conf_listener/autostart", false).toBool();
	bool logconnexionClient = m_iniConfig->value("u3h_conf_logger/client_connexion", false).toBool();
	bool logdisconnexionClient = m_iniConfig->value("u3h_conf_logger/client_deconnexion", false).toBool();
	bool logerrors = m_iniConfig->value("u3h_conf_logger/errors", false).toBool();
	bool logpacketin = m_iniConfig->value("u3h_conf_logger/packet_in", false).toBool();
	bool logpacketout = m_iniConfig->value("u3h_conf_logger/packet_out", false).toBool();

	m_u3h->Gui()->lineEdit_connexionPort->setText(QString::number(connexionPort));
	m_u3h->Gui()->lineEdit_transfertsPort->setText(QString::number(transfertPort));	
	m_u3h->Gui()->checkBox_autostartlisteners->setChecked(autoStartListeners);

}