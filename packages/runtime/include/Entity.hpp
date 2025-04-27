#pragma once
#include "Component.hpp"
#include "core/include/AutoPtr.hpp"
#include "core/include/ObjectBase.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace aleph::runtime {
class Entity : public core::ObjectBase {
private:
  static std::unordered_map<std::string, std::vector<Entity *>> _entitys;

public:
  static std::vector<Entity *> &getEntities(const std::string &type);

private:
  std::unordered_map<std::string, core::AutoPtr<BaseComponent>> _components;

public:
  Entity();

  ~Entity() override;

  void addComponent(const core::AutoPtr<BaseComponent> &component);

  void removeComponent(const core::AutoPtr<BaseComponent> &component);

  core::AutoPtr<BaseComponent> getComponent(const std::string &type);

  template <class T> core::AutoPtr<T> getComponent() {
    return getComponent(T::TYPE_NAME).template cast<T>();
  }

  const core::AutoPtr<BaseComponent>
  getComponent(const std::string &type) const;
};
}; // namespace aleph::runtime