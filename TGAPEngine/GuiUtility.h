#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Config.h"
#include <iostream>

#define TG_ANCHOR_LT 0
#define TG_ANCHOR_CT 1
#define TG_ANCHOR_RT 2
#define TG_ANCHOR_LC 3
#define TG_ANCHOR_CC 4
#define TG_ANCHOR_RC 5
#define TG_ANCHOR_LB 6
#define TG_ANCHOR_CB 7
#define TG_ANCHOR_RB 8

namespace engine{

glm::vec3 guiPositioningCalculator(unsigned int anchorPoint, glm::vec2 scale, glm::vec2 wantedPos);





}