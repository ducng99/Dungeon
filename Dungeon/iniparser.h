#ifndef __INIPARSER_H__
#define __INIPARSER_H__

#include <map>
#include <string>

using namespace std;

class IniParser
{
public:
	static IniParser& GetInstance();
	static void DestroyInstance();

	void LoadIniFile(const string& fileName);
	void SaveIniFile(const string& fileName);

	int GetValueAsInt(const string& section, const string& key);
	float GetValueAsFloat(const string& section, const string& key);
	string GetValueAsString(const string& section, const string& key);
	bool GetValueAsBool(const string& section, const string& key);
private:
	IniParser();
	IniParser(const IniParser& p);
private:
	static IniParser* sm_pInstance;

	map<string, string> m_iniSettings;
};

#endif // !__INIPARSER_H__