#include "LevelEditor_AssetInsp.h"

#include <iostream>
#include <filesystem>

#include <shellapi.h>

std::vector<std::string> AssetInspector::_GetAllDirectories(const std::string& path)
{
    if (path.empty())
        return {};
    std::vector<std::string> r;
    for (auto& p : std::filesystem::recursive_directory_iterator(path))
        if (p.is_directory())
            r.push_back(p.path().string());
    return r;
}

std::vector<std::string> AssetInspector::_GetDirectories(const std::string& path)
{
    if (path.empty())
        return {};
    std::vector<std::string> r;
    for (auto& p : std::filesystem::directory_iterator(path))
        if (p.is_directory())
            r.push_back(p.path().string());
    return r;
}

std::vector<std::string> AssetInspector::_GetFilesInDir(const std::string& path)
{
    if (path.empty())
        return {};
    std::vector<std::string> r;
    for (const auto& p : std::filesystem::directory_iterator(path))
        if (!p.is_directory())
            r.push_back(p.path().string());
    return r;
}

std::string AssetInspector::_EraseSubStr(const std::string& str, const std::string& toErase)
{
    std::string mainStr = str;
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }

    return mainStr;
}

std::string AssetInspector::_GetFilename(const std::string& path)
{
    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = path.rfind(sep, path.length());
    if (i != std::string::npos) {
        return(path.substr(i + 1, path.length() - i));
    }

    return {};
}

std::string AssetInspector::_GetFileType(const std::string& path)
{
    char sep = '.';
    size_t i = path.rfind(sep, path.length());
    if (i != std::string::npos) {
        return(path.substr(i + 1, path.length() - i));
    }

    return {};
}

