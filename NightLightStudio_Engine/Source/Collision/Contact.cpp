#pragma once
#include "Contact.h"



void Manifold::SetPair(OBBCollider* a, OBBCollider* b)
{
	A = a;
	B = b;
}
