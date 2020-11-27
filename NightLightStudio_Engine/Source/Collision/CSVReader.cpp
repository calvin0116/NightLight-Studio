#pragma once
#include <fstream>
#include <algorithm>
#include "CSVReader.h"
#include <sstream>


std::vector<std::vector<std::string> > CSVReader::getData()
{
	std::ifstream file(fileName);

	std::vector<std::vector<std::string> > dataList;

	std::string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		split(vec, line, ',');
		dataList.push_back(vec);
	}
	// Close the File
	file.close();

	return dataList;
}

void CSVReader::writeData(std::vector<std::vector<std::string>> data)
{
	// Create an output filestream object
	std::ofstream myFile(fileName);
	if (myFile.is_open())
	{
		// Send column names to the stream
		for (int i = 0; i < data.size(); ++i)
		{
			for (int j = 0; j < data[i].size(); ++j)
			{
				myFile << data[i][j];
				if (j < data[i].size() - 1)
				{
					myFile << ',';
				}
			}
			myFile << "\n";
		}
	}

	// Close the file
	myFile.close();
}

void CSVReader::split(std::vector<std::string>& vec, std::string line, char delimiter)
{
	std::string token;
	std::istringstream tokenStream(line);
	while (std::getline(tokenStream, token, delimiter))
	{
		vec.push_back(token);
	}
}