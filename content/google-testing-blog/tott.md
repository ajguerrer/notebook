---
title: "Testing on the Toilet"
---

## Exercise Service Call Contracts in Tests

*November 27, 2018* -
[original post](https://testing.googleblog.com/2018/11/testing-on-toilet-exercise-service-call.html)

If the code under test relies on the contract of a service, prefer exercising the service call
instead of mocking it out. Some service owners provide a fake. Otherwise, use a hermetic server.

## Make Interfaces Hard to Misuse

*July 25, 2018* -
[original post](https://testing.googleblog.com/2018/07/code-health-make-interfaces-hard-to.html)

Don't push the responsibility of maintaining invariants on the caller.

{{% notice warning %}}
```cpp
class Vector {
  explicit Vector(int num_slots);
  int RemainingSlots() const;
  void AddSlots(int num_slots);
  void Insert(int value);
};
```
{{% /notice %}}

Caller needs to check `RemainingSlots` and if 0, `AddSlots` in order for `Insert` to work properly.

`Insert` could automatically manage slots.

{{% notice tip %}}
```cpp
class Vector {
  explicit Vector(int num_slots);
  void Insert(int value);
};
```
{{% /notice %}}

Other examples:

- `Initialize` / `Deitialize` functions.
- Allowing partially created objects.
- Parameters that can have invalid values.

Sometimes it's not practical to have a foolproof interface. In those cases rely on static analysis
or documentation.

## Only Verify Relevant Method Arguments

*June 26, 2018* -
[original post](https://testing.googleblog.com/2018/06/testing-on-toilet-only-verify-relevant.html)

Tests become fragile when they expect exact values on irrelevant arguments.


{{% notice warning %}}
```cpp
TEST_F(DisplayGreetingTest, ShowSpecialGreetingOnNewYearsDay) {
  fakeClock_.SetTime(kNewYearsDay);
  fakeUser_.SetName("Fake User");
  EXPECT_CALL(mockUserPrompter_, UpdatePrompt("Hi Fake User! Happy New Year!",
                                              TitleBar("2018-01-01"),
                                              PromptStyle::kNormal));
  userGreeter_.DisplayGreeting();
}
```
{{% /notice %}}

Only verify one behavior per test. Only verify arguments that affect the correctness of the specific
behavior being tested.

{{% notice tip %}}
```cpp
TEST_F(DisplayGreetingTest, ShowSpecialGreetingOnNewYearsDay) {
  fakeClock_.SetTime(kNewYearsDay);
  EXPECT_CALL(mockUserPrompter_, UpdatePrompt(HasSubstr("Happy New Year!"), _, _));
  userGreeter_.DisplayGreeting();
}

TEST_F(DisplayGreetingTest, RenderUserName) {
  fakeUser_.SetName("Fake User");
  EXPECT_CALL(mockUserPrompter_, UpdatePrompt(HasSubstr("Fake User"), _, _));
  userGreeter_.DisplayGreeting();
}
```
{{% /notice %}}

## Keep Tests Focused

*June 11, 2018* -
[original post](https://testing.googleblog.com/2018/06/testing-on-toilet-keep-tests-focused.html)

Testing too many scenarios at once can make it difficult to understand test and reason about the
failure.

{{% notice warning %}}
```cpp
TEST_F(BankAcountTest, WidthdrawFromAccount) {
  Transaction transaction = acceunt_.Deposit(Usd(5));
  clock_.AdvanceTime(kMinTimeToSettle);
  account_.Settle(transaction);

  EXPECT_THAT(account_.Withdraw(Usd(5)), IsOk());
  EXPECT_THAT(account_.Withdraw(USd(1)), IsRejected());
  account_.SetOverdraftLimit(Usd(1));
  EXPECT_THAT(account_.Withdraw(Usd(1)), IsOk());
}
```
{{% /notice %}}

Break up each scenario into its own test.

{{% notice tip %}}
```cpp
TEST_F(BankAccountTest, CanWithdrawWithinBalance) {
  DepositAndSettle(Usd(5));
  EXPECT_THAT(account_.Withdraw(Usd(5)), IsOk());
}

TEST_F(BankAccountTest, CannotOverdraw) {
  DepositAndSettle(Usd(5));
  EXPECT_THAT(account_.Withdraw(Usd(6)), IsRejected());
}

TEST_F(BankAccountTest, CanOverdrawUpToOverdraftLimit) {
  DepositAndSettle(Usd(5));
  account_.SetOverdraftLimit(Usd(1));
  EXPECT_THAT(account_.Withdraw(Usd(6)), IsOk());
}
```
{{% /notice %}}

Notice how each test only asserts on the output of one call per test.

## Understanding Code in Review

*May 1, 2018* - [original
post](https://testing.googleblog.com/2018/05/code-health-understanding-code-in-review.html)

If you find yourself reviewing code that is difficult to understand, don't waste time reviewing it.

{{% notice warning %}}
```cpp
bool IsOkay(int n) {
  bool f = false;
  for (int i = 2; i <= n; ++i) {
    if (n % i == 0) f = true;
  }
  return !f;
}
```
{{% /notice %}}

Ask for it to be clarified.

{{% notice tip %}}
```cpp
bool IsPrime(int n) {
  for (int divisor = 2; divisor <= n / 2; ++divisor) {
    if (n % divisor == 0) return false;
  }
  return true;
}
```
{{% /notice %}}

Clarifying code often results in noticing improvements.

## Cleanly Create Test Data

*Febuary 20, 2018* - [original
post](https://testing.googleblog.com/2018/02/testing-on-toilet-cleanly-create-test.html)

Helper methods make it easier to create test data, but they don't age well.

{{% notice warning %}}
```cpp
// Starts simple
Company company = NewCompany(kPublic);

// But soon acquires more parameters
Company small = NewCompany(2, 2, nullptr, kPublic);
Company privately_owned = NewCompany(0, 0, nullptr, kPrivate);
Company bankrupt = NewCompany(0 , 0, kPastDate, kPublic);

// Or more methods
Company small = NewCompanyWithEmployeesAndBoardMembers(2, 2, kPublic);
Company privately_owned = NewCompanyWithType(kPrivate);
Company bankrupt = NewCompanyWithBankruptcyDate(kPastDate, kPublic);
```
{{% /notice %}}

Try the builder pattern.

{{% notice tip %}}
```cpp
Company small = Company::Builder{}.SetEmployees(2).SetBoardMembers(2).Build();
Company privately_owned = Company::Builder{}.SetType(kPrivate).Build();
Company bankrupt = Company::Builder{}.SetBankruptcyDate(kPastDate).Build();
Company default_company = Company::Builder{}.Build();

class Company::Builder {
public:
  Builder& SetEmployees(int n) { employees_ = n; return *this; }
  Builder& SetBoardMembers(int n) { board_members_ = n; return *this; }
  Builder& SetBankruptcyDate(BankruptcyDate d) { date_ = d; return *this; }
  Builder& SetType(Type t) { type_ = t; return *this; }

  Company Build() const { return Company(employees_, board_members_, date_, type_); }

private:
  int employees_ = 0;
  int board_members_ = 0;
  BankruptcyDate date_ = kBeforeDate;
  Type type_ = kPublic;
};
```
{{% /notice %}}

## Only Expect State-Changing Method Calls
*December 11, 2017* - [original
post](https://testing.googleblog.com/2017/12/testing-on-toilet-only-verify-state.html)

Expecting calls to methods that don't change state can make a test brittle, less readable, and
provide a false sense of security.

{{% notice warning %}}
```cpp
TEST(UserAuthorizer, AddPermissionToDatabase) {
  UserAuthorizer user_authorizer(mock_user_service_, mock_permission_db_);

  EXPECT_CALL(mock_user_service_, IsUserActive(kUser);
  EXPECT_CALL(mock_permission_db_, GetPermission(kUser);
  EXPECT_CALL(mock_permission_db_, IsValidPermission(kReadAccess);
  EXPECT_CALL(mock_permission_db_, AddPermission(kUser, kReadAccess);

  user_authorizer.GrantPermission(kUser, kReadAccess);
}
```
{{% /notice %}}

Instead of expecting non-state-changing methods to be called, use them to verify state.

{{% notice warning %}}
```cpp
EXPECT_FALSE(IsUserActive(kUser));
```
{{% /notice %}}

With unnecessary `EXPECT_CALL`s removed, the test becomes:

{{% notice tip %}}
```cpp
Test(UserAuthorizer, AddPermissionToDatabase) {
  UserAuthorizer user_authorizer(mock_user_service_, mock_permission_db_);

  EXPECT_CALL(mock_permission_db_, AddPermission(kUser, kReadAccess);

  user_authorizer.GrantPermission(kUser, kReadAccess);
}
```
{{% /notice %}}

## Obsessed With Primitives?

*November 14, 2017* - [original
post](https://testing.googleblog.com/2017/11/obsessed-with-primitives.html)

Code that relies too heavily on primitive types instead of custom abstractions can be hard to
understand and maintain.

{{% notice warning %}}
```cpp
std::vector<std::pair<int, int>> polygon = { std::make_pair(0, 0),
                                             std::make_pair(0, 4),
                                             std::make_pair(4, 0) };
std::pair<std::pair<int, int>, std::pair<int, int>> bounding_box = GetBoundingBox(polygon);
int area = (bounding_box.second.first  - bounding_box.first.first) *
           (bounding_box.second.second - bounding_box.first.second);

```
{{% /notice %}}

Make higher-level abstractions.

{{% notice tip %}}
```cpp
Polygon polygon = RightTriangle(4, 4);
int area = polygon.GetBoundingBox().GetArea();
```
{{% /notice %}}

This advice doesn't just apply to primitives and The STL. It's possible for any type to be too
primitive for the job.

{{% notice tip %}}
```cpp
Polygon polygon = IsoscelesRightTriangle(4);
int area = polygon.GetBoundingBox().GetArea();
```
{{% /notice %}}

## IdentifierNamingPostForWorldWideWebBlog

*October 23, 2017* - [original
post](https://testing.googleblog.com/2017/10/code-health-identifiernamingpostforworl.html)

Names should be clear and precise.

Don't mention the type in the variable name. It's OK for the name and the type match.

{{% notice warning %}}
```cpp
std::string nameString;
std::list<std::time_t> holidayDateList;
```
{{% /notice %}}

{{% notice tip %}}
```cpp
std::string name;
std::list<std::time_t> holidays;
Payments payments;
```
{{% /notice %}}

Don't use overly specific names. Get more specific if there is a need for disambiguation.

{{% notice warning %}}
```cpp
Monster final_battle_most_dangerous_boss_monster;
Payments non_typical_monthly_payments;
```
{{% /notice %}}

{{% notice tip %}}
```cpp
Monster boss;
Payments payments;
```
{{% /notice %}}

Do not repeat context.

{{% notice warning %}}
```cpp
class AnnualHolidaySale {
  bool PromoteHolidaySale();
  int annual_sale_rebate_;
};
```
{{% /notice %}}

{{% notice tip %}}
```cpp
class AnnualHolidaySale {
  bool Promote();
  int rebate_;
}
```
{{% /notice %}}

## To Comment or Not to Comment

*July 17, 2017* - [original
post](https://testing.googleblog.com/2017/07/code-health-to-comment-or-not-to-comment.html)

Comments are not always helpful.

{{% notice warning %}}
```cpp
// Subtract discount from final price.
final_price = (num_items * item_price) -
              std::min(5, num_items) * item_price *  0.1;

// Filter offensive words.
for (std::string word : words) { ... }

int width = ...; // Width in pixels.

// Safe since height is always > 0.
return width / height;
```
{{% /notice %}}

It's often better to make your code self-explanatory.

{{% notice tip %}}
```cpp
price = num_items * item_price;
discount = std::min(5, num_items) * item_price * 0.1;
final_price = price - discount;

filterOffensiveWords(words);

Pixels width = ...;

checkArgument(height > 0);
return width / height;
```
{{% /notice %}}

Avoid using comments to explain *what* code does. Use comments to explain *why* code does something.

## To Many Comments on Your Code Reviews?

*June 19, 2017* - [original
post](https://testing.googleblog.com/2017/06/code-health-too-many-comments-on-your.html)

Before you send out the code review:

- Review your own code first.
- Discuss with colleagues.
- Make bite-sized reviews.
- Add comments to the code if necessary.

When addressing code review comments:

- Review your code after addressing non-trivial comments.
- Understand the reviews thought process.
- Answer questions in the code.

## Reduce Nesting, Reduce Complexity

*June 15, 2017* - [original
post](https://testing.googleblog.com/2017/06/code-health-reduce-nesting-reduce.html)

Deeply nested code is error-prone and hurts readability.

{{% notice warning %}}
```cpp
Response response = server.Call(request);

if (response.GetStatus() == Status::kOk) {
  if (response.GetAuthorizedUser()) {
    if (response.GetEnc() == "utf-8") {
      std::vector<Row> rows = response.GetRows();
      if (!rows.empty()) {
        avg = std::accumulate(rows.begin(), rows.end(), 0, ParseRow) / rows.size();
        return avg;
      } else {
      throw EmptyException();
    } else {
      throw AuthException('unauthorized');
    }
  } else {
    throw ValueException('wrong encoding');
  }
} else {
  throw RpcException(response.GetStatus());
}
```
{{% /notice %}}

The code above could be refactored using a technique called guard clause.

{{% notice tip %}}
```cpp
Response response = server.Call(request);

if (response.GetStatus() != Status::kOk) {
  throw RpcException(response.GetStatus());
}

if (!response.GetAuthorizedUser()) {
  throw ValueException('wrong encoding');
}

if (!response.GetEnc() != "utf-8") {
  throw AuthException('unauthorized');
}

std::vector<Row> rows = response.GetRows();
if (rows.empty()) {
  throw EmptyException();
}

avg = std::accumulate(rows.begin(), rows.end(), 0, ParseRow) / rows.size();
return avg;
```
{{% /notice %}}

Can you spot the bug now?

