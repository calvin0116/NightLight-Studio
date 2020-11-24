#include "LevelEditor_AssetInsp.h"

#include <iostream>
#include <filesystem>

#include <shellapi.h>
#include "../../Component/Components.h"

#include "../../Core/SceneManager.h"
#include "LevelEditor_ECHelper.h"

void AssetInspector::_RecursiveDirectoryTree(const std::string& path)
{
    const std::vector<std::string>& dir = _allDirDirs[path];
    const std::vector<std::string>& files = _allDirFiles[path];

    // Sets the flag for the tree node
    ImGuiTreeNodeFlags f = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (_selectedFolderPath.find(path) != std::string::npos)
    {
        if (ImGui::GetID(LE_GetFilename(path).c_str()) == 0)
            ImGui::SetNextTreeNodeOpen(true);

        if (_selectedFolderPath.find(path) != std::string::npos)
            f |= ImGuiTreeNodeFlags_Selected;
    }

    _levelEditor->LE_AddTreeNodes(LE_GetFilename(path),
        [this, &path, &dir, &files]()
        {
            if (ImGui::IsItemClicked())
            {
                if (path.size() >= _selectedFolderPath.size())
                {
                    _selectedFolderPath = path;
                }
                else if (_selectedFolderPath != _currentFilePath)
                {
                    _selectedFolderPath = LE_EraseSubStr(path, LE_GetFilename(path));
                    _selectedFilePath = "";
                }
            }

            // For all files functions
            for (int i = 0; i < files.size(); ++i)
            {
                if (_ignoreFileTypes.find(LE_GetFileType(files[i])) != std::end(_ignoreFileTypes))
                    continue;
                _levelEditor->LE_AddSelectable(LE_GetFilename(files[i]), (_selectedFilePath == files[i]),
                    [&]()
                    {
                        // Selects the right folder
                        std::string temp = LE_EraseSubStr(files[i], LE_GetFilename(files[i]));
                        temp.erase(std::end(temp) - 1);

                        _selectedFolderPath = temp;
                        _selectedFilePath = files[i];

                        if (ImGui::IsMouseDoubleClicked(0))
                        {
#ifdef UNICODE
                            // Double click on Left inspector
                            std::wstring wsTmp(_selectedFilePath.begin(), _selectedFilePath.end());
                            ShellExecute(NULL, L"open", wsTmp.c_str(), NULL, NULL, SW_SHOW);
#else
                            ShellExecute(NULL, "open", _selectedFilePath.c_str(), NULL, NULL, SW_SHOW);
#endif
                        }
                    }, ImGuiSelectableFlags_AllowDoubleClick);

                // On first click 
                if (ImGui::IsItemClicked(0))
                {
                    // Get relative path
                    _dragDropFilePath = LE_EraseSubStr(files[i], _currentFilePath);
                    _selectedFilePath = files[i];
                    _setScroll = true;
                }

                // Gets Relative path from executable
                _levelEditor->LE_AddDragDropSource("ASSET_FILEPATH", &_dragDropFilePath,
                    [this]() { _levelEditor->LE_AddText(_dragDropFilePath); });
            }

            // For all folders. add recursive function to functions
            for (int i = 0; i < dir.size(); ++i)
            {
                ImGui::Separator();
                _RecursiveDirectoryTree(dir[i]);
            }
        }, f);

}

void AssetInspector::_RefreshDirectories(const std::string& path)
{
    _allDirFiles[path] = LE_GetFilesInDir(path);
    _allDirDirs[path] = LE_GetDirectories(path);

    for (int i = 0; i < _allDirDirs[path].size(); ++i)
    {
        _RefreshDirectories(_allDirDirs[path][i]);
    }
}

AssetInspector::AssetInspector()
    : _allDirFiles{}, _allDirDirs{},
    _ignoreFileTypes{},
    _currentFilePath{ std::filesystem::current_path().string() },
    _selectedFolderPath{ std::filesystem::current_path().string() },
    _selectedFilePath{},
    _dragDropFilePath{},
    _setScroll{ false }
{
    _RefreshDirectories(_currentFilePath);
}

AssetInspector::~AssetInspector()
{
}

void AssetInspector::Init()
{
    ImGui::SetNextWindowBgAlpha(1.0f);
    ImGui::SetWindowSize(ImVec2(640, 320), ImGuiCond_FirstUseEver);
    //_ignoreFileTypes.insert("cpp");
    //_ignoreFileTypes.insert("filters");
    //_ignoreFileTypes.insert("user");
}

