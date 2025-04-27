#include "video/include/RendererSystem.hpp"
#include "core/include/Object.hpp"
#include "runtime/include/Entity.hpp"
#include "runtime/include/TickEvent.hpp"
#include "system/include/Terminal.hpp"
#include "video/include/RenderableComponent.hpp"
#include <cstdint>
#include <unordered_map>
using namespace aleph;
using namespace aleph::video;

void RendererSystem::onTick(core::Object *, const runtime::TickEvent &) {
  std::unordered_map<int32_t, std::map<int32_t, Pixel>> current;
  auto &entities = runtime::Entity::getEntities(RenderableComponent::TYPE_NAME);
  auto &size = _terminal->getSize();
  for (auto &entity : entities) {
    auto component = entity->getComponent<RenderableComponent>();
    if (component) {
      auto &[col, row] = component->getPosition();
      if (row < 1 || row > (int32_t)size.height) {
        continue;
      }
      auto &ustr = component->getCharacter();
      for (int32_t idx = 0; idx < (int32_t)ustr.length(); idx++) {
        if (col + idx >= 0 && col + idx < (int32_t)size.width) {
          current[row][col + idx] = {
              .brush = component->getBrush(),
              .chr = ustr.at(idx),
          };
        }
      }
    }
  }
  std::unordered_map<int32_t, std::map<int32_t, Pixel>> diff;
  for (auto &[row, line] : _current) {
    for (auto &[col, pixel] : line) {
      if (!current.contains(row) || !current.at(row).contains(col)) {
        diff[row][col] = {
            .chr = {' ', 0},
        };
      }
    }
  }
  for (auto &[row, line] : current) {
    for (auto &[col, pixel] : line) {
      if (pixel.brush->isUpdated()) {
        diff[row][col] = pixel;
        pixel.brush->setIsUpdated(false);
      } else if (_current.contains(row) && _current.at(row).contains(col) &&
                 _current.at(row).at(col) == pixel) {
        continue;
      } else {
        diff[row][col] = pixel;
      }
    }
  }
  if (diff.empty()) {
    return;
  }
  for (auto &[row, line] : diff) {
    for (auto &[col, pixel] : line) {
      _current[row][col] = pixel;
    }
  }
  for (auto &[row, line] : _current) {
    size_t idx = 0;
    std::string str;
    for (auto &[col, pixel] : line) {
      while (idx < col) {
        str.push_back(' ');
        idx++;
      }
      if (pixel.brush) {
        if (!_brush || _brush->getHandle() != pixel.brush) {
          _brush = pixel.brush;
          if (_brush) {
            str += _brush->getFormat();
          }
        }
      }
      str += pixel.chr;
      idx++;
    }
    str += "\033[m";
    _terminal->move(1, row);
    _terminal->print(str);
    _terminal->flush();
  }
  for (auto &[row, line] : diff) {
    for (auto &[col, pixel] : line) {
      if (pixel.brush == nullptr && pixel.chr[0] == ' ' && pixel.chr[1] == 0) {
        _current[row].erase(col);
        if (_current[row].empty()) {
          _current.erase(row);
        }
      }
    }
  }
}

RendererSystem::RendererSystem() { on(&RendererSystem::onTick); }