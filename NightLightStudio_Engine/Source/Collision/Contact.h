#pragma once
#include "../Math/Vector.h"
#include "../Component/ComponentCollider.h"

struct OBBCollider;

// in stands for "incoming"
// out stands for "outgoing"
// I stands for "incident"
// R stands for "reference"
// See D. Gregorius GDC 2015 on creating contacts for more details
// Each feature pair is used to cache solutions from one physics tick to another. This is
// called warmstarting, and lets boxes stack and stay stable. Feature pairs identify points
// of contact over multiple physics ticks. Each feature pair is the junction of an incoming
// feature and an outgoing feature, usually a result of clipping routines. The exact info
// stored in the feature pair can be arbitrary as long as the result is a unique ID for a
// given intersecting configuration.
union FeaturePair
{
	struct
	{
		unsigned char inR;
		unsigned char outR;
		unsigned char inI;
		unsigned char outI;
	};

	unsigned int key;
};

struct Contact
{
	NlMath::Vec3 position;							// World coordinate of contact
	float penetration;									// Depth of penetration from collision
	float normalImpulse;							// Accumulated normal impulse
	float tangentImpulse[2];						// Accumulated friction impulse
	float bias;												// Restitution + baumgarte
	float normalMass;									// Normal constraint mass
	float tangentMass[2];							// Tangent constraint mass
	FeaturePair fp;										// Features on A and B for this contact
	unsigned char warmStarted;				// Used for debug rendering
};

struct Manifold
{

	OBBCollider* A;
	OBBCollider* B;

	NlMath::Vec3 normal;									// From A to B
	NlMath::Vec3 tangentVectors[2];				// Tangent vectors
	Contact contacts[8];
	unsigned int contactCount;

	Manifold* next;
	Manifold* prev;
};

struct ClipVertex
{
	ClipVertex()
	{
		f.key = ~0;
	}

	NlMath::Vec3 v;
	FeaturePair f;
};