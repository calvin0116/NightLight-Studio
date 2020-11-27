#include "TextureManager.h"
#include "TextureLoader.h"

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

	int TextureManager::GetTexture(const std::string& texturekey)
	{
		if (_textureList.find(texturekey) == _textureList.end())
		{
			//This means if loading fails
			if (TextureLoader::GetInstance().LoadTexture(texturekey) == 0)
			{
				return 0;
			}
		}

		return _textureList[texturekey];
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

	void TextureManager::BindAlbedoTexture(const unsigned& id)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void TextureManager::BindMetallicTexture(const unsigned& id)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void TextureManager::BindRoughnessTexture(const unsigned& id)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void TextureManager::BindAmbientOcclusionTexture(const unsigned& id)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	/*void TextureManager::BindDiffuseTexture(const unsigned& id)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
	}*/

	/*void TextureManager::BindSpecularTexture(const unsigned& id)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, id);
	}*/
}