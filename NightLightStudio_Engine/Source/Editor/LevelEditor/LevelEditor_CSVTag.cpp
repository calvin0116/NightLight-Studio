#include "LevelEditor_CSVTag.h"
#include <iostream>

bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void LevelEditor_CSVTag::AddRowCol()
{
	_rowColNum++;
	_csvArr.emplace_back();

	for (int i = 0; i < _csvArr.size(); ++i)
	{
		_csvArr[i].resize(_rowColNum);
		if (!i)
		{
			for (int j = 1; j < _csvArr[i].size(); ++j)
			{
				_csvArr[i][j] = std::to_string(j);
			}
		}
		else
		{
			_csvArr[i][0] = std::to_string(i);
		}
	}

}

void LevelEditor_CSVTag::ClearRowCol()
{
	_rowColNum--;
	_csvArr.pop_back();

	for (int i = 0; i < _csvArr.size(); ++i)
	{
		_csvArr[i].resize(_rowColNum);
		if (!i)
		{
			for (int j = 1; j < _csvArr[i].size(); ++j)
			{
				_csvArr[i][j] = std::to_string(j);
			}
		}
		else
		{
			_csvArr[i][0] = std::to_string(i);
		}
	}
}

LevelEditor_CSVTag::LevelEditor_CSVTag()
	: _csvArr{}, _fileReader{ "Asset/CollisionConfig.csv" }, _rowColNum{}, _isEngPlaying{ false }
{
}

LevelEditor_CSVTag::~LevelEditor_CSVTag()
{
}

void LevelEditor_CSVTag::Start()
{
	_csvArr = _fileReader.getData();
	_rowColNum = (int)_csvArr.size();

	if (!_rowColNum)
	{
		AddRowCol();
		AddRowCol();
	}

	/*
	for (const std::vector<std::string>& vec : data)
	{
		for (const std::string& str : vec)
		{
			std::cout << str << " ";
		}
		std::cout << std::endl;
	}
	*/
}

void LevelEditor_CSVTag::Init()
{
	ImGui::SetNextWindowContentSize(ImVec2(_rowColNum * 30.0f, 0));
}

void LevelEditor_CSVTag::Run()
{
	_levelEditor->LE_AddButton("Add##CSV",
		[this]()
		{
			AddRowCol();
		});
	ImGui::SameLine();
	_levelEditor->LE_AddButton("Del##CSV",
		[this]()
		{
			if (_rowColNum > 2)
				ClearRowCol();
		});

	ImGui::Columns(_rowColNum, "csvColumn", false);

	int j = 0;
	bool changed = false;

	while (j < _rowColNum)
	{
		ImGui::SetColumnWidth(-1, 30);
		for (int i = 0; i < _csvArr.size(); ++i)
		{
			
			if (!i)
			{
				ImGui::Text(std::to_string(j).c_str());
			}
			else if (!j)
			{
				ImGui::Dummy(ImVec2(0, 2));
				ImGui::Text(std::to_string(i).c_str());
			}
			else
			{
				
				bool val;
				if (is_number(_csvArr[i][j]))
				{
					val = std::stoi(_csvArr[i][j]);
				}
				else
				{
					val = 0;
				}
				if (j <= i)
				{
					if (ImGui::Checkbox(std::string("##" + std::to_string(i)).append(std::to_string(j)).c_str(), &val))
					{
						changed = true;
					}
				}
				else
					ImGui::Dummy(ImVec2(19,19));
				_csvArr[i][j] = std::string(std::to_string(val));
				_csvArr[j][i] = _csvArr[i][j];
			}
		}
		++j;
		
		ImGui::NextColumn();
	}

	if (changed)
		_fileReader.writeData(_csvArr);
	

	/*
	for (int i = 0; i < _csvArr.size(); ++i)
	{
		for (int j = 0; j < _csvArr[i].size(); ++j)
		{
			if (j)
				ImGui::SameLine();
			if (!i)
			{
				ImGui::Text(std::to_string(j).c_str());
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(2, 0));
			}
			else if (!j)
			{
				ImGui::Text(std::to_string(i).c_str());
			}
			else
			{
				bool val;
				if (is_number(_csvArr[i][j]))
				{
					val = std::stoi(_csvArr[i][j]);
				}
				else
				{
					val = 0;
				}
				ImGui::Checkbox(std::string("##" + std::to_string(i)).append(std::to_string(j)).c_str(), &val);
				_csvArr[i][j] = std::string(std::to_string(val));
			}
		}
	}
	*/
}

void LevelEditor_CSVTag::Exit()
{
}

void LevelEditor_CSVTag::End()
{
}
