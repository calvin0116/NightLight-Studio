#pragma once
#include <string>
#include <vector>


class CSVReader
{
	std::string fileName;
	std::string delimeter;

public:
	CSVReader(std::string filename, std::string delm = ",") :
		fileName(filename), delimeter(delm)
	{ }
	// Function to fetch data from a CSV File
	std::vector<std::vector<std::string> > getData();
	void writeData(std::vector<std::vector<std::string>> data);
	void split(std::vector<std::string>& vec, std::string line, char delimitor);
};