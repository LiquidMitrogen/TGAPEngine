#pragma once
#include <vector>
#include <string>
#include "Entity.h"
#include "DrawingContext.h"
namespace engine{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();
		void addEntity(Entity *e);
		void removeEntity(Entity *e);
		void draw(DrawingContext * context);
		Entity * findEntityByName(std::string name);
	protected:
		std::vector<Entity * > entities;
	};

}