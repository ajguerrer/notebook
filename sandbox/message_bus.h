#pragma once

#include <string>

class MessageBus {
 public:
  void SendEmailChangedMessage(int user_id, std::string new_email);
};