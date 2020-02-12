#pragma once

#include <string>

#include "user.h"

struct UserData {
  int id;
  std::string email;
  UserType type;
};

struct CompanyData {
  std::string name;
  int number_of_employees;
};

class Database {
 public:
  UserData GetUserById(int user_id);
  void SaveUser(const User& user);

  CompanyData GetCompany();
  void SaveCompany(int number_of_employees);
};