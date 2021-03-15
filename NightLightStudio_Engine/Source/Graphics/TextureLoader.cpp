#include "TextureLoader.h"
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../Log/SpeedLog.h"
#pragma warning(disable:4244)
#pragma warning(disable:4996)
#pragma warning(disable:6029)

namespace NS_GRAPHICS
{
	#define SANE_DEPTH(x) ((x) == 8 || (x) == 16 || (x) == 24 || (x) == 32)
	#define UNMAP_DEPTH(x)            ((x) == 16 || (x) == 24 || (x) == 32)

	#define TGA_COLOR_MAP_ABSENT    0
	#define TGA_COLOR_MAP_PRESENT   1

	#define TGA_IMAGE_TYPE_NONE          0 /* no image data */
	#define TGA_IMAGE_TYPE_COLORMAP      1 /* uncompressed, color-mapped */
	#define TGA_IMAGE_TYPE_BGR           2 /* uncompressed, true-color */
	#define TGA_IMAGE_TYPE_MONO          3 /* uncompressed, black and white */
	#define TGA_IMAGE_TYPE_COLORMAP_RLE  9 /* run-length, color-mapped */
	#define TGA_IMAGE_TYPE_BGR_RLE      10 /* run-length, true-color */
	#define TGA_IMAGE_TYPE_MONO_RLE     11 /* run-length, black and white */

	#define TGA_ATTRIB_BITS (uint8_t)(BIT(0)|BIT(1)|BIT(2)|BIT(3))
	#define TGA_R_TO_L_BIT  (uint8_t)BIT(4)
	#define TGA_T_TO_B_BIT  (uint8_t)BIT(5)
	#define TGA_UNUSED_BITS (uint8_t)(BIT(6)|BIT(7))

	#define BIT(index) (1 << (index))

		//Checks for endian type
	#ifdef _BIG_ENDIAN
	# define htole16(x) ( (((x) & 0x00FF) << 8) | (((x) & 0xFF00) >> 8) )
	# define letoh16(x) htole16(x)
	#else
	# define htole16(x) (x)
	# define letoh16(x) (x)
	#endif

	TextureLoader::TextureLoader()
	{

	}

	TextureLoader::~TextureLoader()
	{

	}

	bool TextureLoader::LoadDDSImage(const std::string& file)
	{
		std::string outStr = "TextureLoader::LoadDDSImage: Loading DDS Images "; outStr.append(file);
		TracyMessage(outStr.c_str(), outStr.size());
		SPEEDLOG(outStr);
		//std::cout << "Loading DDS Images " << file << std::endl;
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
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
		std::string outStr = "TextureLoader::LoadHDR: Loading HDR : "; outStr.append(file);
		TracyMessage(outStr.c_str(), outStr.size());
		SPEEDLOG(outStr);
		//std::cout << "Loading HDR : " << file << std::endl;
		int width, height, nrComp;
		float* data = stbi_loadf(file.c_str(), &width, &height, &nrComp, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			stbi_image_free(data);
			return true;
		}

		return false;
	}

	TGA_ERROR Load(TGAImage* dest, const std::string& filename)
	{
		TGA_ERROR error;
		FILE* fp = fopen(filename.c_str(), "rb");
		if (fp == NULL) return TGA_ERROR::ERROR_FOPEN;
		error = LoadFromFile(dest, fp);
		fclose(fp);
		return error;
	}

