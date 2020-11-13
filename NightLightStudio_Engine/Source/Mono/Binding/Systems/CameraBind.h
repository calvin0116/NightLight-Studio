#pragma once
#include <mono/metadata/object.h>
//#include "..\..\..\Math\Vector.h"

namespace CameraBind
{
  void BindCamera();

  // Directional Vectors
  // UP vector
  MonoObject* csGetUpVector();
  // RIGHT vector
  //MonoObject* csGetRightVector();
  //// View Vectors
  //// get viewVector
  //MonoObject* csGetViewVector();
  //// get viewVector on XZ plane                     
  //MonoObject* csGetXZViewVector();
  //// get viewVector on XZ plane, rotate @Y 180 deg  
  //MonoObject* csGetXZViewVector_Back();
  //// get viewVector on XZ plane, rotate @Y 90 deg   
  //MonoObject* csGetXZViewVector_Left();
  //// get viewVector on XZ plane, rotate @Y -90 deg  
  //MonoObject* csGetXZViewVector_Right();
}
