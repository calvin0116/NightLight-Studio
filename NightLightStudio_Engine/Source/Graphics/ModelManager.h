#pragma once

#include "../../framework.h"
#include "Model.h"
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	// Manages OpenGL Object IDs A.K.A Model "keys" for all graphics components
	// DOES NOT CONTAIN MODELS THEMSELVES
	class ModelManager
	{
		ModelManager();
		~ModelManager();

	public:
		// Unique Singleton instance
		static ModelManager& GetInstance()
		{
			static ModelManager instance;
			return instance;
		}

		std::vector<GLuint> VAOs;			// Vertex Attribute Objects, GPU layout instructions
		std::vector<GLuint> VBOs;			// Vertex Buffer Objects, vertex position/RGBA/UV ID
		std::vector<GLuint> EBOs;			// Element Buffer Objects, indices ID
		std::vector<GLuint> ModelMatrixBOs; // Vertex Buffer objects, model matrix ID

		// Free all existing OpenGL Objects, if any
		// This should be called upon graphics system shutdown
		void FreeGLObjects();
	};
}