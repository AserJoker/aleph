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
    for (auto &line : _lines) {
      auto renderable = line->getComponent<video::RenderableComponent>();
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
  }

  std::vector<core::AutoPtr<runtime::Entity>> _lines;

public:
  GameMainSystem() {
    std::string str = "";
    on(&GameMainSystem::onInput);
    FILE *fp = fopen("text.txt", "r");
    int32_t y = 0;
    while (!feof(fp)) {
      char str[1024] = {};
      fgets(str, 1024, fp);
      auto entity = new runtime::Entity{};
      auto renderable = new video::RenderableComponent{};
      renderable->getPosition().y = y;
      renderable->setCharacter(str);
      entity->addComponent(renderable);
      _lines.push_back(entity);
      y++;
    }
    fclose(fp);
  }
};

int main(int argc, char *argv[]) {
  core::Singleton<runtime::Application> theApp;
  theApp->addModule(new runtime::TerminalModule)
      ->addSystem(new GameMainSystem)
      ->addSystem(new video::RendererSystem);
  return theApp->run(argc, argv);
}