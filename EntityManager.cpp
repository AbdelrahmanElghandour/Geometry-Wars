#include "EntityManager.h"


/*
	Some Notes:
		- Only add or remove entities in the beginning of a frame, not in the middle of a frame
*/


void EntityManager::update() {
	for (auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}
	m_entitiesToAdd.clear();

	removeDeadEntities(m_entities);

	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}


void EntityManager::removeDeadEntities(EntityVector& vec)
{
	// remove all the dead entities from input vector
	// this is called by the update function
	vec.erase(
		std::remove_if(
			vec.begin(),
			vec.end(),
			[](const std::shared_ptr<Entity>& e)
			{
				return !e->isActive();
			}),
		vec.end());
}


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntites++, tag));
	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVector& EntityManager::getEntitiesByTag(const std::string& tag) const {
	auto it = m_entityMap.find(tag);
	if (it == m_entityMap.end())
	{
		return {};
	}
	return m_entityMap.at(tag);
}

const EntityVector& EntityManager::getEntities() const {
	return m_entities;
}