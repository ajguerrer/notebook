#pragma once

#include <string>

#include "database.h"
#include "message_bus.h"

class UserController {
 public:
  void ChangeEmail(int user_id, std::string new_email);

 private:
  Database database_;
  MessageBus message_bus_;
};