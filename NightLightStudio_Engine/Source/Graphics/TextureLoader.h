#pragma once
#include <string>
#include "../Window/WndUtils.h"
#include "../../SOIL/SOIL.h"
#include "../SOIL/stb_image_aug.h"
#include "TextureManager.h"
#include <cstdint>

namespace NS_GRAPHICS
{
	static std::string s_DDSFileFormat = ".dds";
	static std::string s_LocalTexturePathName = "Asset\\Texture\\";

	///////////////////////////////////////////////////////////////////////////////
	///DDS variables for loading dds files
	///////////////////////////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////////////////////////
	///TGA variables for loading tga files
	///////////////////////////////////////////////////////////////////////////////
	struct TGAImage
	{
		uint8_t _imageIDLength;
		uint8_t _colourMapType;
		uint8_t _imageType;

		uint16_t _colourMapOrigin;   
		uint16_t _colourMapLength;
		uint8_t _colourMapDepth;

		uint16_t _originX;
		uint16_t _originY;
		uint16_t _width;
		uint16_t _height;
		uint8_t _pixelDepth;

		uint8_t _imageDescriptor;

		uint8_t* _imageID;
		uint8_t* _colourMapData;
		uint8_t* _imageData;
	};

	enum class TGA_ERROR{
		ERROR_NONE,
		ERROR_FOPEN,
		ERROR_EOF,
		ERROR_WRITE,
		ERROR_CMAP_TYPE,
		ERROR_IMG_TYPE,
		ERROR_NO_IMG,
		ERROR_CMAP_MISSING,
		ERROR_CMAP_PRESENT,
		ERROR_CMAP_LENGTH,
		ERROR_CMAP_DEPTH,
		ERROR_ZERO_SIZE,
		ERROR_PIXEL_DEPTH,
		ERROR_NO_MEM,
		ERROR_NOT_CMAP,
		ERROR_RLE,
		ERROR_INDEX_RANGE,
		ERROR_MONO
	};

	//TGA Helper Functions
	std::string GetError(const TGA_ERROR errcode);
	void FreeTargaBuffers(TGAImage* target);
	TGA_ERROR Load(TGAImage* dest, const std::string& filename);
	TGA_ERROR LoadFromFile(TGAImage* dest, FILE* fp);
	TGA_ERROR LoadRLE(TGAImage* dest, FILE* fp);
	int IsColourMapped(const TGAImage* tga);
	int IsRLE(const TGAImage* tga);
	int IsRightToLeft(const TGAImage* tga);
	int IsTopToBottom(const TGAImage* tga);

	TGA_ERROR FlipVertical(TGAImage* tga);
	TGA_ERROR FlipHorizontal(TGAImage* tga);
	TGA_ERROR ConvertDepth(TGAImage* tga, const uint8_t bits);
	TGA_ERROR UnmapColour(TGAImage* tga);
	TGA_ERROR UnpackPixel(const uint8_t* src, const uint8_t bits, uint8_t* b, uint8_t* g, uint8_t* r, uint8_t* a);
	TGA_ERROR PackPixel(uint8_t* dest, const uint8_t bits, const uint8_t b, const uint8_t g, const uint8_t r, const uint8_t a);

	class TextureLoader
	{
		TextureLoader();
		~TextureLoader();

		//Handles DDS
		bool LoadDDSImage(const std::string& file);
		DDSImageData* LoadDDSImageData(const std::string& file);

		//Handles HDR
		bool LoadHDR(const std::string& file);

		//Handles TGA
		bool LoadTarga(const std::string& file);


		//Handles other types
		bool LoadOtherImage(const std::string& file, const std::string& newFile, bool sRGB = false, bool blackAlpha = false);

	public:
		// Unique Singleton instance
		static TextureLoader& GetInstance()
		{
			static TextureLoader instance;
			return instance;
		}

		unsigned LoadTexture(const std::string& file, bool sRGB = false, bool blackAlpha = false);
	};
}