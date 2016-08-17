#include "Scene.h"

namespace engine{
	Scene::Scene()
	{
	}


	Scene::~Scene()
	{
	}
	void Scene::removeEntity(Entity *e){
		std::vector<Entity *>::iterator it = entities.begin();
		while (it != entities.end()){
			if ((*it) == e){
				//Entity * removedEntity = ()
				it = entities.erase(it);
				break;
			}
			else{
				++it;
			}

		}
	}
	void Scene::addEntity(Entity *e)
	{
		entities.emplace_back(e);
	}
	void Scene::draw(DrawingContext * context){
		for (std::vector<Entity *>::iterator it = entities.begin(); it != entities.end(); ++it)

		{
			(*it)->draw(glm::mat4(1.0f),context);
		}
	}
	Entity * Scene::findEntityByName(std::string name){
		for (std::vector<Entity *>::iterator it = entities.begin(); it != entities.end(); ++it)

		{
			if ((*it)->name == name){
				return (*it);
			}
		}
		return nullptr;
	}
}