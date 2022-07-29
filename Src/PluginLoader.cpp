#include "PluginLoader.h"
#include <QDebug>
#include <vector>
#include <sstream>
#include <filesystem>

#ifdef _WIN32
	#include <stdio.h>
	#include <tchar.h>
	#include <string.h>
	#include <atlbase.h>
	#include <QDebug>
	#include <QFileInfo>
	#define MAX_KEY_LENGTH 255
	#define MAX_VALUE_NAME 16383

std::vector<std::string> QueryKey(HKEY hKey, std::string path)
{
	// See
	// https://docs.microsoft.com/en-us/windows/desktop/sysinfo/enumerating-registry-subkeys
	std::vector<std::string> list;
	TCHAR    achClass[MAX_PATH] = TEXT(""); // buffer for class name
	DWORD    cchClassName       = MAX_PATH; // size of class string
	DWORD    cSubKeys           = 0;        // number of subkeys
	DWORD    cbMaxSubKey;                   // longest subkey size
	DWORD    cchMaxClass;                   // longest class string
	DWORD    cValues;                       // number of values for key
	DWORD    cchMaxValue;                   // longest value name
	DWORD    cbMaxValueData;                // longest value data
	DWORD    cbSecurityDescriptor;          // size of security descriptor
	FILETIME ftLastWriteTime;               // last write time

	DWORD i, retCode;

	TCHAR achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count.
	retCode = RegQueryInfoKey(hKey,            // key handle
	                          achClass,        // buffer for class name
	                          &cchClassName,   // size of class string
	                          NULL,            // reserved
	                          &cSubKeys,       // number of subkeys
	                          &cbMaxSubKey,    // longest subkey size
	                          &cchMaxClass,    // longest class string
	                          &cValues,        // number of values for this key
	                          &cchMaxValue,    // longest value name
	                          &cbMaxValueData, // longest value data
	                          &cbSecurityDescriptor, // security descriptor
	                          &ftLastWriteTime);     // last write time

	// Enumerate the key values.
	if (cValues) {
		// printf( "\nNumber of values: %d\n", cValues);

		for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++) {
			cchValue    = MAX_VALUE_NAME;
			achValue[0] = '\0';
			retCode =
			   RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL);

			if (retCode == ERROR_SUCCESS) {
				CRegKey regKey;
				CHAR    szBuffer[512];
				ULONG   dwBufferSize = sizeof(szBuffer);

				if (auto error_code = regKey.Open(HKEY_LOCAL_MACHINE,
				                                  path.c_str(),
				                                  KEY_READ);
				    error_code != ERROR_SUCCESS) {
					qWarning() << "Error opening windows registry path "
					           << path.c_str() << ": " << error_code;
					regKey.Close();
					continue;
				}
				if (auto error_code = regKey.QueryStringValue(achValue,
				                                              szBuffer,
				                                              &dwBufferSize);
				    error_code != ERROR_SUCCESS) {
					qWarning() << "Error opening windows registry value "
					           << achValue << ": " << error_code;
					regKey.Close();
					continue;
				}

				std::string fp = szBuffer;
				std::replace(fp.begin(), fp.end(), '\\', '/');
				list.push_back(fp);
			}
		}
	}
	return list;
}
#endif

std::vector<std::string> PluginLoader::queryRegistryBehaviors(std::string path)
{
	std::vector<std::string> list;

#ifdef _WIN32
	HKEY hTestKey;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
	                 TEXT(path.c_str()),
	                 0,
	                 KEY_READ,
	                 &hTestKey) == ERROR_SUCCESS) {
		list = QueryKey(hTestKey, path);
	}

	RegCloseKey(hTestKey);
#endif

	return list;
}

#ifdef _WIN32
const char* WinGetEnv(const char* name)
{
	const DWORD buffSize = 65535;
	static char buffer[buffSize];
	if (GetEnvironmentVariableA(name, buffer, buffSize)) {
		return buffer;
	} else {
		return 0;
	}
}
bool WinSetEnv(const char* name, const char* toWhat)
{
	return SetEnvironmentVariableA(name, toWhat);
}
#endif

bool is_shared_library(std::filesystem::path const& p)
{
	auto extension = p.extension();
	return extension == ".so" || extension == ".dylib" || extension == ".dll";
}

const char* PluginLoader::addDllPath(std::string f)
{
// Get the directory of the DLL/*.so and add it to the PATH env variable.
// This way dependencies can be shipped in the same directory
#ifdef _WIN32
	QFileInfo finf(f.c_str());
	// rather than the buggy _getenv:
	// https://docs.microsoft.com/de-de/windows/desktop/api/winbase/nf-winbase-getenvironmentvariable
	auto old_path = WinGetEnv("PATH");
	auto path     = std::ostringstream();
	if (old_path) {
		path << old_path << ";" << finf.absolutePath().toStdString().c_str();
		WinSetEnv("PATH", path.str().c_str());
	} else {
		qWarning() << "Failed to get and modify PATH enviromental variable.";
	}
	return old_path;
#endif

	return "";
}