	TGA_ERROR LoadFromFile(TGAImage* dest, FILE* fp)
	{
		#define BARF(errcode) \
				{ FreeTargaBuffers(dest);  return errcode; }

		#define READ(destptr, size) \
				if (fread(destptr, size, 1, fp) != 1) BARF(TGA_ERROR::ERROR_EOF)

		#define READ16(dest) \
				{ if (fread(&(dest), 2, 1, fp) != 1) BARF(TGA_ERROR::ERROR_EOF); \
				  dest = letoh16(dest); }

		dest->_imageID = NULL;
		dest->_colourMapData = NULL;
		dest->_imageData = NULL;

		READ(&dest->_imageIDLength, 1);
		READ(&dest->_colourMapType, 1);
		if (dest->_colourMapType != TGA_COLOR_MAP_ABSENT &&
			dest->_colourMapType != TGA_COLOR_MAP_PRESENT)
			BARF(TGA_ERROR::ERROR_CMAP_TYPE);

		READ(&dest->_imageType, 1);
		if (dest->_imageType == TGA_IMAGE_TYPE_NONE)
			BARF(TGA_ERROR::ERROR_NO_IMG);

		if (dest->_imageType != TGA_IMAGE_TYPE_COLORMAP &&
			dest->_imageType != TGA_IMAGE_TYPE_BGR &&
			dest->_imageType != TGA_IMAGE_TYPE_MONO &&
			dest->_imageType != TGA_IMAGE_TYPE_COLORMAP_RLE &&
			dest->_imageType != TGA_IMAGE_TYPE_BGR_RLE &&
			dest->_imageType != TGA_IMAGE_TYPE_MONO_RLE)
			BARF(TGA_ERROR::ERROR_IMG_TYPE);

		if (IsColourMapped(dest) &&
			dest->_colourMapType == TGA_COLOR_MAP_ABSENT)
			BARF(TGA_ERROR::ERROR_CMAP_MISSING);

		if (!IsColourMapped(dest) &&
			dest->_colourMapType == TGA_COLOR_MAP_PRESENT)
			BARF(TGA_ERROR::ERROR_CMAP_PRESENT);

		READ16(dest->_colourMapOrigin);
		READ16(dest->_colourMapLength);
		READ(&dest->_colourMapDepth, 1);
		if (dest->_colourMapType == TGA_COLOR_MAP_PRESENT)
		{
			if (dest->_colourMapLength == 0)
				BARF(TGA_ERROR::ERROR_CMAP_LENGTH);

			if (!UNMAP_DEPTH(dest->_colourMapDepth))
				BARF(TGA_ERROR::ERROR_CMAP_DEPTH);
		}

		READ16(dest->_originX);
		READ16(dest->_originY);
		READ16(dest->_width);
		READ16(dest->_height);

		if (dest->_width == 0 || dest->_height == 0)
			BARF(TGA_ERROR::ERROR_ZERO_SIZE);

		READ(&dest->_pixelDepth, 1);
		if (!SANE_DEPTH(dest->_pixelDepth) ||
			(dest->_pixelDepth != 8 && IsColourMapped(dest)))
			BARF(TGA_ERROR::ERROR_PIXEL_DEPTH);

		READ(&dest->_imageDescriptor, 1);

		if (dest->_imageIDLength > 0)
		{
			dest->_imageID = (uint8_t*)malloc(dest->_imageIDLength);
			if (dest->_imageID == NULL) BARF(TGA_ERROR::ERROR_NO_MEM);
			READ(dest->_imageID, dest->_imageIDLength);
		}

		if (dest->_colourMapType == TGA_COLOR_MAP_PRESENT)
		{
			dest->_colourMapData = (uint8_t*)malloc(
				(dest->_colourMapOrigin + dest->_colourMapLength) *
				dest->_colourMapDepth / 8);
			if (dest->_colourMapData == NULL) 
				BARF(TGA_ERROR::ERROR_NO_MEM);
			READ(dest->_colourMapData +
				(dest->_colourMapOrigin * dest->_colourMapDepth / 8),
				dest->_colourMapLength* dest->_colourMapDepth / 8);
		}

		dest->_imageData = (uint8_t*)malloc(
			dest->_width * dest->_height * dest->_pixelDepth / 8);
		if (dest->_imageData == NULL)
			BARF(TGA_ERROR::ERROR_NO_MEM);

		if (IsRLE(dest))
		{
			/* read RLE */
			TGA_ERROR error = LoadRLE(dest, fp);
			if (error != TGA_ERROR::ERROR_NONE)
				BARF(error);
		}
		else
		{
			/* uncompressed */
			READ(dest->_imageData,
				dest->_width * dest->_height * dest->_pixelDepth / 8);
		}

		return TGA_ERROR::ERROR_NONE;
		#undef BARF
		#undef READ
		#undef READ16
	}

