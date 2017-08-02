#pragma once
#include <vector>
#include <string>
#include "Entity.h"
#include "DrawingContext.h"
namespace engine{
	class Scene
	{
	protected:
		std::vector<Entity * > entities;

	public:
		//ctor
		Scene();
		//dtor
		virtual ~Scene();
		//methods
		void addEntity(Entity *e);
		void removeEntity(Entity *e);
		void draw(DrawingContext * context);
		Entity * findEntityByName(std::string name);
	
	};

}