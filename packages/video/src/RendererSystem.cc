#include "video/include/RendererSystem.hpp"
#include "core/include/Object.hpp"
#include "core/include/UString.hpp"
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
      auto [col, row] = component->getPosition();
      col++;
      row++;
      if (row < 1 || row > (int32_t)size.height) {
        continue;
      }
      auto &ustr = component->getCharacter();
      int32_t offset = -1;
      for (int32_t idx = 0; idx < (int32_t)ustr.length(); idx++) {
        auto chr = ustr.at(idx);
        std::vector tokens = {chr};
        if (chr.size() == 1 && chr[0] < 0x20) {
          tokens = {"^", {(char)(chr[0] + 'A')}};
        }
        for (auto &ch : tokens) {
          if (col + offset >= 0) {
            current[row][col + offset] = {
                .brush = component->getBrush(),
                .chr = ch,
            };
          }
          offset += core::UString(ch).getRenderWidth();
        }
      }
    }
  }
  std::unordered_map<int32_t, std::map<int32_t, Pixel>> diff;
  for (auto &[row, line] : _current) {
    for (auto &[col, pixel] : line) {
      if (!current.contains(row) || !current.at(row).contains(col) ||
          current.at(row).at(col) != pixel) {
        diff[row][col] = {
            .chr = {' '},
        };
      }
    }
  }
  for (auto &[row, line] : current) {
    for (auto &[col, pixel] : line) {
      if (pixel.brush && pixel.brush->isUpdated()) {
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
    auto &current = _current[row];
    for (auto &[col, pixel] : line) {
      if (current.contains(col - 1)) {
        auto chr = current.at(col - 1).chr;
        if (core::UString(chr).getRenderWidth() > 1) {
          current.erase(col);
        } else {
          current[col] = pixel;
        }
      } else {
        current[col] = pixel;
      }
    }
    std::string result;
    size_t idx = 0;
    for (auto &[col, pixel] : current) {
      while (idx < col) {
        idx++;
        if (idx > size.width) {
          break;
        }
        result += ' ';
      }
      idx += core::UString(pixel.chr).getRenderWidth();
      if (idx > size.width) {
        break;
      }
      result += pixel.chr;
    }
    auto it = current.begin();
    while (it != current.end()) {
      if (it->second.chr.size() == 1 && it->second.chr[0] == ' ' &&
          !it->second.brush) {
        current.erase(it);
        it = current.begin();
      } else {
        it++;
      }
    }
    _terminal->move(1, row);
    _terminal->print(result);
    _terminal->flush();
  }
}

RendererSystem::RendererSystem() { on(&RendererSystem::onTick); }