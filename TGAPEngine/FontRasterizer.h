#pragma once

#include <iostream>
#include <string>
#include <ft2build.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include "Utility.h"
#include "Config.h"
namespace engine{
	class FontRasterizer
	{
	public:
		FontRasterizer();
		~FontRasterizer();
		bool loadFont(std::string fontPath);
		FT_GlyphSlot getGlyphBitmap(unsigned int charWidth, unsigned int charHeight, unsigned long c);
		FT_GlyphSlot getGlyph(unsigned int charWidth, unsigned int charHeight, unsigned long c);
		GLubyte * getTextBitmap(unsigned int charWidth, unsigned int charHeight, std::string text, unsigned int * resultWidth);
		GLubyte * getGlyphBitmapBytes(unsigned int charWidth, unsigned int charHeight, unsigned long c){ return getGlyphBitmap(charWidth, charHeight, c)->bitmap.buffer; };
	private:
		FT_Library ft;
		FT_Face face;
	};

}