/***************************************************************************
Brief Description :
			Json header files and related defines
**************************************************************************/
#pragma once
#include "../../../Include/rapidjson/document.h"     // rapidjson's DOM-style API
#include "../../../Include/rapidjson/prettywriter.h" // for stringify JSON
#include "../../../Include/rapidjson/ostreamwrapper.h"
#include "../../../Include/rapidjson/writer.h"
using namespace rapidjson;

//!!!!== Use for allocator only ==!!!!!//
//static Document global_doc;