	TGA_ERROR LoadRLE(TGAImage* dest, FILE* fp)
	{
		#define RLE_BIT BIT(7)
		#define READ(dest, size) \
        if (fread(dest, size, 1, fp) != 1) return TGA_ERROR::ERROR_EOF

		uint8_t* pos;
		uint32_t start = 0,
			end = dest->_width * dest->_height;
		uint8_t bytesPerPixel = dest->_pixelDepth / 8;

		pos = dest->_imageData;

		while ((start < end) && !feof(fp))
		{
			uint8_t b;
			READ(&b, 1);
			if (b & RLE_BIT)
			{
				uint8_t count, tmp[4], i;

				count = (b & ~RLE_BIT) + 1;
				READ(tmp, bytesPerPixel);

				for (i = 0; i < count; i++)
				{
					start++;
					if (start > end) 
						return TGA_ERROR::ERROR_RLE;
					memcpy(pos, tmp, bytesPerPixel);
					pos += bytesPerPixel;
				}
			}
			else
			{
				uint8_t count;

				count = (b & ~RLE_BIT) + 1;
				if (start + count > end) 
					return TGA_ERROR::ERROR_RLE;

				start += count;
				READ(pos, bytesPerPixel * count);
				pos += count * bytesPerPixel;
			}
		}
		return TGA_ERROR::ERROR_NONE;
		#undef RLE_BIT
		#undef READ
	}

	int IsColourMapped(const TGAImage* tga)
	{
		return (
			tga->_imageType == TGA_IMAGE_TYPE_COLORMAP ||
			tga->_imageType == TGA_IMAGE_TYPE_COLORMAP_RLE
			);
	}

	int IsRLE(const TGAImage* tga)
	{
		return (
			tga->_imageType == TGA_IMAGE_TYPE_COLORMAP_RLE ||
			tga->_imageType == TGA_IMAGE_TYPE_BGR_RLE ||
			tga->_imageType == TGA_IMAGE_TYPE_MONO_RLE
			);
	}

	int IsRightToLeft(const TGAImage* tga)
	{
		return (tga->_imageDescriptor & TGA_R_TO_L_BIT) != 0;
	}

	int IsTopToBottom(const TGAImage* tga)
	{
		return (tga->_imageDescriptor & TGA_T_TO_B_BIT) != 0;
	}

	TGA_ERROR FlipVertical(TGAImage* tga)
	{
		uint16_t col;
		size_t bytesPerPixel, line;
		uint8_t* top, * bottom;
		int topToBottom;

		if (!SANE_DEPTH(tga->_pixelDepth)) 
			return TGA_ERROR::ERROR_PIXEL_DEPTH;
		bytesPerPixel = (size_t)(tga->_pixelDepth / 8);
		line = bytesPerPixel * tga->_width;    

		for (col = 0; col < tga->_width; col++)
		{
			top = tga->_imageData + col * bytesPerPixel;
			bottom = top + (tga->_height - 1) * line;

			while (top < bottom)
			{
				uint8_t buffer[4];

				/* swap */
				memcpy(buffer, top, bytesPerPixel);
				memcpy(top, bottom, bytesPerPixel);
				memcpy(bottom, buffer, bytesPerPixel);

				top += line;
				bottom -= line;
			}
		}

		topToBottom = IsTopToBottom(tga);
		tga->_imageDescriptor &= ~TGA_T_TO_B_BIT; 
		if (!topToBottom)
			tga->_imageDescriptor |= TGA_T_TO_B_BIT;

		return TGA_ERROR::ERROR_NONE;
	}

	TGA_ERROR FlipHorizontal(TGAImage* tga)
	{
		uint16_t row;
		size_t bytesPerPixel;
		uint8_t* left, * right;
		int rightToLeft;

		if (!SANE_DEPTH(tga->_pixelDepth)) 
			return TGA_ERROR::ERROR_PIXEL_DEPTH;
		bytesPerPixel = (size_t)(tga->_pixelDepth / 8);

		for (row = 0; row < tga->_height; row++)
		{
			left = tga->_imageData + row * tga->_width * bytesPerPixel;
			right = left + (tga->_width - 1) * bytesPerPixel;

			while (left < right)
			{
				uint8_t buffer[4];

				memcpy(buffer, left, bytesPerPixel);
				memcpy(left, right, bytesPerPixel);
				memcpy(right, buffer, bytesPerPixel);

				left += bytesPerPixel;
				right -= bytesPerPixel;
			}
		}

		rightToLeft = IsRightToLeft(tga);
		tga->_imageDescriptor &= ~TGA_R_TO_L_BIT;
		if (!rightToLeft)
			tga->_imageDescriptor |= TGA_R_TO_L_BIT;

		return TGA_ERROR::ERROR_NONE;
	}

