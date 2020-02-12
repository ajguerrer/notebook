#pragma once

#include <memory>
#include <string>
#include <vector>

#include "absl/strings/ascii.h"

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
  std::vector<std::shared_ptr<Renderer>> GetSubRenderers() {
    return sub_renderers_;
  }
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

class User {
 public:
  std::string GetName() const { return name_; }
  void SetName(const std::string& name) { name_ = NormalizeName(name); }

 private:
  std::string NormalizeName(const std::string& name) {
    return std::string(absl::StripAsciiWhitespace(name)).substr(50);
  }

  std::string name_;
};

class UserController {
 private:
  User GetUserFromDatabase(int) { return User(); }
  void SaveUserToDatabase(const User& user) {}

 public:
  void RenameUser(int user_id, const std::string& new_name) {
    User user = GetUserFromDatabase(user_id);

    user.SetName(new_name);

    SaveUserToDatabase(user);
  }
};