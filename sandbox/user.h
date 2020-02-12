#pragma once

#include <string>

#include "user_type.h"

class User {
 public:
  User(int id, std::string email, UserType type)
      : id_(id), email_(email), type_(type) {}

  int ChangeEmail(const std::string& new_email,
                  const std::string& company_domain_name,
                  int number_of_employees);

 private:
  int id_;
  std::string email_;
  UserType type_;
};
