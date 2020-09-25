#pragma once
#include <mono/jit/jit.h>
//
//#define MONO_COMPILER_PATH "Mono\\bin\\mcs"
//#define SCRIPTS_PATH "Assets\\Scripts"
//#define DLL_NAME "NLSGame.dll"
//
//namespace Neon
//{
//  namespace MonoWrapper
//  {
//    extern MonoImage* currImage;
//
//    //Initialization / Exit
//    MonoDomain* InitMono();
//    MonoDomain* LoadScriptDomain();
//    void        UnloadScriptDomain();
//    MonoImage*  OpenDLL();
//    void        MonoExit();
//
//
//
//    //Script compilations
//    bool       CompileScripts();
//    MonoImage* ReloadScripts_();
//    bool       ReloadScripts();
//
//
//
//    //Binding
//    void BindClassFunction(void* _unmanagedFnPtr, myString _managedFnName,
//      myString _managedClassName, myString _managedNamespace = "Neon");
//
//    //void BindStaticFunction(void* _unmanagedFn, myString _managedFnName,
//    // myString _managedNamespace);
//
//
//
//    //Objects
//    MonoClass*  GetMonoClass(myString _className, myString _namespace = "Neon");
//    MonoObject* ConstructObject(myString _className, myString _namespace = "Neon");
//    uint32_t    ConstructGCHandle(MonoObject* _obj);
//    void        FreeGCHandle(uint32_t _gchandle);
//    //MonoObject* GetGCHandleObject(uint32_t _gchandle);
//    MonoObject* CloneObject(MonoObject* _obj);
//    void        SetNativeHandle(MonoObject* _obj, void* _value);  
//
//
//
//    //Strings
//    MonoString* ToMonoString(myString _str);
//    myString    ToMyString(MonoString* _str);
//
//
//
//    //Methods / Functions
//    MonoMethod* GetObjectMethod(myString _methodName, myString _className,
//      myString _namespace = "Neon");
//    MonoMethod* GetDerivedMethod(MonoObject* obj, MonoMethod* baseMethod);
//    MonoObject* InvokeMethod(MonoMethod* method, MonoObject* obj);
//
//    //Pass NULL into obj for static method
//    template<typename... TArgs>
//    MonoObject* InvokeMethodParams(MonoMethod* method, MonoObject* obj, TArgs... param)
//    {
//      void* args[sizeof...(TArgs)] = { (&param)... };
//      return mono_runtime_invoke(method, obj, args, nullptr);
//    }    
//
//    //Object Field Values / Variable values
//    template<typename T>
//    void SetObjectFieldValue(MonoObject* _obj, const myString& _fieldName,
//      T& _fieldValue)
//    {
//      MonoClass* classType = mono_object_get_class(_obj);
//
//      MonoClassField* field = mono_class_get_field_from_name(classType, 
//        _fieldName.c_str());
//
//      mono_field_set_value(_obj, field, &_fieldValue);
//    }
//
//    //Copy monoobject (all it's variable)
//    void CopyObjectVariables(MonoObject* _from, MonoObject* _to);
//
//    template<typename T>
//    T GetObjectFieldValue(MonoObject* _obj, const myString& _fieldName)
//    {
//      MonoClass* classType = mono_object_get_class(_obj);
//
//      MonoClassField* field = mono_class_get_field_from_name(classType,
//        _fieldName.c_str());
//
//      T val;
//
//      mono_field_get_value(_obj, field, &val);
//
//      return val;
//    }
//  }
//}