void AssetInspector::_RecursiveDirectoryTree(const std::string& path)
{
    std::vector<std::string> dir = _GetDirectories(path);
    std::vector<std::string> files = _GetFilesInDir(path);

    // List of functions to run inside the tree node
    std::vector<std::function<void()>> fns =
    {
        std::bind(
        [&](std::string p)
        {
            if (ImGui::IsItemClicked())
            {
                if (p.size() >= _selectedFolderPath.size())
                {
                    _selectedFolderPath = p;
                }
                else if (_selectedFolderPath != std::filesystem::current_path().string())
                {
                    _selectedFolderPath = _EraseSubStr(p, _GetFilename(p));
                    _selectedFilePath = "";
                }
            }
        }, path)
    };

    // For all files functions
    for (int i = 0; i < files.size(); ++i)
    {
        if (_ignoreFileTypes.find(_GetFileType(files[i])) != std::end(_ignoreFileTypes))
            continue;
        fns.push_back(std::bind(
            [&](std::string p)
            {
                _levelEditor->LE_AddSelectable(_GetFilename(p).c_str(), (_selectedFilePath == p),
                    [&]()
                    {
                        // Selects the right folder
                        std::string temp = _EraseSubStr(p, _GetFilename(p));
                        temp.erase(std::end(temp) - 1);

                        _selectedFolderPath = temp;
                        _selectedFilePath = p;

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
                    _dragDropFilePath = _EraseSubStr(p, std::filesystem::current_path().string());
                    _selectedFilePath = p;
                }

                // Gets Relative path from executable
                _levelEditor->LE_AddDragDropSource("ASSET_FILEPATH", &_dragDropFilePath,
                    {
                        [&]() {ImGui::Text(_dragDropFilePath.c_str()); }
                    });
            },
            files[i]
                ));
        std::string test = _GetFileType(files[i]);
    }

    // For all folders. add recursive function to functions
    for (int i = 0; i < dir.size(); ++i)
    {
        fns.push_back(std::bind(
            [&](std::string p)
            {
                ImGui::Separator();
                _RecursiveDirectoryTree(p);
            },
            dir[i]
                ));
    }

    // Sets the flag for the tree node
    ImGuiTreeNodeFlags f = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (_selectedFolderPath.find(path) != std::string::npos)
    {
        if (ImGui::GetID(_GetFilename(path).c_str()) == 0)
            ImGui::SetNextTreeNodeOpen(true);

        if (_selectedFolderPath.find(path) != std::string::npos)
            f |= ImGuiTreeNodeFlags_Selected;
    }

    if (std::filesystem::current_path().string() == path)
    {
        f |= ImGuiTreeNodeFlags_DefaultOpen;
    }

    _levelEditor->LE_AddTreeNodes(_GetFilename(path), fns, f);

}

AssetInspector::AssetInspector()
    : _ignoreFileTypes{},
    _selectedFolderPath{ std::filesystem::current_path().string() },
    _selectedFilePath{},
    _dragDropFilePath{}
{
}

AssetInspector::~AssetInspector()
{
}

void AssetInspector::Init()
{
    ImGui::SetNextWindowBgAlpha(1.0f);

    //_ignoreFileTypes.insert("cpp");
    //_ignoreFileTypes.insert("filters");
    //_ignoreFileTypes.insert("user");
}

void AssetInspector::Run()
{
    ImGui::Columns(2);


    //std::vector<std::string> dir = get_directories(std::filesystem::current_path().string());
    //std::vector<std::string> dir = _GetFilesInDir("../");

    ImVec2 winSize = ImGui::GetWindowSize();
    _levelEditor->LE_AddChildWindow("FolderTree", ImVec2(0, 0),
        {
            [&]()
            {
                _RecursiveDirectoryTree(std::filesystem::current_path().string());
            }
        });
    ImGui::NextColumn();

    _levelEditor->LE_AddChildWindow("FolderView", ImVec2(0, 0),
        {
            [&]()
            {
                _levelEditor->LE_AddButton("Back##AssetInspector",
                    [&]()
                    {
                        if (_selectedFolderPath.size() > std::filesystem::current_path().string().size())
                        {
                            _selectedFolderPath = _EraseSubStr(_selectedFolderPath, _GetFilename(_selectedFolderPath));
                            while (*(std::end(_selectedFolderPath) - 1) == '\\')
                                _selectedFolderPath.pop_back();
                        }
                    });

                ImVec2 size(100.0f, 100.0f);
                //float offset = 50.0f;

                float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

                std::vector<std::string> files = _GetDirectories(_selectedFolderPath);
                int filesStart = (int)files.size();
                std::vector<std::string> temp = _GetFilesInDir(_selectedFolderPath);
                files.insert(std::end(files), std::begin(temp), std::end(temp));

                for (int i = 0; i < files.size(); ++i)
                {
                    ImGui::PushID(i);
                    ImGui::BeginGroup(); // Lock X position

                    std::string name = "##empty";
                    name.append(std::to_string(i));

                    _levelEditor->LE_AddSelectable(name.c_str(), (_selectedFilePath == files[i]), [&]()
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

                    if (i >= filesStart && ImGui::IsItemClicked(0))
                    {
                        _selectedFilePath = files[i];
                        _dragDropFilePath = _EraseSubStr(files[i], std::filesystem::current_path().string());
                    }

                    const ImVec2 p0 = ImGui::GetItemRectMin();
                    const ImVec2 p1 = ImGui::GetItemRectMax();
                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    ImGuiIO& io = ImGui::GetIO();
                    //ImTextureID my_tex_id = io.Fonts->TexID;

                    draw_list->PushClipRect(p0, p1, true);
                    if (i < filesStart)
                        // Folder image goes here
                        //draw_list->AddImage(0, { p0.x + 20.0f, p0.y - 40.0f }, { p1.x - 20.0f, p1.y - 40.0f }, ImVec2(0,0), ImVec2(1,1), ImColor(252,186,3));
                        draw_list->AddRectFilled({ p0.x + 20.0f, p0.y }, { p1.x - 20.0f, p1.y - 40.0f }, ImColor(255, 252, 54));
                    else
                        // File image goes here
                        //draw_list->AddImage(0, { p0.x + 20.0f, p0.y - 40.0f }, { p1.x - 20.0f, p1.y - 40.0f });
                        draw_list->AddRectFilled({ p0.x + 20.0f, p0.y }, { p1.x - 20.0f, p1.y - 40.0f }, ImColor(111, 111, 111));
                    //draw_list->AddText({ p0.x, p0.y + 90.0f }, IM_COL32_WHITE, _GetFilename(files[i]).c_str()); // Filename goes here
                    draw_list->AddText(io.Fonts->Fonts[0], 13.0f, { p0.x, p0.y + 65.0f }, IM_COL32_WHITE, _GetFilename(files[i]).c_str(), nullptr, size.x + 10.0f);
                    draw_list->PopClipRect();

                    // Crashes when item is off-screen for some reason?
                    if (ImGui::IsItemVisible())
                    {
                        if (i >= filesStart)
                        {
                            // Gets Relative path from executable
                            _levelEditor->LE_AddDragDropSource("ASSET_FILEPATH", &_dragDropFilePath,
                                {
                                    [&]()
                                    {
                                        ImGui::Text(_dragDropFilePath.c_str());
                                    }
                                });
                        }
                    }

                    ImGui::EndGroup();

                    ImGuiStyle& style = ImGui::GetStyle();
                    float last_button_x2 = ImGui::GetItemRectMax().x;
                    float next_button_x2 = last_button_x2 + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
                    if ((i + 1) < files.size() && next_button_x2 < window_visible_x2)
                        ImGui::SameLine();


                    ImGui::PopID();
                    std::string relativePath = _EraseSubStr(files[i], std::filesystem::current_path().string());
                    _levelEditor->LE_AddTooltip(relativePath.c_str());
                }
            }
        }
    , true);
}

void AssetInspector::Exit()
{
}