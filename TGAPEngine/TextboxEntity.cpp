#include "TextboxEntity.h"

namespace engine{
	TextboxEntity::TextboxEntity(VertexAttributes * va, std::string text, FontRasterizer * ft) :Entity(va)
	{
		this->text = text;
		this->ft = ft;
		if (ft == NULL){
			std::cout << "InvalidParam" << std::endl;
			system("pause");
			exit(-1);
		}
		this->textNeedsUpdate = true;
	}
	TextboxEntity::TextboxEntity(Entity * e, std::string text, FontRasterizer * ft) : Entity(e->getVertexAttributes()){
		this->text = text;
		this->ft = ft;
		if (ft == NULL){
			std::cout << "InvalidParam" << std::endl;
			system("pause");
			exit(-1);
		}
		this->textNeedsUpdate = true;
	}
	void TextboxEntity::setText(std::string text){
		if (text != this->text){
			this->text = text;
			this->textNeedsUpdate = true;
		}
		
	}
	TextboxEntity::~TextboxEntity()
	{
	}
	void TextboxEntity::prepare(){
		if (textNeedsUpdate){
			updateText();
		}
		Entity::prepare();
	}
	/*Image * TextboxEntity::setBackgroundImage(Image * bg){
		Image * tmp = this->bgImage;
		this->bgImage = bg;
		this->textNeedsUpdate = true;
		return tmp;
	}*/
	void TextboxEntity::updateText(){
		this->textNeedsUpdate = false;
		unsigned int width;
		if (fontNeedsUpdate == true){
			updateFont();
		}
		
		GLubyte * bitmap = ft->getTextBitmap(textHeight, textHeight, this->text, &width);
		//if (this->textImage != NULL){
			//delete this->textImage;
		//}
		this->textImage = new Image(bitmap, width, textHeight, 1, GL_RGBA, TG_FILTER_LINEAR,false);
		if (DEBUG_MODE == 1){
			std::cout << "new Image" << std::endl;
		}
		delete bitmap;
		this->textures.clear();
		this->textures.emplace_back(this->textImage);
		this->ratio = (float)width / textHeight;
		/*if (bgImage != NULL){
			this->textures.emplace_back(this->bgImage);
		}*/
		

	}
	void TextboxEntity::setTextHeight(unsigned int textHeight){
		this->textHeight = textHeight;
		this->textNeedsUpdate = true;
	}
	void TextboxEntity::setFontName(std::string fontName){
		if (fontName != this->fontName){
			this->fontName = fontName;

			this->textNeedsUpdate = true;
			this->fontNeedsUpdate = true;
		}
	}
	float TextboxEntity::getWidthToHeightRatio(){
		if (textNeedsUpdate){
			updateText();
		}
		return this->ratio;
	}
	void TextboxEntity::updateFont(){
		this->fontNeedsUpdate = false;
		ft->loadFont(this->fontName);
	}
}