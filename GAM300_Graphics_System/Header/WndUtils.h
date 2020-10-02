/******************************************************************************/
/*!
\par
\file		WndUtils.h

\author     Teh Yu Yin, t.yuyin, 390005118
\par		t.yuyin\@digipen.edu
\date       7 September 2019

\brief      Header for the Window System Utilities

Copyright (C) 2019 DigiPen Institute of Technology. Reproduction or disclosure
of this file or its contents without the prior written consent of DigiPen
Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

//WIN32 INCLUDES
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // We only want the basic functions
#endif

#include <windows.h>

//BASIC UTIL INCLUDES
#include <string>
#include <sstream>
#include <iostream>

// OPENGL INCLUDES
#include "OpenGL/glew.h" // MUST BE INCLUDED FIRST
#include <gl/GL.h>
#include <gl/GLU.h>
#include "OpenGL/glext.h"
#include "OpenGL/wglext.h"

// LINK TO LIBRARIES (JUST IN CASE)
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib")

// Error check for failed initialization
inline bool OutErrorMsg(const char* msg)
{
	MessageBox(NULL, msg, NULL, NULL);
	return false;
}
