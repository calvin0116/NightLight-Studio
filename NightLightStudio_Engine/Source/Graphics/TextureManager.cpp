#include "TextureManager.h"

namespace NS_GRAPHICS
{
	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{

	}

	void TextureManager::AddTexture(unsigned texture, const std::string& texturekey)
	{
		_textureList.insert({ texturekey, texture });
	}

	void TextureManager::Free()
	{
		DumpTextureData();
	}

	void TextureManager::DumpTextureData()
	{
		for (auto& texture : _textureList)
		{
			glDeleteTextures(1, &texture.second);
		}
	}


}