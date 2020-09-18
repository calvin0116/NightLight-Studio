/***************************************************************************
Brief Description :
			Json header files and related defines
**************************************************************************/
#pragma once
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
using namespace rapidjson;

//!!!!== Use for allocator only ==!!!!!//
static Document global_doc;