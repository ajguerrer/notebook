#include "user.h"

#include "absl/strings/str_split.h"

int User::ChangeEmail(const std::string& new_email,
                      const std::string& company_domain_name,
                      int number_of_employees) {
  if (email_ == new_email) return number_of_employees;

  const std::string email_domain =
      std::vector<std::string>(absl::StrSplit(new_email, '@'))[1];
  const bool is_email_corperate = email_domain == company_domain_name;
  const UserType new_type =
      is_email_corperate ? UserType::kEmployee : UserType::kCustomer;

  if (type_ != new_type) {
    const bool is_user_employee = new_type == UserType::kEmployee;
    number_of_employees =
        is_user_employee ? number_of_employees + 1 : number_of_employees - 1;
  }

  email_ = new_email;
  type_ = new_type;

  return number_of_employees;
}
