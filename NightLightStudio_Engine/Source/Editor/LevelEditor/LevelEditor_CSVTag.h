#ifndef LEVELEDITOR_CSV_TAG
#define LEVELEDITOR_CSV_TAG

#include "LevelEditor.h"
#include "../../Collision/CSVReader.h"


class LevelEditor_CSVTag : public LE_WinBase_Derived<LevelEditor_CSVTag>
{
	std::vector<std::vector<std::string>> _csvArr;
	CSVReader _fileReader;

	int _rowColNum;

	bool _isEngPlaying;

	void AddRowCol();
	void ClearRowCol();

public:
	LevelEditor_CSVTag();
	~LevelEditor_CSVTag();

	void Init() override;

	void Start() override;

	void Run() override;

	void Exit() override;

	void End() override;
};

#endif