void AssetInspector::Run()
{
    ImGui::Columns(2);
    //std::vector<std::string> dir = get_directories(std::filesystem::current_path().string());
    //std::vector<std::string> dir = _GetFilesInDir("../");

    _levelEditor->LE_AddChildWindow("FolderTree", ImVec2(0, 0),
        [this]()
        {
            _RecursiveDirectoryTree(_currentFilePath);
        });
    ImGui::NextColumn();

    _levelEditor->LE_AddInputText("Search##SearchFilePath", _searchPath, 256, 0);
    // lower-case everything
    std::transform(std::begin(_searchPath), std::end(_searchPath), std::begin(_searchPath), [](char u) {return (char)std::tolower(u); });

    _levelEditor->LE_AddChildWindow("FolderView", ImVec2(0, 0),
        [this]()
        {
            _levelEditor->LE_AddButton("Back##AssetInspector",
                [&]()
                {
                    if (_selectedFolderPath.size() > _currentFilePath.size())
                    {
                        _selectedFolderPath = LE_EraseSubStr(_selectedFolderPath, LE_GetFilename(_selectedFolderPath));
                        while (*(std::end(_selectedFolderPath) - 1) == '\\')
                            _selectedFolderPath.pop_back();
                    }
                });

            ImGui::SameLine();

            _levelEditor->LE_AddButton("Refresh Inspector##AssetInspector",
                [this]()
                {
                    _allDirFiles.clear();
                    _allDirDirs.clear();
                    _currentFilePath = std::filesystem::current_path().string();
                    _RefreshDirectories(_currentFilePath);
                });

            ImVec2 size(100.0f, 100.0f);
            //float offset = 50.0f;

            float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

            std::vector<std::string> files = _allDirDirs[_selectedFolderPath];
            int filesStart = (int)files.size();
            const std::vector<std::string>& temp = _allDirFiles[_selectedFolderPath];
            files.insert(std::end(files), std::begin(temp), std::end(temp));

            for (int i = 0; i < files.size(); ++i)
            {
                {
                    if (_searchPath != "")
                    {
                        std::string lowerCase = files[i];
                        std::transform(std::begin(lowerCase), std::end(lowerCase), std::begin(lowerCase), [](char u) {return (char)std::tolower(u); });
                        if (lowerCase.rfind(_searchPath) == std::string::npos)
                            continue;
                    }
                }
                ImGui::PushID(i);
                ImGui::BeginGroup(); // Lock X position

                std::string name = "##empty";
                name.append(std::to_string(i));

                _levelEditor->LE_AddSelectable(name, (_selectedFilePath == files[i]), [this, &files, &filesStart, &i]()
                    {
                        _selectedFilePath = files[i];
                        // Run if double click
                        if (ImGui::IsMouseDoubleClicked(0))
                        {
                            if (i >= filesStart)
                            {
#ifdef UNICODE
                                // Double click on Left inspector
                                std::wstring wsTmp(_selectedFilePath.begin(), _selectedFilePath.end());
                                ShellExecute(NULL, L"open", wsTmp.c_str(), NULL, NULL, SW_SHOW);
#else
                                ShellExecute(NULL, "open", _selectedFilePath.c_str(), NULL, NULL, SW_SHOW);
#endif
                            }
                            else
                            {
                                _selectedFolderPath = files[i];
                                //ImGui::GetStateStorage()->SetInt(ImGui::GetID(_selectedFolderPath.c_str()), true);
                            }
                        }
                    }, ImGuiSelectableFlags_AllowDoubleClick, size);

                if (ImGui::IsItemVisible())
                {
                    if (i >= filesStart && ImGui::IsItemClicked(0))
                    {
                        
                        _selectedFilePath = files[i];
                        _dragDropFilePath = LE_EraseSubStr(files[i], _currentFilePath);

                        std::string fileType = LE_GetFileType(files[i]);
                        //For prefab seletion and showcase
                        if (fileType == "prefab")
                        {
                            LE_ECHELPER->SelectPrefab(files[i]);
                            //LE_ECHELPER->prefab_inst.isActive = true;
                        }
                    }

                    const ImVec2 p0 = ImGui::GetItemRectMin();
                    const ImVec2 p1 = ImGui::GetItemRectMax();
                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    ImGuiIO& io = ImGui::GetIO();
                    //ImTextureID my_tex_id = io.Fonts->TexID;

                    //unsigned val = 1;

                    draw_list->PushClipRect(p0, p1, true);
                    if (i < filesStart)
                        // Folder image goes here
                        //draw_list->AddImage((void*)(intptr_t)val, { p0.x + 20.0f, p0.y - 40.0f }, { p1.x - 20.0f, p1.y - 40.0f }, ImVec2(0,0), ImVec2(1,1), ImColor(252,186,3));
                        draw_list->AddRectFilled({ p0.x + 20.0f, p0.y }, { p1.x - 20.0f, p1.y - 40.0f }, ImColor(255, 252, 54));
                    else
                    {
                        // File image goes here
                        //draw_list->AddImage((void*)(intptr_t)val, { p0.x + 20.0f, p0.y - 40.0f }, { p1.x - 20.0f, p1.y - 40.0f });
                        std::string fileType = LE_GetFileType(files[i]);
                        std::transform(fileType.begin(), fileType.end(), fileType.begin(),
                            [](unsigned char c)
                            { return (char)std::tolower(c); });
                        if (fileType == "tga" || fileType == "png" || fileType == "jpeg")
                        {
                            draw_list->AddRectFilled({ p0.x + 20.0f, p0.y }, { p1.x - 20.0f, p1.y - 40.0f }, ImColor(0, 0, 255));
                        }
                        else if (fileType == "json")
                        {
                            draw_list->AddRectFilled({ p0.x + 20.0f, p0.y }, { p1.x - 20.0f, p1.y - 40.0f }, ImColor(0, 255, 0));
                        }
                        else if (fileType == "fbx")
                        {
                            draw_list->AddRectFilled({ p0.x + 20.0f, p0.y }, { p1.x - 20.0f, p1.y - 40.0f }, ImColor(225, 0, 255));
                        }
                        else
                        {
                            draw_list->AddRectFilled({ p0.x + 20.0f, p0.y }, { p1.x - 20.0f, p1.y - 40.0f }, ImColor(111, 111, 111));
                        }
                    }
                    //draw_list->AddText({ p0.x, p0.y + 90.0f }, IM_COL32_WHITE, _GetFilename(files[i]).c_str()); // Filename goes here
                    draw_list->AddText(io.Fonts->Fonts[0], 13.0f, { p0.x, p0.y + 65.0f }, IM_COL32_WHITE, LE_GetFilename(files[i]).c_str(), nullptr, size.x + 10.0f);
                    draw_list->PopClipRect();

                    // Crashes when item is off-screen for some reason? Set to not visible for optimization and prevent crashes above
                    if (i >= filesStart)
                    {
                        // Gets Relative path from executable
                        _levelEditor->LE_AddDragDropSource("ASSET_FILEPATH", &_dragDropFilePath,
                            [&]()
                            {
                                _levelEditor->LE_AddText(_dragDropFilePath);
                            });
                    }
                }


                ImGui::EndGroup();

                ImGuiStyle& style = ImGui::GetStyle();
                float last_button_x2 = ImGui::GetItemRectMax().x;
                float next_button_x2 = last_button_x2 + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
                if ((i + 1) < files.size() && next_button_x2 < window_visible_x2)
                    ImGui::SameLine();

                if (_setScroll && (_selectedFilePath == files[i]))
                {
                    _setScroll = false;
                    ImGui::SetScrollHereY();
                }


                ImGui::PopID();
                std::string relativePath = LE_EraseSubStr(files[i], _currentFilePath);
                _levelEditor->LE_AddTooltip(relativePath);
            }
        }, true);

        // Entity of Object
        _levelEditor->LE_AddDragDropTarget<Entity>("HIERARCHY_ENTITY_OBJECT",
            [this](Entity* entptr)
            {
                Entity ent = *entptr;
                //std::cout << ent.getId() << std::endl;
                //std::cout << _selectedFolderPath << std::endl;

                PFunc::WritePrefab(_selectedFolderPath, ent);
                //Copied from your refresh inspector to refresh
                {
                    _allDirFiles.clear();
                    _allDirDirs.clear();
                    _currentFilePath = std::filesystem::current_path().string();
                    _RefreshDirectories(_currentFilePath);
                }
            });
}

void AssetInspector::Exit()
{
}