#pragma once

#include <memory>
#include <string>
#include <vector>

struct Message {
  std::string header;
  std::string body;
  std::string footer;
};

class Renderer {
 public:
  virtual std::string Render(const Message& message) = 0;
};

class MessageRenderer : public Renderer {
 public:
  MessageRenderer();
  virtual ~MessageRenderer() {}
  virtual std::string Render(const Message& message);

 private:
  std::vector<std::shared_ptr<Renderer>> sub_renderers_;
};

class HeaderRenderer : public Renderer {
 public:
  virtual std::string Render(const Message& message) {
    return "<head><title>" + message.header + "</title></head>";
  }
};

class BodyRenderer : public Renderer {
 public:
  virtual std::string Render(const Message& message) {
    return "<body>" + message.body + "</body>";
  }
};

class FooterRenderer : public Renderer {
 public:
  virtual std::string Render(const Message& message) {
    return "<footer>" + message.footer + "</footer>";
  }
};