	TGA_ERROR ConvertDepth(TGAImage* tga, const uint8_t bits)
	{
		size_t srcSize, destSize;
		uint8_t srcBytesPerPixel, destBytePerPixel;
		uint8_t* src, * dest;

		if (!UNMAP_DEPTH(bits) ||
			!SANE_DEPTH(tga->_pixelDepth))
			return TGA_ERROR::ERROR_PIXEL_DEPTH;

		if (IsColourMapped(tga))
		{
			TGA_ERROR error = UnmapColour(tga);
			if (error != TGA_ERROR::ERROR_NONE) return error;
		}

		if (tga->_pixelDepth == bits) 
			return TGA_ERROR::ERROR_NONE;

		srcBytesPerPixel = tga->_pixelDepth / 8;
		destBytePerPixel = bits / 8;

		srcSize = (size_t)(tga->_width * tga->_height * srcBytesPerPixel);
		destSize = (size_t)(tga->_width * tga->_height * destBytePerPixel);

		if (srcSize > destSize)
		{
			void* tmp;

			/* convert forwards */
			dest = tga->_imageData;
			for (src = tga->_imageData;
				src < tga->_imageData + tga->_width * tga->_height * srcBytesPerPixel;
				src += srcBytesPerPixel)
			{
				uint8_t r, g, b, a;
				(void)UnpackPixel(src, tga->_pixelDepth, &r, &g, &b, &a);
				(void)PackPixel(dest, bits, r, g, b, a);
				dest += destBytePerPixel;
			}

			/* shrink */
			tmp = realloc(tga->_imageData, tga->_width * tga->_height * destBytePerPixel);
			if (tmp == NULL) 
				return TGA_ERROR::ERROR_NO_MEM;
			tga->_imageData = (uint8_t*)tmp;
		}
		else
		{
			/* expand */
			void* tmp = realloc(tga->_imageData,
				tga->_width * tga->_height * destBytePerPixel);
			if (tmp == NULL) 
				return TGA_ERROR::ERROR_NO_MEM;
			tga->_imageData = (uint8_t*)tmp;

			/* convert backwards */
			dest = tga->_imageData + (tga->_width * tga->_height - 1) * destBytePerPixel;
			for (src = tga->_imageData + (tga->_width * tga->_height - 1) * srcBytesPerPixel;
				src >= tga->_imageData;
				src -= srcBytesPerPixel)
			{
				uint8_t r, g, b, a;
				(void)UnpackPixel(src, tga->_pixelDepth, &r, &g, &b, &a);
				(void)PackPixel(dest, bits, r, g, b, a);
				dest -= destBytePerPixel;
			}
		}

		tga->_pixelDepth = bits;
		return TGA_ERROR::ERROR_NONE;
	}

	TGA_ERROR UnmapColour(TGAImage* tga)
	{
		uint8_t bytesPerPixel = tga->_colourMapDepth / 8;
		int pos;
		void* tmp;

		if (!IsColourMapped(tga)) 
			return TGA_ERROR::ERROR_NOT_CMAP;
		if (tga->_pixelDepth != 8) 
			return TGA_ERROR::ERROR_PIXEL_DEPTH;
		if (!SANE_DEPTH(tga->_colourMapDepth)) 
			return TGA_ERROR::ERROR_CMAP_DEPTH;

		tmp = realloc(tga->_imageData, tga->_width * tga->_height * bytesPerPixel);
		if (tmp == NULL) 
			return TGA_ERROR::ERROR_NO_MEM;
		tga->_imageData = (uint8_t*)tmp;

		for (pos = tga->_width * tga->_height - 1; pos >= 0; pos--)
		{
			uint8_t colourIndex = tga->_imageData[pos];
			uint8_t* colourBGR = tga->_colourMapData + (colourIndex * bytesPerPixel);

			if (colourIndex >= tga->_colourMapOrigin + tga->_colourMapLength)
				return TGA_ERROR::ERROR_INDEX_RANGE;

			memcpy(tga->_imageData + (pos * bytesPerPixel), colourBGR, (size_t)bytesPerPixel);
		}

		tga->_imageType = TGA_IMAGE_TYPE_BGR;
		tga->_pixelDepth = tga->_colourMapDepth;

		free(tga->_colourMapData);
		tga->_colourMapData = NULL;
		tga->_colourMapType = TGA_COLOR_MAP_ABSENT;
		tga->_colourMapOrigin = 0;
		tga->_colourMapLength = 0;
		tga->_colourMapDepth = 0;

		return TGA_ERROR::ERROR_NONE;
	}

