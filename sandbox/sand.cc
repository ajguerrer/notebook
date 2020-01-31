#include "sand.h"

#include <memory>
#include <numeric>
#include <string>

MessageRenderer::MessageRenderer()
    : sub_renderers_({std::make_unique<HeaderRenderer>(),
                      std::make_unique<BodyRenderer>(),
                      std::make_unique<FooterRenderer>()}) {}

std::string MessageRenderer::Render(const Message& message) {
  return std::accumulate(sub_renderers_.begin(), sub_renderers_.end(),
                         std::string{},
                         [&](std::string m, std::shared_ptr<Renderer> r) {
                           return std::move(m) + r->Render(message);
                         });
}