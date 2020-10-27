#pragma once
#include <string>
#include "../Window/WndUtils.h"
#include "../../SOIL/SOIL.h"
#include "TextureManager.h"

namespace NS_GRAPHICS
{
	struct DDSImageData
	{
		int _width;
		int _height;
		GLint _components;
		unsigned _format;
		int _numMipMaps;
		unsigned char* _pixels;
	};

	struct DDSPixelFormat
	{
		unsigned long _size;
		unsigned long _flags;
		unsigned long _fourCC;
		unsigned long _rgbBitCount;
		unsigned long _rBitMask;
		unsigned long _gBitMask;
		unsigned long _bBitMask;
		unsigned long _aBitMask;
	};

	struct DDSDescription
	{
		unsigned long _size;
		unsigned long _flags;
		unsigned long _height;
		unsigned long _width;
		unsigned long _linearSize;
		unsigned long _depth;
		unsigned long _mipMapCount;
		unsigned long _reserved1[11];
		DDSPixelFormat _pixelFormat;
		unsigned long _caps1;
		unsigned long _caps2;
		unsigned long _reserved2[3];
	};

	const unsigned long DXT1_FLAG = 0x31545844;
	const unsigned long DXT3_FLAG = 0x33545844;
	const unsigned long DXT5_FLAG = 0x35545844;

	class TextureLoader
	{
		TextureLoader();
		~TextureLoader();

		bool LoadDDSImage(const std::string& file);
		DDSImageData* LoadDDSImageData(const std::string& file);
		bool LoadImage(const std::string& file);

	public:
		// Unique Singleton instance
		static TextureLoader& GetInstance()
		{
			static TextureLoader instance;
			return instance;
		}

		bool LoadTexture(const std::string& file);
	};
}