	TGA_ERROR UnpackPixel(const uint8_t* src, const uint8_t bits, uint8_t* b, uint8_t* g, uint8_t* r, uint8_t* a)
	{
		switch (bits)
		{
		case 32:
			if (b) *b = src[0];
			if (g) *g = src[1];
			if (r) *r = src[2];
			if (a) *a = src[3];
			break;

		case 24:
			if (b) *b = src[0];
			if (g) *g = src[1];
			if (r) *r = src[2];
			if (a) *a = 0;
			break;

		case 16:
		{
			uint16_t src16 = (uint16_t)(src[1] << 8) | (uint16_t)src[0];

			#define FIVE_BITS (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4))
						if (b) *b = (uint8_t)(((src16)&FIVE_BITS) << 3);
						if (g) *g = (uint8_t)(((src16 >> 5) & FIVE_BITS) << 3);
						if (r) *r = (uint8_t)(((src16 >> 10) & FIVE_BITS) << 3);
						if (a) *a = (uint8_t)((src16 & BIT(15)) ? 255 : 0);
			#undef FIVE_BITS
			break;
		}

		case 8:
			if (b) *b = *src;
			if (g) *g = *src;
			if (r) *r = *src;
			if (a) *a = 0;
			break;

		default:
			return TGA_ERROR::ERROR_PIXEL_DEPTH;
		}
		return TGA_ERROR::ERROR_NONE;
	}

	TGA_ERROR PackPixel(uint8_t* dest, const uint8_t bits, const uint8_t b, const uint8_t g, const uint8_t r, const uint8_t a)
	{
		switch (bits)
		{
		case 32:
			dest[0] = b;
			dest[1] = g;
			dest[2] = r;
			dest[3] = a;
			break;

		case 24:
			dest[0] = b;
			dest[1] = g;
			dest[2] = r;
			break;

		case 16:
		{
			uint16_t tmp;

			#define FIVE_BITS (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4))
						tmp = (uint16_t)((b >> 3) & FIVE_BITS);
						tmp |= ((g >> 3) & FIVE_BITS) << 5;
						tmp |= ((r >> 3) & FIVE_BITS) << 10;
						if (a > 127) tmp |= BIT(15);
			#undef FIVE_BITS

			dest[0] = (uint8_t)(tmp & 0x00FF);
			dest[1] = (uint8_t)((tmp & 0xFF00) >> 8);
			break;
		}

		default:
			return TGA_ERROR::ERROR_PIXEL_DEPTH;
		}
		return TGA_ERROR::ERROR_NONE;
	}

	std::string GetError(const TGA_ERROR errcode)
	{
		switch (errcode)
		{
		case TGA_ERROR::ERROR_NONE:
			return std::string("No Error");
		case TGA_ERROR::ERROR_FOPEN:
			return std::string("Error opening file");
		case TGA_ERROR::ERROR_EOF:
			return std::string("Error premature end of file");
		case TGA_ERROR::ERROR_WRITE:
			return std::string("Error writing to file");
		case TGA_ERROR::ERROR_CMAP_TYPE:
			return std::string("Invalid colour type");
		case TGA_ERROR::ERROR_IMG_TYPE:
			return  std::string("Invalid image type");
		case TGA_ERROR::ERROR_NO_IMG:
			return  std::string("No image data found");
		case TGA_ERROR::ERROR_CMAP_MISSING:
			return std::string("Colour mapped image doesn't have colour map");
		case TGA_ERROR::ERROR_CMAP_PRESENT:
			return std::string("Non-color-mapped image with extraneous color map");
		case TGA_ERROR::ERROR_CMAP_LENGTH:
			return std::string("Color map has zero length");
		case TGA_ERROR::ERROR_CMAP_DEPTH:
			return std::string("Invalid color map depth");
		case TGA_ERROR::ERROR_ZERO_SIZE:
			return std::string("The image dimensions are zero");
		case TGA_ERROR::ERROR_PIXEL_DEPTH:
			return std::string("Invalid pixel depth");
		case TGA_ERROR::ERROR_NO_MEM:
			return std::string("Out of memory");
		case TGA_ERROR::ERROR_NOT_CMAP:
			return std::string("Image is not color mapped");
		case TGA_ERROR::ERROR_RLE:
			return std::string("RLE data is corrupt");
		case TGA_ERROR::ERROR_INDEX_RANGE:
			return std::string("Color map index out of range");
		case TGA_ERROR::ERROR_MONO:
			return std::string("Image is mono");
		default:
			return std::string("Unknown error code");
		}
	}
	void FreeTargaBuffers(TGAImage* target)
	{
		if (target->_imageID != NULL)
		{
			free(target->_imageID);
			target->_imageID = NULL;
		}
		if (target->_colourMapData != NULL)
		{
			free(target->_colourMapData);
			target->_colourMapData = NULL;
		}
		if (target->_imageData != NULL)
		{
			free(target->_imageData);
			target->_imageData = NULL;
		}
	}

