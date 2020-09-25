//// Disable prefer enum class over enum warning
//// Warning comes from mono headers
//#pragma warning( disable : 26812 ) 
//
//#include "MonoWrapper.h"
//#include <mono/metadata/assembly.h>
//#include <mono/metadata/debug-helpers.h>
//#include <mono/metadata/mono-gc.h>
//
//namespace Neon
//{
//  namespace MonoWrapper 
//  {
//    MonoImage* currImage = nullptr;
//
//    MonoDomain* InitMono()
//    {
//      //File directory settings
//      mono_set_dirs("Mono\\lib",
//        "Mono\\etc");
//
//      //Set path where mono will look for assemblies
//      //but it dosent work, just specify full path for assembly related stuff
//      //mono_set_assemblies_path("..\\MonoTest");
//
//      MonoDomain* domain = mono_jit_init_version("Root", "v4.0.30319");
//
//      return domain;
//    }
//
//    MonoDomain* LoadScriptDomain()
//    {
//      MonoDomain* newDomain = mono_domain_create_appdomain(
//        const_cast<char*>("Scripts"), NULL);
//
//      //Set current domain to new domain
//      if (!mono_domain_set(newDomain, false))
//        std::cout << "Error setting new domain" << std::endl;
//
//      return mono_domain_get();
//    }
//
//    void UnloadScriptDomain()
//    {
//      //Close image
//      if (currImage != nullptr)
//      {
//        //Commented out for now, weird crash
//        mono_image_close(currImage);
//        currImage = nullptr;
//      }
//
//      //Check if im on the root domain now
//      MonoDomain* currDomain = mono_domain_get();
//      if (currDomain && currDomain != mono_get_root_domain())
//      {
//        //Switch to root domain
//        if (!mono_domain_set(mono_get_root_domain(), false))
//          NEON_PRINT_FATAL("Error setting root domain");
//        
//        //Unload script domain
//        mono_domain_unload(currDomain);
//      }
//      mono_gc_collect(mono_gc_max_generation());
//    }
//
//    bool CompileScripts()
//    {
//      bool bSucceeded = true;
//      static const myString compileCommand =
//        myString(MONO_COMPILER_PATH) + myString(" -recurse:") + myString(SCRIPTS_PATH) +
//        myString("\\*.cs -unsafe -target:library -out:") + myString(DLL_NAME) + 
//        myString(" 2>&1");
//
//      //Compile the scripts
//      myArray<char, 128> buffer;
//      myString result;
//      FILE* compileOutput = _popen(compileCommand.c_str(), "r");
//
//      if (compileOutput != NULL)
//      {
//        PRINT("Compilation output: ");
//        while (fgets(buffer.data(), static_cast<int>(buffer.size()), compileOutput) != nullptr)
//        {
//          /*result += buffer.data();*/
//          PRINT(buffer.data());
//          bSucceeded = false;
//        }
//        PRINT("Compilation completed");
//
//        _pclose(compileOutput);
//      }
//      else
//      {
//        PRINT_WARNING("Script Compilation failed");
//      }
//      return bSucceeded;
//    }
//    
//    MonoImage* ReloadScripts_()
//    {
//      UnloadScriptDomain();
//
//      CompileScripts();
//
//      LoadScriptDomain();
//      
//      OpenDLL();
//
//      return currImage;
//    }
//
//    bool ReloadScripts()
//    {
//      bool compiled;
//      UnloadScriptDomain();
//
//      compiled = CompileScripts();
//
//      LoadScriptDomain();
//
//      OpenDLL();
//
//      return compiled;
//    }
//
//    MonoImage* OpenDLL()
//    {
//      MonoImageOpenStatus status;
//      MonoAssembly* assembly = mono_assembly_open(DLL_NAME, &status);
//      if (status != MONO_IMAGE_OK || assembly == nullptr)
//      {
//        mono_assembly_close(assembly);
//        NEON_PRINT_FATAL("Failed to open assembly");
//        return nullptr;
//      }
//
//      currImage = mono_image_open(DLL_NAME, &status);
//      if (status != MONO_IMAGE_OK || currImage == nullptr)
//      {
//        mono_image_close(currImage);
//        NEON_PRINT_FATAL("Failed to open image");
//        return nullptr;
//      }
//      
//      return currImage;
//    }
//    
//    MonoObject* CloneObject(MonoObject* _obj)
//    {
//      return mono_object_clone(_obj);
//    }
//
//    void MonoExit()
//    {
//      UnloadScriptDomain();
//      mono_jit_cleanup(mono_get_root_domain());
//    }
//
//    void BindClassFunction(void* _unmanagedFnPtr, myString _managedFnName,
//      myString _managedClassName, myString _managedNamespace)
//    {
//      myString managedFnFull = _managedNamespace + myString(".") +
//        _managedClassName + myString("::") + _managedFnName;
//
//      mono_add_internal_call(managedFnFull.c_str(), _unmanagedFnPtr);
//    }
//
//    MonoString* ToMonoString(myString _str)
//    {
//      return mono_string_new(mono_domain_get(), _str.c_str());
//    }
//
//    myString ToMyString(MonoString* _str)
//    {
//      char* temp = mono_string_to_utf8(_str);
//
//      myString result(temp);
//
//      mono_free(temp); // Must free result from mono_string_to_utf8
//
//      return result;
//    }
//
//    MonoClass* GetMonoClass(myString _className, myString _namespace)
//    {
//      return mono_class_from_name(currImage, _namespace.c_str(), _className.c_str());
//    }
//    
//    MonoObject* ConstructObject(myString _className, 
//      myString _namespace)
//    {
//      MonoClass* classType = GetMonoClass(_className, _namespace);
//      MonoObject* obj = mono_object_new(mono_domain_get(), classType);
//      mono_runtime_object_init(obj);
//
//      return obj;
//    }
//
//    uint32_t ConstructGCHandle(MonoObject* _obj)
//    {
//      return mono_gchandle_new(_obj, true);
//    }
//
//    void FreeGCHandle(uint32_t _gchandle)
//    {
//      mono_gchandle_free(_gchandle);
//    }
//
//    /*MonoObject* GetGCHandleObject(uint32_t _gchandle)
//    {
//      return mono_gchandle_get_target(_gchandle);
//    }*/
//
//    void SetNativeHandle(MonoObject* _obj, void* _value)
//    {
//      MonoClass* classType = mono_object_get_class(_obj);
//
//      MonoClassField* field = mono_class_get_field_from_name(classType, 
//        "native_handle");
//
//      mono_field_set_value(_obj, field, &_value);
//    }
//
//    MonoMethod* GetObjectMethod(myString _methodName, myString _className,
//      myString _namespace)
//    {
//      myString fullMethodName = _namespace + "." + _className + ":" + _methodName;
//
//      MonoClass* klass = GetMonoClass(_className, _namespace);
//
//      MonoMethodDesc* method_desc = mono_method_desc_new(fullMethodName.c_str(), true);
//      MonoMethod* method = mono_method_desc_search_in_class(method_desc, klass);
//      mono_method_desc_free(method_desc);
//
//      NEON_ASSERT_MSG(method, "mono_method_desc_search_in_class failed");
//
//      return method;
//    }
//
//    MonoObject* InvokeMethod(MonoMethod* method, MonoObject* obj)
//    {
//      return mono_runtime_invoke(method, obj, nullptr, nullptr);
//    }
//
//    MonoMethod* GetDerivedMethod(MonoObject* obj, MonoMethod* baseMethod)
//    {
//      return mono_object_get_virtual_method(obj, baseMethod);
//    }
//
//    //Not sure if there is a simpler way to do it
//    //Cannot get mono_value_copy(...) to work
//    void CopyObjectVariables(MonoObject* _from, MonoObject* _to)
//    {
//      MonoClass* toklass = mono_object_get_class(_to);
//      MonoClass* fromklass = mono_object_get_class(_from);
//      void* iter = NULL;
//      MonoClassField* field = mono_class_get_fields(toklass, &iter);
//
//      
//      
//      //Loop through all variable
//      while (field)
//      {
//        const char* var_name = mono_field_get_name(field);
//        int var_typeid = mono_type_get_type(mono_field_get_type(field));
//
//        //Check if new variable
//        if (!mono_class_get_field_from_name(fromklass, var_name)) continue;
//
//        switch (var_typeid)
//        {
//        case MONO_TYPE_BOOLEAN: //boolean
//        {
//          bool src = GetObjectFieldValue<bool>(_from, var_name);
//          SetObjectFieldValue(_to, var_name, src);
//          break;
//        }
//        case MONO_TYPE_I4: //int
//        {
//          int src = GetObjectFieldValue<int>(_from, var_name);
//          SetObjectFieldValue(_to, var_name, src);
//          break;
//        }
//        case MONO_TYPE_U4: //unsigned
//        {
//          unsigned src = GetObjectFieldValue<unsigned>(_from, var_name);
//          SetObjectFieldValue(_to, var_name, src);
//          break;
//        }
//        case MONO_TYPE_R4: //float
//        {
//          float src = GetObjectFieldValue<float>(_from, var_name);
//          SetObjectFieldValue(_to, var_name, src);
//          break;
//        }
//        case MONO_TYPE_R8: //double
//        {
//          double src = GetObjectFieldValue<double>(_from, var_name);
//          SetObjectFieldValue(_to, var_name, src);
//          break;
//        }
//        case MONO_TYPE_STRING: //string
//        {
//          //myString src = GetObjectFieldValue<myString>(_from, var_name);
//          //SetObjectFieldValue(_to, var_name, src);
//          break;
//        }
//        default:
//          //Unsupported type
//          break;
//        }
//
//        field = mono_class_get_fields(toklass, &iter);
//      }
//    }
//  }
//}