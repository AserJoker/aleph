#pragma once
#include "../core/EventBus.hpp"
#include "../core/Singleton.hpp"
#include "Element.hpp"
namespace aleph::ui {
class Root : public Element {
protected:
  void on(Event &event) override {
    auto bus = core::Singleton<core::EventBus>::getInstance();
    bus->emit(event);
    Element::on(event);
  }

public:
  Root() { setActivable(true); }
};
}; // namespace aleph::ui