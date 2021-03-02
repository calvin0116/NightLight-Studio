#include "AssetInfo.h"
#include "Singleton.h"
#include "MySystem.h"

#include <map>
#include <functional>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class AssetsManager : public Singleton<AssetsManager>, public MySystem
{
	private:
		enum FILETYPE
		{
			MAIN, 		// Get from function -> C:\Users\crystalwei\Documents\GitHub
			ASSET,		//-> \Asset
			SCENE,		//-> \Scene
			TEXTURE,		//-> \Art
			JSON,		//-> \Json
			PREFABS,	//-> \Prefabs
			SOUND,		//-> \Sounds
			NONE
		};
		//
		class FileSystem
		{
		private:
			//=========================
			//std::string MainFilePath; 	
			//std::string AssetsFilePath;
			//=>
			std::map<FILETYPE, std::string> PathMap;

		public:
			FileSystem()
			{

			}

			std::string& operator [] (FILETYPE ft)
			{
				return PathMap[ft];
			}
			
			//Get and store pointer so that any changes here will be reflected on other class using this
			std::string* GetPathPtr(FILETYPE ft)
			{
				return &PathMap[ft];
			}
			//Address version
			std::string& GetPathRef(FILETYPE ft)
			{
				return PathMap[ft];
			}

			std::string GetFullPath(FILETYPE ft)
			{
				return PathMap[MAIN] + PathMap[ft];
			}

		};
		FileSystem fileSystem;
		
		//Assets container
		template<typename T, FILETYPE FileType = MAIN>
		class MyAssets : public Singleton<T>
		{
			int _uid_count = 0;
			std::map<std::string, T> assets_map; 
			
		public:
			//Create Empty Assets
			//return id and Asset
			std::pair<std::string,T>& CreateAsset(std::string filename)
			{
				//Find for existing assets
				for(std::pair<std::string,T>& asset : assets_map)
				{
					if(asset.first == filename)
						return asset;
				}
				//Create new assets
				std::pair<std::string,T>& asset = assets_map.push_back(std::make_pair<std::string,T>(filename, T())); //Can change to using memory man
				AssetInfo* asset_info = asset.second;
				asset_info._uid = _uid_count;
				++_uid_count;
				
				return assets_map.back();
			}
			
			void LoadAssets()
			{
				//StartThread	-> 	copy of assets_map
				//Load Asset
				for(std::pair<std::string,T>& asset : assets_map)
				{
					AssetInfo* asset_info = asset.second;
					if(asset_info != nullptr && !asset_info.GetIsLoaded())
					{	
						std::string fullpath = fileSystem.GetPathRef(FileType) + asset.first;
						asset_info->LoadData(fullpath);
					}
					//Merge thread 	-> merges new asserts_map with old assets_map
					//				-> update progress
				}
			}
			
			T& GetAsset(std::string filename)
			{
				return assets_map[filename];
			}
			
			T& operator [] (std::string filename)
			{
				return assets_map[filename];
			}
		};
		
		std::string currentAssetsLoading;
		float loadingProgress;				//->100%
		
		std::vector<std::function<void(void)>> loadfuncs; 
		
		//Load All Requested Asset
		void LoadAsset()
		{
			//for each container, load their asset
			for (auto load : loadfuncs)
			{
				load();
			}
			//TracyMessageL()
		}
	public:
	
	//========== Manager Function ======================//
		//AssertHolder<Audio> audio_asset;
		//AssertHolder<Json> json_asset;
		//=>
		//Function for controling first creation
		template<typename T, typename Index = std::string>
		void AddType()
		{
			MyAssets<T>::GetInstance();	//Create class instance
			loadfuncs.push_back(MyAssets<T>::LoadAssets);	
		}
		
		//Create Asset Request
		//e.g. CreateAsset<SoundAsset>(food step)
		template<typename T>
		std::map<std::string,T>& CreateAsset(std::string filename)
		{
			return MyAssets<T>::GetInstance()->CreateAsset(filename);		
		}
		

		
		//For ease of keeping track assets on this side
		template<typename T>
		T& GetAsset(std::string filename) 
		{
			MyAssets<T>* as_map = MyAssets<T>::GetInstance();
			return as_map[filename];
		};
		
	//===============Inherited Function======================//
		void EarlyLoad()
		{
			fileSystem[MAIN] = fs::current_path().string() + "\\NightLightStudio_Game";
			fileSystem[ASSET] = "\\Asset";
			fileSystem[TEXTURE] = fileSystem[ASSET]+"\\Textures";
			fileSystem[JSON] = +"\\JsonFile";
			fileSystem[PREFABS]	= fileSystem[JSON]+"\\Prefab";
			fileSystem[SCENE] = fileSystem[JSON] + "\\Scene";
			fileSystem[SOUND] = "\\Sounds";

		}

		void Load() override
		{
			LoadAsset(); //<- can send to another thread to load
		}

		void Exit() override
		{
			DestroyInstance();
		}
};

static AssetsManager* SYS_ASSETSMANAGER = AssetsManager::GetInstance();