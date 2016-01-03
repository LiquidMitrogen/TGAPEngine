#include "GuiUtility.h"
namespace engine{
	glm::vec3 guiPositioningCalculator(unsigned int anchorPoint, glm::vec2 scale, glm::vec2 wantedPos){
		glm::vec2 apos;
		switch (anchorPoint){
		case TG_ANCHOR_LT:{
			apos = glm::vec2(-1.0f, 1.0f);
			break;
		}
		case TG_ANCHOR_CT:{
			apos = glm::vec2(0.0f, 1.0f);
			break;
		}
		case TG_ANCHOR_RT:{
			apos = glm::vec2(1.0f, 1.0f);
			break;
		}
		case TG_ANCHOR_LC:{
			apos = glm::vec2(-1.0f, 0.0f);
			break;
		}
		case TG_ANCHOR_CC:{
			apos = glm::vec2(0.0f, 0.0f);
			break;
		}
		case TG_ANCHOR_RC:{
			apos = glm::vec2(1.0f, 0.0f);
			break;
		}
		case TG_ANCHOR_LB:{
			apos = glm::vec2(-1.0f, -1.0f);
			break;
		}
		case TG_ANCHOR_CB:{
			apos = glm::vec2(0.0f, -1.0f);
			break;
		}
		case TG_ANCHOR_RB:{
			apos = glm::vec2(1.0f, -1.0f);
			break;
		}
		default:{
			return glm::vec3(0.0f, 0.0f, 0.0f);
			break;
		}

		}


		apos.x = apos.x * scale.x;
		apos.y = apos.y * scale.y;

		glm::vec3 finalPos = glm::vec3(wantedPos.x - apos.x, wantedPos.y - apos.y, 0.0f);

		/*if (DEBUG_MODE){
			std::cout << apos.x << " " << apos.y  << std::endl;
			std::cout << apos.x << " " << apos.y << std::endl;
			}*/

		return finalPos;




	}

	
}