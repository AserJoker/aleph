#include "core/include/AutoPtr.hpp"
#include "core/include/Object.hpp"
#include "core/include/Singleton.hpp"
#include "runtime/include/Application.hpp"
#include "runtime/include/Entity.hpp"
#include "runtime/include/QuitEvent.hpp"
#include "runtime/include/System.hpp"
#include "runtime/include/TerminalModule.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Key.hpp"
#include "system/include/Terminal.hpp"
#include "video/include/Brush.hpp"
#include "video/include/RenderableComponent.hpp"
#include "video/include/RendererSystem.hpp"
#include <cstdio>

using namespace aleph;

class GameMainSystem : public runtime::System {
private:
  void onInput(core::Object *, const system::InputEvent &e) {
    auto &codes = e.getCodes();
    if (codes.size() == 1 && codes[0] == system::Key::ESC) {
      emit<runtime::QuitEvent>();
    }
    auto renderable = _entity->getComponent<video::RenderableComponent>();
    auto &pos = renderable->getPosition();
    if (codes.size() == 1) {
      if (codes[0] == system::Key::LEFT) {
        pos.x--;
      }
      if (codes[0] == system::Key::RIGHT) {
        pos.x++;
      }
      if (codes[0] == system::Key::UP) {
        pos.y--;
      }
      if (codes[0] == system::Key::DOWN) {
        pos.y++;
      }
      if (codes[0] == 'b') {
      }
    }
  }

  core::AutoPtr<runtime::Entity> _entity;

public:
  GameMainSystem() {
    on(&GameMainSystem::onInput);
    _entity = new runtime::Entity{};
    auto brush = new video::Brush{{
        .background = {0xfe, 0x0f, 0x0},
        .flag = system::Terminal::Attr::BACKGROUND,
    }};
    auto renderable = new video::RenderableComponent{};
    renderable->getBrush() = brush;
    renderable->getCharacter() = "Test";
    _entity->addComponent(renderable);
  }
};

int main(int argc, char *argv[]) {
  core::Singleton<runtime::Application> theApp;
  theApp->addModule(new runtime::TerminalModule)
      ->addSystem(new GameMainSystem)
      ->addSystem(new video::RendererSystem);
  return theApp->run(argc, argv);
}