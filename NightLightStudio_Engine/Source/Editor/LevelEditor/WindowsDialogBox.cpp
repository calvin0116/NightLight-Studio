/******************************************************************************/
/*!
\file		WindowsDialogBox.cpp
\project	Aris
\author 	Primary: Leong Jun Ming
\par    	email: l.junming\@digipen.edu
\date   	December 03, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "WindowsDialogBox.h"
#include <filesystem>

#ifdef _EDITOR

// Convert wide Unicode String to an UTF8 string
std::string UTF8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}
// Convert an UTF8 string to a wide Unicode String
std::wstring UTF8_decode(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

std::string WindowsOpenFileBox(HWND handle, COMDLG_FILTERSPEC* options, UINT size)
{
#ifdef UNICODE
	std::wstring filename;
#else
	std::string filename;
#endif
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen = NULL;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			pFileOpen->SetFileTypes(size, options);
			// Show the Open dialog box.
			hr = pFileOpen->Show(handle);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
#ifdef UNICODE
						filename = pszFilePath;
#else
						std::wstring temp = pszFilePath;
						filename = UTF8_encode(temp);
#endif
						//MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}

#ifdef UNICODE
	wchar_t szOut[MAX_PATH] = L"";
	wchar_t szFrom[MAX_PATH] =  L"";
	//GetModuleFileName(NULL, szFrom, MAX_PATH);
	if (GetCurrentDirectoryW(MAX_PATH, szFrom))
	{
		if (PathRelativePathToW(szOut, szFrom, FILE_ATTRIBUTE_DIRECTORY, filename.c_str(), FILE_ATTRIBUTE_NORMAL))
		{
			std::wstring toOut = szOut;
			return UTF8_encode(toOut);
		}
	}

	return UTF8_encode(filename);
#else

	std::filesystem::path filePath = filename;
	std::string toReturn = std::filesystem::relative(filePath).string();
	if (toReturn != "")
		return ".\\" + std::filesystem::relative(filePath).string();
	else
		return filename;
	/*
	char szOut[MAX_PATH] = "";
	char szFrom[MAX_PATH] = "";
	if (GetCurrentDirectoryA(MAX_PATH, szFrom))
	{
		if (PathRelativePathToA(szOut, szFrom, FILE_ATTRIBUTE_DIRECTORY, filename.c_str(), FILE_ATTRIBUTE_NORMAL))
		{
			return szOut;
		}
	}
	*/
	//return filename;
#endif
}

std::string WindowsSaveFileBox(HWND handle, COMDLG_FILTERSPEC* options, UINT size)
{
#ifdef UNICODE
	std::wstring filename;
#else
	std::string filename;
#endif
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* pFileSave;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		if (SUCCEEDED(hr))
		{
			pFileSave->SetDefaultExtension(L"");
			pFileSave->SetFileTypes(size, options);
			// Show the Open dialog box.
			hr = pFileSave->Show(handle);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
#ifdef UNICODE
						filename = pszFilePath;
#else
						std::wstring temp = pszFilePath;
						filename = UTF8_encode(temp);
#endif
						//MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}

#if UNICODE
	wchar_t szOut[MAX_PATH] = L"";
	wchar_t szFrom[MAX_PATH] = L"";
	//GetModuleFileName(NULL, szFrom, MAX_PATH);
	if (GetCurrentDirectoryW(MAX_PATH, szFrom))
	{
		if (PathRelativePathToW(szOut, szFrom, FILE_ATTRIBUTE_DIRECTORY, filename.c_str(), FILE_ATTRIBUTE_NORMAL))
		{
			std::wstring toOut = szOut;
			return UTF8_encode(toOut);
		}
	}

	return UTF8_encode(filename);
#else

	std::filesystem::path filePath = filename;
	std::string toReturn = std::filesystem::relative(filePath).string();
	if (toReturn != "")
		return ".\\" + std::filesystem::relative(filePath).string();
	else
		return filename;
	/*
	char szOut[MAX_PATH] = "";
	char szFrom[MAX_PATH] = "";
	if (GetCurrentDirectoryA(MAX_PATH, szFrom))
	{
		if (PathRelativePathToA(szOut, szFrom, FILE_ATTRIBUTE_DIRECTORY, filename.c_str(), FILE_ATTRIBUTE_NORMAL))
		{
			return szOut;
		}
	}
	*/
	// return filename;
#endif
}

#endif