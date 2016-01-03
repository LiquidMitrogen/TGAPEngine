#include "FontRasterizer.h"

namespace engine{
	FontRasterizer::FontRasterizer()
	{


		if (FT_Init_FreeType(&ft)) {
			std::cout << "Could not init freetype library" << std::endl;
		}
	}


	FontRasterizer::~FontRasterizer()
	{
	}


	bool FontRasterizer::loadFont(std::string fontPath){

		FT_Error error = FT_New_Face(ft,
			fontPath.c_str(),
			0,
			&face);
		if (error == FT_Err_Unknown_File_Format)
		{
			std::cout << "Wrong font file format" << std::endl; system("pause");
			exit(1);
		}
		else if (error)
		{
			std::cout << "No such font file found or file could not be read - " <<fontPath.c_str()<< std::endl; system("pause");
			exit(1);
		}
		return true;
	}

	FT_GlyphSlot FontRasterizer::getGlyphBitmap(unsigned int charWidth, unsigned int charHeight, unsigned long c){
		FT_Error error = FT_Set_Pixel_Sizes(
			face,   /* handle to face object */
			charWidth,      /* pixel_width           */
			charHeight);   /* pixel_height          */

		unsigned int glyph_index = FT_Get_Char_Index(face, c);
		if (glyph_index == 0){
			std::cout << "Missing glyph " << std::endl; system("pause");
		}

		error = FT_Load_Glyph(
			face,          /* handle to face object */
			glyph_index,   /* glyph index           */
			FT_LOAD_NO_BITMAP);  /* load flags, see below */
		if (error != 0){
			std::cout << "Load glyph error " << std::endl; system("pause");
		}

		error = FT_Render_Glyph(face->glyph,   /* glyph slot  */
			FT_RENDER_MODE_NORMAL); /* render mode */

		if (error != 0){
			std::cout << "Render glyph error " << std::endl; system("pause");
		}

		return this->face->glyph;

	}

	FT_GlyphSlot FontRasterizer::getGlyph(unsigned int charWidth, unsigned int charHeight, unsigned long c){
		FT_Error error = FT_Set_Pixel_Sizes(
			face,   /* handle to face object */
			charWidth,      /* pixel_width           */
			charHeight);   /* pixel_height          */

		unsigned int glyph_index = FT_Get_Char_Index(face, c);
		if (glyph_index == 0){
			std::cout << "Missing glyph " << std::endl; system("pause");
		}

		error = FT_Load_Glyph(
			face,          /* handle to face object */
			glyph_index,   /* glyph index           */
			FT_LOAD_NO_BITMAP);  /* load flags, see below */
		if (error != 0){
			std::cout << "Load glyph error " << std::endl; system("pause");
		}


		return this->face->glyph;

	}

	GLubyte * FontRasterizer::getTextBitmap(unsigned int charWidth, unsigned int charHeight, std::string text, unsigned int * resultWidth){
		//FT_Glyph * glyphs = new FT_Glyph[text.length()];
		FT_Glyph *  glyphs = new FT_Glyph[text.length()];
		if (DEBUG_MODE == 1){
			std::cout << "new FontRasterizer" << std::endl;
		}
		unsigned int * advances = new unsigned int[text.length()];
		if (DEBUG_MODE == 1){
			std::cout << "new FontRasterizer" << std::endl;
		}
		int belowBaseline = 0;
		unsigned int calculatedWidth = 0;
		int maxBearingY = 0;
		if (charWidth == 0) charWidth = charHeight;
		if (charHeight == 0) charHeight = charWidth;
		
		for (unsigned int i = 0; i < text.length(); i++){
			FT_GlyphSlot slot;
			slot = getGlyphBitmap(charWidth, charHeight, text[i]);
			FT_Get_Glyph(slot, &glyphs[i]);
			int glyphBelowBaseline = floor((slot->metrics.horiBearingY - slot->metrics.height) / 64.0f);
			if (glyphBelowBaseline < belowBaseline){
				belowBaseline = glyphBelowBaseline;
			}
			advances[i] = slot->advance.x / 64;
			calculatedWidth += slot->advance.x / 64;
			
			//if (glyphs[i]->metrics.horiBearingY > maxBearingY){
				//maxBearingY = glyphs[i]->metrics.horiBearingY;
			//}
			//blitChar(slot->bitmap.buffer, bitmap, i * charWidth, charHeight -  (slot->bitmap_top + ), slot->bitmap.width, slot->bitmap.rows, text.length() * charWidth, charHeight);
			//blitChar(slot->bitmap)
		}
		if (resultWidth != NULL)
			*resultWidth = calculatedWidth;
		/*if ((calculatedWidth % 4) != 0){
			float m = calculatedWidth / 4.0f;
			calculatedWidth = ceil(m) * calculatedWidth;
		}*/

		GLubyte * bitmap = new GLubyte[calculatedWidth * 4 * charHeight];
		if (DEBUG_MODE == 1){
			std::cout << "new FontRasterizer" << std::endl;
		}
		
		if (bitmap == NULL){
			std::cout << "allocation error new out of memory" << std::endl;
			exit(-1);
		}
		for (int j = 0; j < calculatedWidth * 4 * charHeight; j++){
			bitmap[j] = 0;
		}
		unsigned int advanceSum = 0;
		for (unsigned int i = 0; i < text.length() ; i++){
			FT_Error error = FT_Glyph_To_Bitmap(
				&glyphs[i],
				FT_RENDER_MODE_NORMAL,
				NULL,
				1);

			if (error != 0){
				std::cout << "Render glyph error " << std::endl; system("pause");
			}
			FT_BitmapGlyph rendered = (FT_BitmapGlyph)glyphs[i];
			blitCharRGBA(rendered->bitmap.buffer, bitmap, advanceSum, charHeight - rendered->top + belowBaseline, rendered->bitmap.width, rendered->bitmap.rows, calculatedWidth, charHeight);
			advanceSum += advances[i];
			FT_Done_Glyph(glyphs[i]);
		}
		return bitmap;
		

	}
}