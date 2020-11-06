#ifndef LEVELEDITOR_ASSET_INSPECTOR
#define LEVELEDITOR_ASSET_INSPECTOR

#include "LevelEditor.h"
#include <set>
#include <map>

class AssetInspector : public LE_WinBase_Derived<AssetInspector>
{
	std::map<std::string, std::vector<std::string>> _allDirFiles;
	std::map<std::string, std::vector<std::string>> _allDirDirs;

	std::set<std::string> _ignoreFileTypes;
	std::string _selectedFolderPath;
	std::string _selectedFilePath;
	std::string _dragDropFilePath;
	std::string _currentFilePath;

	std::string _searchPath;

	bool _setScroll;

	void _RecursiveDirectoryTree(const std::string& path);

	void _RefreshDirectories(const std::string& path);

public:
	AssetInspector();
	~AssetInspector();

	void Init() override;

	void Run() override;

	void Exit() override;
};

#endif
