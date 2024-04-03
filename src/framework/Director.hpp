#pragma once
#include "../core/EventBus.hpp"
#include "../core/Object.hpp"
#include "../core/Singleton.hpp"
#include "../device/EventInput.hpp"
#include "../ui/Document.hpp"
#include "../ui/Text.hpp"
#include "../video/Renderer.hpp"
#include "EventInitialize.hpp"
#include "EventPostInitialize.hpp"
#include "EventPreInitialize.hpp"
#include "EventTick.hpp"
#include "EventUnInitialize.hpp"
#include <fmt/xchar.h>
namespace aleph::framework {
class Director : public core::Object {
private:
  core::Auto<video::Renderer> renderer;
  core::Auto<ui::Document> document;
  ui::Element *box;

protected:
  void onPreInitialize(const EventPreInitialize &) {}
  void onInitialize(const EventInitialize &) {
    auto terminal = core::Singleton<device::Terminal>::getInstance();
    renderer = new video::Renderer(terminal);
    document = new ui::Document(renderer);
    document->initialize();
    auto root = document->getRoot();
    int32_t w, h;
    terminal->getTerminalSize(w, h);
    root->setSize({w, h});
    box = new ui::Element();
    box->setActivable(true);
    box->ref() | ui::Element::Size(10, 10);
    for (auto i = 0; i < 100; i++) {
      auto text = new ui::Text(fmt::format(L"text{}", i));
      text->ref() | ui::Element::MarginTop(i);
      box->append(text);
      text->setActivable(true);
    }
    root->append(box);
    box->ref() | ui::Element::Border();
  }
  void onPostInitialize(const EventPostInitialize &) {}

  void onUnInitialize(const EventUnInitialize &) {}

  void onTick(const EventTick &) {}
  void onInput(const device::EventInput &input) {
    if (input.getKey().name == L"<resize>") {
      auto terminal = core::Singleton<device::Terminal>::getInstance();
      video::Rect viewport = {0, 0, 0, 0};
      terminal->getTerminalSize(viewport.width, viewport.height);
      renderer->setViewport(viewport);
      document->getRoot()->setSize({viewport.width, viewport.height});
    }
    if (input.getKey().name == L" ") {
      box->isDebug = true;
    }
  }

public:
  void initialize() {
    auto bus = core::Singleton<core::EventBus>::getInstance();
    bus->on(this, &Director::onPreInitialize);
    bus->on(this, &Director::onInitialize);
    bus->on(this, &Director::onPostInitialize);
    bus->on(this, &Director::onUnInitialize);
    bus->on(this, &Director::onTick);
    bus->on(this, &Director::onInput);
  }
};
}; // namespace aleph::framework