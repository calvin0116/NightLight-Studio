/******************************************************************************/
/*!
\file		WindowsDialogBox.h
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

#ifndef _WINDOWS_DIALOG_BOX_CUST
#define _WINDOWS_DIALOG_BOX_CUST

#include <windows.h>
#include <shobjidl.h> 
#include <string>
#include "../../../framework.h"

#ifdef _EDITOR

// Converts Wide String to String
std::string UTF8_encode(const std::wstring& wstr);

// Convert an UTF8 string to a wide Unicode String
std::wstring UTF8_decode(const std::string& str);

std::string WindowsOpenFileBox(HWND handle, COMDLG_FILTERSPEC* options, UINT size);
std::string WindowsSaveFileBox(HWND handle, COMDLG_FILTERSPEC* options, UINT size);

#endif

#endif