	bool TextureLoader::LoadTarga(const std::string& file)
	{
		TGAImage image;

		if (Load(&image, file) == TGA_ERROR::ERROR_NONE)
		{
			// Make sure the image is left to right
			if (IsRightToLeft(&image))
				FlipHorizontal(&image);

			// Make sure the image is bottom to top
			if (IsTopToBottom(&image))
				FlipVertical(&image);

			// Make the image BGR 24
			ConvertDepth(&image, 24);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image._width, image._height, 0, GL_BGR,
				GL_UNSIGNED_BYTE, image._imageData);

			FreeTargaBuffers(&image);

			return true;
		}

		return false;
	}

	bool TextureLoader::LoadOtherImage(const std::string& file, const std::string& newFile, bool sRGB, bool blackAlpha)
	{
		(void)newFile;
		std::string outStr = "TextureLoader::LoadOtherImage: Loading Other Images "; outStr.append(file);
		TracyMessage(outStr.c_str(), outStr.size());
		SPEEDLOG(outStr);
		//std::cout << "Loading Other Images " << file << std::endl;
		int width, height, channel;
		unsigned char* textureData = SOIL_load_image(file.c_str(), &width, &height, &channel, SOIL_LOAD_AUTO);

		//Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_CLAMP_TO_EDGE (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Using mipmap to solve the lines issue. It is due to oversized texture on small sprite and therefore mipmapping to solve it
		//Set texture filtering parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

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
			{
				if (blackAlpha)
				{
					unsigned char* newTextureData = new unsigned char[width * height * 4];
					unsigned char tolerance = 30;

					int original = 0;
					for (int i = 3; i < width * height * 4; i += 4)
					{
						newTextureData[i - 3] = textureData[original];
						newTextureData[i - 2] = textureData[original+1];
						newTextureData[i - 1] = textureData[original+2];

						int avgColour = 0.333f * (newTextureData[i - 3] + newTextureData[i - 2] + newTextureData[i - 1]);
						if (avgColour <= tolerance)
						{
							newTextureData[i] = 0;
						}
						else
						{
							newTextureData[i] = 255;
						}

						original += 3;
					}

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newTextureData);
					delete[] newTextureData;
				}
				else
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
				}
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			//Debug_LogToFile("Resources/Logs/Generate_texture.txt", "Texture loading failed");
			//CDEBUG_ASSERT(textureData, std::string("Failed to load texture: ").append(file).c_str());
			std::string outStr2 = "TextureLoader::LoadOtherImage: Failed to load texture: "; outStr2.append(SOIL_last_result());
			TracyMessage(outStr2.c_str(), outStr2.size());
			SPEEDLOG(outStr2);
			//std::cout << "Failed to load texture: " << SOIL_last_result() << std::endl;
			SOIL_free_image_data(textureData);
			return false;
		}

		//SAVES CUSTOM DDS HERE
		//SOIL_save_image(newFile.c_str(), SOIL_SAVE_TYPE_DDS, width, height, channel, textureData);
		//
		SOIL_free_image_data(textureData);

		return true;
	}

	unsigned TextureLoader::LoadTexture(const std::string & file, bool sRGB, bool blackAlpha)
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
		else if (file.find(".tga") != std::string::npos)
		{
			result = LoadTarga(file);
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

			result = LoadOtherImage(file, file, sRGB, blackAlpha);
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