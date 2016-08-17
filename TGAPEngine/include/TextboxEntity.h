#pragma once
#include <string>
#include "Entity.h"
#include "FontRasterizer.h"
namespace engine{
	class TextboxEntity :
		public Entity
	{
	public:
		TextboxEntity(VertexAttributes * va, std::string text,FontRasterizer * ft);
		TextboxEntity(Entity * e, std::string text, FontRasterizer * ft);
		virtual ~TextboxEntity();
		void setText(std::string text);
		//Image * setBackgroundImage(Image * bg);
		void setTextHeight(unsigned int textHeight);
		void setFontName(std::string fontName);
		virtual void prepare();
		float getWidthToHeightRatio();
	private:
		Image * textImage = NULL;
		//Image * bgImage = NULL;
		std::string text;
		bool textNeedsUpdate = true;
		bool fontNeedsUpdate = true;
		void updateText();
		void updateFont();
		FontRasterizer * ft;
		unsigned int textHeight = 64;
		std::string fontName = "Hack-Regular.ttf";
		float ratio = 1.0f;
	};

}