#pragma once
#include "../core/EventBus.hpp"
#include "../core/Object.hpp"
#include "../core/Singleton.hpp"
#include "../framework/EventTick.hpp"
#include "../ui/Element.hpp"
#include "../video/Renderer.hpp"
#include "EventKey.hpp"
#include "Root.hpp"
#include <chrono>
#include <curses.h>
#include <thread>
namespace aleph::ui {
class Document : public core::Object {
private:
  ui::Element *root;
  core::Auto<video::Renderer> renderer;
  ui::Element *active;
  Element *findNext(Element *node) {
    Element *active = nullptr;
    if (!node->getChildren().empty()) {
      for (auto &c : node->getChildren()) {
        if (c->isActivable()) {
          if (c->getChildren().empty()) {
            active = c;
          } else {
            active = findNext(c);
          }
          break;
        }
      }
    }
    if (!active) {
      auto pnode = node;
      while (active == nullptr) {
        auto parent = pnode->getParent();
        if (!parent) {
          break;
        }
        auto children = parent->getChildren();
        auto mask = false;
        for (auto &c : children) {
          if (mask) {
            if (c->isActivable()) {
              if (c->getChildren().empty()) {
                active = c;
              } else {
                active = findNext(c);
              }
              break;
            }
          }
          if (c == pnode) {
            mask = true;
          }
        }
        pnode = parent;
      }
    }
    return active;
  }
  Element *findLast(Element *node) {
    Element *active = nullptr;
    if (!node->getChildren().empty()) {
      auto children = node->getChildren();
      std::reverse(children.begin(), children.end());
      for (auto &c : children) {
        if (c->isActivable()) {
          if (c->getChildren().empty()) {
            active = c;
          } else {
            active = findLast(c);
          }
          break;
        }
      }
    }
    if (!active) {
      auto pnode = node;
      while (active == nullptr) {
        auto parent = pnode->getParent();
        if (!parent) {
          break;
        }
        auto children = parent->getChildren();
        std::reverse(children.begin(), children.end());
        auto mask = false;
        for (auto &c : children) {
          if (mask) {
            if (c->isActivable()) {
              if (c->getChildren().empty()) {
                active = c;
              } else {
                active = findLast(c);
              }
              break;
            }
          }
          if (c == pnode) {
            mask = true;
          }
        }
        pnode = parent;
      }
    }
    return active;
  }

protected:
  void onTick(const framework::EventTick &) {
    using namespace std::chrono;
    renderer->clear();
    root->draw(renderer);
    renderer->present();
  }
  void nextActive() {
    if (!active) {
      active = findNext(root);
    } else {
      active->dective();
      active = findNext(active);
      if (active == nullptr) {
        active = findNext(root);
      }
    }
    if (active) {
      active->active();
    }
  }
  void lastActive() {
    if (!active) {
      active = findLast(root);
    } else {
      active->dective();
      active = findLast(active);
      if (active == nullptr) {
        active = findLast(root);
      }
    }
    if (active) {
      active->active();
    }
  }
  void onInput(const device::EventInput &e) {
    EventKey key(e.getKey(), active);
    if (active) {
      active->emit(key);
    }
    if (!key.isCanceled()) {
      if (key.getKey().name == L"<tab>") {
        nextActive();
      } else if (key.getKey().name == L"<s-tab>") {
        lastActive();
      } else if (key.getKey().name == L"<esc>") {
        auto app = core::Singleton<framewrok::Application>::getInstance();
        app->exit();
      }
    }
  }
  void onUiEvent(const ui::Event &e) {
    if (e.getEvent() == "active") {
      if (e.getTarget() != active) {
        active->dective();
        active = const_cast<Element *>(e.getTarget());
      }
    }
  }

public:
  Document(const core::Auto<video::Renderer> &renderer)
      : root(nullptr), renderer(renderer), active(nullptr) {
    root = new ui::Root();
  }
  ~Document() override { delete root; }
  void initialize() {
    auto bus = core::Singleton<core::EventBus>::getInstance();
    bus->on(this, &Document::onTick);
    bus->on(this, &Document::onInput);
    bus->on(this, &Document::onUiEvent);
  }
  ui::Element *getRoot() { return root; }
};
} // namespace aleph::ui