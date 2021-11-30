#include "iniparser.h"
#include "logmanager.h"

#include <fstream>
#include <cassert>

IniParser* IniParser::sm_pInstance = 0;

IniParser::IniParser()
{

}

IniParser & IniParser::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new IniParser();
	}

	assert(sm_pInstance);
	return *sm_pInstance;
}

void IniParser::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

void IniParser::LoadIniFile(const string& fileName)
{
	ifstream settingsFile(fileName);
	string line;

	if (settingsFile.is_open())
	{
		string section("");

		while (getline(settingsFile, line))
		{
			if (!line.empty())
			{
				if (line.find("[") != string::npos)
				{
					section = line.substr(1, line.length() - 2);
				}
				else if (line.find("=") != string::npos)
				{
					m_iniSettings[section + "|" + line.substr(0, line.find("="))] = line.substr(line.find("=") + 1);
				}
			}
		}
	}
	else
	{
		LogManager::GetInstance().Log("Cannot find ini file!");
	}
}

void IniParser::SaveIniFile(const string & fileName)
{
	ofstream settingsFile("Settings.ini");

	if (settingsFile.is_open())
	{
		
	}
}

int IniParser::GetValueAsInt(const string& section, const string& key)
{
	return stoi(m_iniSettings[section + "|" + key]);
}

float IniParser::GetValueAsFloat(const string& section, const string& key)
{
	return stof(m_iniSettings[section + "|" + key]);
}

string IniParser::GetValueAsString(const string& section, const string& key)
{
	return m_iniSettings[section + "|" + key];
}

bool IniParser::GetValueAsBool(const string& section, const string& key)
{
	return stoi(m_iniSettings[section + "|" + key]);
}
