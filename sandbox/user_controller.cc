#include "user_controller.h"

#include "user.h"

void UserController::ChangeEmail(int user_id, std::string new_email) {
  const UserData user_data = database_.GetUserById(user_id);
  User user(user_data.id, user_data.email, user_data.type);

  const CompanyData company_data = database_.GetCompany();

  const int new_number_of_employees = user.ChangeEmail(
      new_email, company_data.name, company_data.number_of_employees);

  database_.SaveCompany(new_number_of_employees);
  database_.SaveUser(user);
  message_bus_.SendEmailChangedMessage(user_id, new_email);
}