void PluginLoader::delDllPath(const char* oldPath)
{
// reset path. We don't want some weird cross-effects
#ifdef _WIN32
	if (oldPath) {
		WinSetEnv("PATH", oldPath);
	}
#endif
}

bool endsWith(std::string value, std::string ending)
{
	std::transform(value.begin(), value.end(), value.begin(), ::tolower);
	std::transform(ending.begin(), ending.end(), ending.begin(), ::tolower);
	if (ending.size() > value.size())
		return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool validSuffix(std::string f, std::string suffix)
{
	return (endsWith(f, suffix + ".dll") || endsWith(f, suffix + ".so"));
}

std::vector<std::string> PluginLoader::searchDirectoriesForPlugins(
   std::vector<std::string> list,
   std::string              suffix)
{
	// Search directories
	std::vector<std::string> filesFromFolders;

	for (auto f : list) {
		auto path = std::filesystem::path(f);
		if (path.empty()) {
			continue;
		}
		if (std::filesystem::is_directory(path)) {
			try {
				for (auto& e : std::filesystem::directory_iterator(path)) {
					auto p = e.path();
					if (is_shared_library(p) &&
					    p.replace_extension().extension() == suffix) {
						filesFromFolders.push_back(e.path().string());
					}
				}
			} catch (std::filesystem::filesystem_error const& e) {
				qWarning() << e.what();
			}
		} else if (is_shared_library(path)) {
			filesFromFolders.push_back(f);
		} else {
			qWarning() << "Neither a directory, nor a shared library:"
			           << f.data();
		}
	}

	return filesFromFolders;
}

PluginLoader::PluginLoader(QObject* parent)
{
	m_MetaData        = nullptr;
	m_PluginLoader    = new QPluginLoader(this);
	m_PluginListModel = new QStringListModel();
}
PluginLoader::~PluginLoader()
{
	delete m_PluginLoader;
	delete m_PluginListModel;
}

bool PluginLoader::loadPluginFromFilename(QString const& filename)
{
	bool retval = false;
	if (m_PluginLoader->isLoaded()) {
		m_PluginLoader->unload();
	}

	bool isLib = QLibrary::isLibrary(filename);

	if (isLib) {

		auto oldPath = PluginLoader::addDllPath(filename.toStdString());
		m_PluginLoader->setFileName(filename);

		readMetaDataFromPlugin();

		retval         = m_PluginLoader->load();
		QString     s  = m_PluginLoader->errorString();
		std::string ss = s.toStdString();
		addPluginnameToLists(getCurrentPluginName(), filename);

		if (!m_PluginLoader->isLoaded()) {
			qWarning() << ss.c_str();
			retval = false;
		}
		PluginLoader::delDllPath(oldPath);
	} else {
		retval = false;
	}

	return retval;
}

void PluginLoader::addPluginnameToLists(QString mstring, QString filename)
{
	if (!m_PluginList.contains(mstring))
		m_PluginList.append(mstring);
	m_PluginListModel->setStringList(m_PluginList);
	m_PluginMap.insert(std::pair<QString, QString>(mstring, filename));
}

bool PluginLoader::loadPluginFromName(QString name)
{
	QString filename = m_PluginMap.find(name)->second;
	return loadPluginFromFilename(filename);
}

int PluginLoader::addToPluginList(QString filename, QString suffix)
{
	if (!validSuffix(filename.toStdString(), suffix.toStdString()))
		return 1;

	bool isLib = QLibrary::isLibrary(filename);

	if (isLib) {

		QPluginLoader loader;
		loader.setFileName(filename);
		QJsonValue  pluginMeda(loader.metaData().value("MetaData"));
		QJsonObject metaObj = pluginMeda.toObject();
		QString     mstring = metaObj.value("name").toString();
		addPluginnameToLists(mstring, filename);
	} else {
		return 2;
		qWarning() << "Error reading plugin: " << filename;
	}
	return 0;
}

QStringListModel* PluginLoader::getPluginList()
{
	return m_PluginListModel;
}

QObject* PluginLoader::getPluginInstance()
{
	return (m_PluginLoader->instance());
}

QJsonObject PluginLoader::getPluginMetaData() const
{
	if (m_MetaData == nullptr)
		qFatal("(getPluginMetaData) No plugin loaded");
	return *m_MetaData;
}

void PluginLoader::readMetaDataFromPlugin()
{
	m_MetaData = std::make_shared<QJsonObject>(
	   m_PluginLoader->metaData().value("MetaData").toObject());
}

bool PluginLoader::getIsPluginLoaded()
{
	return m_isPluginLoaded;
}

QString PluginLoader::getCurrentPluginName()
{
	if (m_MetaData == nullptr)
		return "Error name";
	return m_MetaData->value("name").toString();
}
const std::map<QString, QString>& PluginLoader::getPluginMap() const
{
	return m_PluginMap;
}
