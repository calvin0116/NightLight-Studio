#include "TextureLoader.h"

namespace NS_GRAPHICS
{
	TextureLoader::TextureLoader()
	{

	}

	TextureLoader::~TextureLoader()
	{

	}

	bool TextureLoader::LoadDDSImage(const std::string& file)
	{
		std::cout << "Loading DDS Images " << file << std::endl;
		DDSImageData* image;

		image = LoadDDSImageData(file);

		//If it loads successful
		if (image != NULL)
		{
			int width = image->_width;
			int height = image->_height;
			int mipMaps = image->_numMipMaps;
			int size;
			int offset = 0;
			int i;
			int blockSize;

			if (image->_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
				blockSize = 8;
			else
				blockSize = 16;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			unsigned int imageSize = (((width >> 2) + 3) & ~3) * (((height >> 2) + 3) & ~3) * blockSize;
			if (image->_pixels != NULL)
			{
				glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, image->_format,
					width, height, 0,
					imageSize,
					image->_pixels);
			}

			//Creates the mipmaps into 1 texture
			for (i = 0; i < mipMaps; ++i)
			{
				if (width == 0) width = 1;
				if (height == 0) height = 1;

				size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

				glCompressedTexImage2DARB(GL_TEXTURE_2D,
					i,
					image->_format,
					width,
					height,
					0,
					size,
					image->_pixels + offset);

				offset += size;

				width = (width / 2);
				height = (height / 2);
			}

			if (image->_pixels != NULL)
				delete[] image->_pixels;

			delete image;
		}
		else
		{
			//CDEBUG_ERROR_IF(image == NULL, std::string("Failed to load texture: ").append(file).c_str());
			return false;
		}

		return true;
	}

	DDSImageData* TextureLoader::LoadDDSImageData(const std::string& file)
	{
		DDSImageData* image = new DDSImageData();
		DDSDescription description;
		char fileCode[4]{};
		FILE* imageFile{};
		int factor;
		int bufferSize;

		errno_t error = fopen_s(&imageFile, file.c_str(), "rb");
		if (error != 0)
		{
			delete image;
			return nullptr;
		}
		else
		{
			//To get rid of warnings from VS
			if (imageFile)
				fread(fileCode, 1, 4, imageFile);
			else
			{
				delete image;
				return nullptr;
			}

			if (strncmp(fileCode, "DDS ", 4) != 0)
			{
				delete image;
				fclose(imageFile);
				return nullptr;
			}

			fread(&description, sizeof(description), 1, imageFile);

			if (description._pixelFormat._fourCC == DXT1_FLAG)
			{
				// DXT1's compression ratio is 8:1
				image->_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				factor = 2;
				image->_components = 3;
			}
			else
			{
				if (description._pixelFormat._fourCC == DXT3_FLAG)
				{
					// DXT3's compression ratio is 4:1
					image->_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
					factor = 4;
					image->_components = 4;
				}
				else
				{
					if (description._pixelFormat._fourCC == DXT5_FLAG)
					{
						// DXT5's compression ratio is 4:1
						image->_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
						factor = 4;
						image->_components = 4;
					}
					else
					{
						delete image;
						return nullptr;
					}
				}
			}

			if (description._linearSize == 0)
			{
				delete image;
				return nullptr;
			}

			if (description._mipMapCount > 1)
				bufferSize = description._linearSize * factor;
			else
				bufferSize = description._linearSize;

			image->_pixels = new unsigned char[bufferSize];
			fread(image->_pixels, 1, bufferSize, imageFile);

			// Close the file
			fclose(imageFile);

			image->_width = description._width;
			image->_height = description._height;
			image->_numMipMaps = description._mipMapCount;
			return image;
		}
	}

	bool TextureLoader::LoadHDR(const std::string& file)
	{
		std::cout << "Loading HDR : " << file << std::endl;
		int width, height, nrComp;
		float* data = stbi_loadf(file.c_str(), &width, &height, &nrComp, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			return true;
		}

		return false;
	}

	bool TextureLoader::LoadOtherImage(const std::string& file, const std::string& newFile, bool sRGB)
	{
		(void)newFile;
		std::cout << "Loading Other Images " << file << std::endl;
		int width, height, channel;
		unsigned char* textureData = SOIL_load_image(file.c_str(), &width, &height, &channel, SOIL_LOAD_AUTO);

		//Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_CLAMP_TO_EDGE (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//Set texture filtering parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Using mipmap to solve the lines issue. It is due to oversized texture on small sprite and therefore mipmapping to solve it
		//Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//If textureData is valid and channel is 4 use RGBA, else use RGB
		if (textureData && channel == 4)
		{
			if(sRGB)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (textureData && channel == 3)
		{
			if (sRGB)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			//Debug_LogToFile("Resources/Logs/Generate_texture.txt", "Texture loading failed");
			//CDEBUG_ASSERT(textureData, std::string("Failed to load texture: ").append(file).c_str());
			std::cout << "Failed to load texture: " << SOIL_last_result() << std::endl;
			SOIL_free_image_data(textureData);
			return false;
		}

		//SAVES CUSTOM DDS HERE
		//SOIL_save_image(newFile.c_str(), SOIL_SAVE_TYPE_DDS, width, height, channel, textureData);
		//
		SOIL_free_image_data(textureData);

		return true;
	}

	unsigned TextureLoader::LoadTexture(const std::string & file, bool sRGB)
	{
		if (file.empty())
		{
			return 0;
		}
		//Generate a texture Id for use and bind it to the active texture unit
		unsigned texture = 0;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		bool result = false;

		std::string finalFileName = file;

		//Load the image file
		if (file.find(".dds") != std::string::npos)
		{
			result = LoadDDSImage(file);
		}
		else if (file.find(".hdr") != std::string::npos)
		{
			result = LoadHDR(file);
		}
		else
		{
			////Checks for the file name
			//std::string name;
			//size_t pos = file.rfind("\\");
			////Get just the string after the last path
			//if (pos != std::string::npos)
			//{
			//	name = file.substr(pos + 1);
			//}
			//else
			//{
			//	name = file;
			//}

			////Trim the extension to get the file name
			//std::string newName;
			//name.erase(name.rfind("."));
			//newName = s_LocalTexturePathName + name + s_DDSFileFormat;
			//finalFileName = newName;

			result = LoadOtherImage(file, file, sRGB);
		}

		if(!result)
		{
			glDeleteTextures(1, &texture);

			//Unbind textureID from active texture unit once done with loading
			glBindTexture(GL_TEXTURE_2D, 0);

			//Not sure what to do here for error handling, used to throw catch all the way out
			return 0;
		}

		//Unbind textureID from active texture unit once done with loading
		glBindTexture(GL_TEXTURE_2D, 0);

		TextureManager::GetInstance().AddTexture(texture, finalFileName);

		return texture;
	}
}