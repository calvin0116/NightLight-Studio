#pragma once

#include "../../framework.h"
#include "../Window/WndUtils.h"
#include <unordered_map>

namespace NS_GRAPHICS
{
	// Manages OpenGL Object IDs A.K.A Model "keys" for all graphics components
	// DOES NOT CONTAIN MODELS THEMSELVES
	class TextureManager
	{
		TextureManager();
		~TextureManager();

		// Texture IDs
		// Loaded texture
		std::unordered_map<std::string, unsigned> _textureList;

	public:
		// Unique Singleton instance
		static TextureManager& GetInstance()
		{
			static TextureManager instance;
			return instance;
		}

		// Adds loaded texture into master list, requires name as key and context
		// E.g.
		void AddTexture(unsigned texture, const std::string& texturekey);
		int GetTexture(const std::string& texturekey);

		void Free();

		// Free all existing OpenGL Objects and removes texture from memory, if any
		// This should be called upon graphics system shutdown
		void DumpTextureData();

		// Bind textures to uniform in shader
		void BindDiffuseTexture(const unsigned& id);

		void BindSpecularTexture(const unsigned& id);
	};
}