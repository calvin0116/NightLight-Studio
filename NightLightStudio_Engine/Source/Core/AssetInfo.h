#pragma once
#include <string>
//Assets class to be inherited
class AssetInfo
{
	private:
		std::string _path;
		bool isLoaded;
	public:
		int _uid;
		
		virtual void LoadData(std::string path)
		{
			path = _path ;
			isLoaded = true;
		};
		
		bool GetIsLoaded()
		{
			return isLoaded;
		};
};
