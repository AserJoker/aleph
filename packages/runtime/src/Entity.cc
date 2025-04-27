#include "runtime/include/Entity.hpp"
#include <algorithm>
using namespace aleph;
using namespace aleph::runtime;

std::unordered_map<std::string, std::vector<Entity *>> Entity::_entitys;

std::vector<Entity *> &Entity::getEntities(const std::string &type) {
  return _entitys[type];
}

Entity::Entity() {}

Entity::~Entity() {
    while (!_components.empty()) {
        _components.erase(_components.begin());
    }
}

void Entity::addComponent(const core::AutoPtr<BaseComponent> &component) {
  auto type = component->getType();
  if (!_components.contains(type)) {
    getEntities(type).push_back(this);
  }
  _components[type] = component;
}

void Entity::removeComponent(const core::AutoPtr<BaseComponent> &component) {
  auto type = component->getType();
  if (_components.contains(type) && _components.at(type) == component) {
    _components.erase(type);
    auto &entities = getEntities(type);
    auto it = std::find(entities.begin(), entities.end(), this);
    if (it != entities.end()) {
      entities.erase(it);
    }
  }
}

core::AutoPtr<BaseComponent> Entity::getComponent(const std::string &type) {
  if (_components.contains(type)) {
    return _components.at(type);
  }
  return nullptr;
}

const core::AutoPtr<BaseComponent>
Entity::getComponent(const std::string &type) const {
  if (_components.contains(type)) {
    return _components.at(type);
  }
  return nullptr;
}
