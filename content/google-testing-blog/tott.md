---
title: "Testing on the Toilet"
---

This page contains potable bits of testing best-practices that will keep you riveted to
your seat.

## Tests Too DRY? Make Them DAMP!

*December 03, 2019* -
[original post](https://testing.googleblog.com/2019/12/testing-on-toilet-tests-too-dry-make.html)

Though the DRY ("Don't Repeat Yourself") principle is great for production code, tests don't test 
themselves. 

{{% notice warning %}}
```cpp
class ForumTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (auto user : users_) {
      forum_.Register(user);
    }
  }

  Forum forum_;
  std::vector<User> users_ = {User("Alice"), User("Bob")};
}

TEST_F(ForumTest, CanRegisterMultipleUsers) {
  for (auto user : users_) {
    EXPECT_TRUE(forum_.HasRegisteredUser(user));
  }
}
```
{{% /notice %}}

Tests should optimize for readability, even at the expense of redundancy. Prefer the DAMP 
("Descriptive and Meaningful Phrases") principle. 

{{% notice tip %}}
```cpp
TEST(ForumTest, CanRegisterMultipleUsers) {
  Forum forum;
  User user1("Alice");
  User user2("Bob");
  forum.Register(user1);
  forum.Register(user2);

  EXPECT_TRUE(forum.HasRegisteredUser(user1));
  EXPECT_TRUE(forum.HasRegisteredUser(user2));
}
```
{{% /notice %}}

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

In the code above, the caller needs to check `RemainingSlots`, and if `0`, `AddSlots` in order for
`Insert` to work properly.

Instead, `Insert` could automatically manage slots.

{{% notice tip %}}
```cpp
class Vector {
  explicit Vector(int num_slots);
  void Insert(int value);
};
```
{{% /notice %}}

Other signs an interface is hard to use:

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
  fake_clock_.SetTime(kNewYearsDay);
  fake_user_.SetName("Fake User");
  EXPECT_CALL(mock_user_prompter_, UpdatePrompt("Hi Fake User! Happy New Year!",
                                                TitleBar("2018-01-01"),
                                                PromptStyle::kNormal));
  user_greeter_.DisplayGreeting();
}
```
{{% /notice %}}

- Only verify one behavior per test.
- Only verify arguments that affect the correctness of the specific behavior being tested.

{{% notice tip %}}
```cpp
TEST_F(DisplayGreetingTest, ShowSpecialGreetingOnNewYearsDay) {
  fake_clock_.SetTime(kNewYearsDay);
  EXPECT_CALL(mock_user_prompter_, UpdatePrompt(HasSubstr("Happy New Year!"), _, _));
  user_greeter_.DisplayGreeting();
}

TEST_F(DisplayGreetingTest, RenderUserName) {
  fake_user_.SetName("Fake User");
  EXPECT_CALL(mock_user_prompter_, UpdatePrompt(HasSubstr("Fake User"), _, _));
  user_greeter_.DisplayGreeting();
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
TEST_F(BankAccountTest, WithdrawFromAccount) {
  Transaction transaction = account_.Deposit(Usd(5));
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

Notice how each test only verifies the output of one call per test.

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

*February 20, 2018* - [original
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

It is fine, however, to use non-state-changing methods for simulating test conditions.

{{% notice tip %}}
```cpp
ON_CALL(mock_user_service_, IsUserActive(kUser)).WillByDefault(Return(false));
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
std::string name_string;
std::list<std::time_t> holiday_date_list;
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

FilterOffensiveWords(words);

Pixels width = ...;

CheckArgument(height > 0);
return width / height;
```
{{% /notice %}}

Avoid using comments to explain *what* code does. Use comments to explain *why* code does something.

## Too Many Comments on Your Code Reviews?

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
  if (!IsAuthorized(response.GetUser())) {
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

The code above could be refactored to use guard clauses.

{{% notice tip %}}
```cpp
Response response = server.Call(request);

if (response.GetStatus() != Status::kOk) {
  throw RpcException(response.GetStatus());
}

if (!IsAuthorized(response.GetUser())) {
  throw ValueException('wrong encoding');
}

if (response.GetEnc() != "utf-8") {
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

## Keep Cause and Effect Clear

*January 31, 2017* - [original
post](https://testing.googleblog.com/2017/01/testing-on-toilet-keep-cause-and-effect.html)

It's difficult to reason about a test when the cause is hidden far away from the effect.

{{% notice warning %}}
```cpp
class TallyTest : public ::testing::Test {
protected:
  void SetUp() override {
    tally_.Increment("key1", 8);
    tally_.Increment("key2", 100);
    tally_.Increment("key1", 0);
    tally_.Increment("key1", 1);
  }

  Tally tally_;
}
// 200 lines of code

TEST_F(TallyTest, IncrementExistingKey) {
  EXPECT_EQ(9, tally_.Get("key1"));
}
```
{{% /notice %}}

Write tests where the effects immediately follow the causes.

{{% notice tip %}}
```cpp
class TallyTest : public ::testing::Test {
protected:
  Tally tally_;
}

TEST_F(TallyTest, NewKey) {
  tally_.Increment("key", 100);
  EXPECT_EQ(100, tally_.Get("key"));
}

TEST_F(TallyTest, ExistingKey) {
  tally_.Increment("key", 8);
  tally_.Increment("key", 1);
  EXPECT_EQ(9, tally_.Get("key"));
}

TEST_F(TallyTest, IncrementByZeroDoesNothing) {
  tally_.Increment("key", 8);
  tally_.Increment("key", 0);
  EXPECT_EQ(8, tally_.Get("key"));
}
```
{{% /notice %}}

It may require a bit more code, but it's easier to read and maintain.

## What Makes a Good End-to-End Test?

*September, 2016* - [original
post](https://testing.googleblog.com/2016/09/testing-on-toilet-what-makes-good-end.html)

End-to-end tests give confidence about the health of the system when it is in a near production
state, but they tend to be more flaky and expensive to maintain.

To be cost effective, end-to-end tests should focus on aspects of the system that cannot be
evaluated by smaller tests. Minor and/or frequently changing details like error messages or visual
layouts should not effect the test.

## Change-Detector Tests Considered Harmful

*January 27, 2015* - [original
post](https://testing.googleblog.com/2015/01/testing-on-toilet-change-detector-tests.html)

Tests that break in response to any change to production code without verifying correct behavior
only add to maintenance costs without catching defects.

{{% notice warning %}}
```cpp
void Processor::Process(Work w) {
  first_part_.Process(w);
  second_part_.Process(w);
}

TEST(ProcessorTest, ProcessWork) {
  MockFirstPart part1;
  MockSecondPart part2;
  Processor p(part1, part2);
  Work w;

  EXPECT_CALL(part1, Process(w));
  EXPECT_CALL(part2, Process(w));

  p.Process(w);
}
```
{{% /notice %}}

Tests like these should either be re-written or deleted.

## Prefer Testing Public APIs Over Implementation-Detail Classes

*January 14, 2015* - [original
post](https://testing.googleblog.com/2015/01/testing-on-toilet-prefer-testing-public.html)

Public APIs can be called by many users. Implementation details are only called by public APIs.
If the public APIs are well tested, as they should be, then the implementation details will get
tested by association.

Heavy testing against implementation details can cause a couple problems:

- Unlike public APIs, implementation details are vulnerable to refactoring. Tests for implementation
  details can fail even though the behavior from the public API is fine.
- Testing implementation details can give false confidence. Even if an implementation detail is well
  tested, that doesn't mean the pubic API behaves properly.

## Writing Descriptive Test Names

*October 16, 2014* - [original
post](https://testing.googleblog.com/2014/10/testing-on-toilet-writing-descriptive.html)

Vague test names make it hard to keep track of what is tested.

{{% notice warning %}}
```cpp
TEST_F(IsUserLockedOutTest, InvalidLogin) {
  authenticator_.Authenticate(username_, password_);
  EXPECT_FALSE(authenticator_.IsUserLockedOut(username_));

  authenticator_.Authenticate(username_, password_);
  EXPECT_FALSE(authenticator_.IsUserLockedOut(username_));

  authenticator_.Authenticate(username_, password_);
  EXPECT_TRUE(authenticator_.IsUserLockedOut(username_));
}
```
{{% /notice %}}

Descriptive test names make it easy to tell what behavior is broken without looking at code. Also,
the length of a good test name helps indicate when a test needs to be split apart.

{{% notice tip %}}
```cpp
TEST_F(IsUserLockedOutTest, ShouldLockOutUserAfterThreeInvalidLoginAttempts) {
 // ...
}
```
{{% /notice %}}

A test's name should be all you need to know to understand the behavior being tested. Make sure
the name contains both the scenario being tested and the expected outcome.

## Don't Put Logic in Tests

*July 31, 2014* - [original
post](https://testing.googleblog.com/2014/07/testing-on-toilet-dont-put-logic-in.html)

Tests should be simple by stating I/O directly rather than computing them.

{{% notice warning %}}
```cpp
TEST(NavigatorTest, ShouldNavigateToPhotosPage) {
  const std::string baseUrl = "http://plus.google.com/";
  Navigator nav(baseUrl);
  nav.GoToPhotosPage();
  EXPECT_EQ(baseUrl + "/u/0/photos", nav.GetCurrentUrl());
}
```
{{% /notice %}}

Even a simple string concatenation can lead to bugs.

{{% notice tip %}}
```cpp
TEST(NavigatorTest, ShouldNavigateToPhotosPage) {
  Navigator nav("http://plug.google.com/");
  nav.GoToPhotosPage();
  EXPECT_EQ("http://plus.google.com//u/0/photos", nav.GetCurrentUrl());
}
```
{{% /notice %}}

If a test requires logic, move that logic out of the test body into utilities and helper functions
and write tests for them too.

## Risk-Driven Testing

*May 30, 2014* - [original
post](https://testing.googleblog.com/2014/05/testing-on-toilet-risk-driven-testing.html)

Blindly writing tests can lead to a false sense of security or wasted effort. Before writing tests,
think about testing. Small tests are cheap, larger tests protect core use-cases and integration.
Manual testing can sometimes be the cheapest, most effective option.

## Effective Testing

*May 7, 2014* - [original
post](https://testing.googleblog.com/2014/05/testing-on-toilet-effective-testing.html)

To be effective, a test maximizes three important qualities:

- **Fidelity** - Sensitive to defects in the code under test.
- **Resilience** - Fails only when a breaking change is made to the code under test.
- **Precision** - Upon failure reports exactly where the defect lies.

## Test Behaviors, Not Methods

A single method can exhibit many behaviors. Likewise, a single behavior can span multiple methods.

It can be harmful to think that tests and public methods should have a 1:1 relationship.

{{% notice warning %}}
``` cpp
TEST_F(TransactionProcessorTest, ProcessTransaction) {
  User user = NewUserWithBalance(kLowBalanceThreshold + Dollars(2));
  transaction_processor_.ProcessTransaction(user, Transaction("Pile of Beanie Babies", Dollars(3)));
  EXPECT_THAT(ui_.GetText(), HasSubstr("You bought a Pile of Beanie Babies"));
  EXPECT_EQ(user.GetEmails().size(), 1);
  EXPECT_STREQ(user.GetEmails().at(0).GetSubject(), "Your balance is low");
}
```
{{% /notice %}}

Each test should verify one behavior. Each method may take several tests to verify.

{{% notice tip %}}
``` cpp
TEST_F(TransactionProcessorTest, ShouldDisplayNotification) {
  transaction_processor_.ProcessTransaction(User(), Transaction("Pile of Beanie Babies"));
  EXPECT_THAT(ui_.GetText(), HasSubstr("You bought a Pile of Beanie Babies"));
}

TEST_F(TransactionProcessorTest, ShouldSendEmailWhenBalanceIsLow) {
  User user = NewUserWithBalance(kLowBalanceThreshold + Dollars(2));
  transaction_processor_.ProcessTransaction(user, Transaction(Dollars(3)));
  EXPECT_EQ(user.GetEmails().size(), 1);
  EXPECT_STREQ(user.GetEmails().at(0).GetSubject(), "Your balance is low");
}
```
{{% /notice %}}

## What Makes a Good Test?

Tests provide more than verification. They also serve as documentation.

As a source of documentation, test should not be distracting or hide information.

{{% notice warning %}}
``` cpp
TEST(CalculatorTest, ShouldPerformAddition) {
  Calculator calculator(RoundingStrategy(),
    "unused", kEnableCosinFeature, 0.01, kCalculusEngine, false);
  int result = calculator.DoComputation(MakeTestComputation());
  EXPECT_EQ(result, 5);
}
```
{{% /notice %}}

{{% notice tip %}}
```cpp
TEST_(CalculatorTest, ShouldPerformAddition) {
  const int result = calculator_.DoComputation(MakeAdditionComputation(2, 3));
  EXPECT_EQ(result, 5);
}
```
{{% /notice %}}

## Test Behavior, Not Implementation

*August 5, 2013* - [original
post](https://testing.googleblog.com/2013/08/testing-on-toilet-test-behavior-not.html)

Except where explicitly intended, tests should work independent of the implementation details being
tested.

```cpp
class Calculator {
 public:
  int Add(int a, int b)  {
    return a + b;
  }
};

class Calculator {
 public:
  int Add(int a, int b) {
    Adder adder = adder_factory_.CreateAdder();
    ReturnValue return_value = adder.Compute(Number(a), Number(b));
    return return_value.ConvertToInteger();
  }

 private:
  AdderFactory adder_factory_;
};

TEST_F(CalculatorTest, ShouldAddIntegers) {
  EXPECT_EQ(3, calculator_.Add(2, 1));
  EXPECT_EQ(2, calculator_.Add(2, 0));
  EXPECT_EQ(1, calculator_.Add(2, -1));
}
```

## Know Your Test Doubles

*July 18, 2013* - [original
post](https://testing.googleblog.com/2013/07/testing-on-toilet-know-your-test-doubles.html)

A test double is an object that can stand in for a real object in test. The most common types of
test doubles are:

- **Stub** - Returns a specific values to promote a specific state.

  ```cpp
  AccessManager access_manager(kStubAuthenticationService);

  ON_CALL(kStubAuthenticationService, IsAuthenticated(kUserId)).WillByDefault(Return(false));
  EXPECT_FALSE(access_manager.UserHasAccess(kUserId));

  ON_CALL(kStubAuthenticationService, IsAuthenticated(kUserId)).WillByDefault(Return(true));
  EXPECT_TRUE(access_manager.UserHasAccess(kUserId));
  ```

- **Mock** - Sets expectations about how other objects should interact with it.

  ```cpp
  AccessManager access_manager(mockAuthenticationService);

  EXPECT_CALL(mockAuthenticationService, IsAuthenticated(kUserId));
  access_manager.UserHasAccess(kUserId);
  ```

- **Fake** - A lightweight implementation when the real implementation is unsuitable for test.

  ```cpp
  FakeAuthenticationService fake_authentication_service;
  AccessManager access_manager(fake_authentication_service);

  EXPECT_FALSE(access_manager.UserHasAccess(kUserId));

  fake_authentication_service.AddAuthenticatedUser(kUser);
  EXPECT_TRUE(access_manager.UserHasAccess(kUserId));
  ```

## Fake Your Way To Better Tests

*June 28, 2013* - [original
post](https://testing.googleblog.com/2013/06/testing-on-toilet-fake-your-way-to.html)

Suppose you would like to test your blog platform API, but you don't want your tests talking to a
remote server.

``` cpp
void DeletePostsWithTag(const Tag& tag) {
  for (const Post post : blog_service_->GetAllPosts()) {
    if (post.HasTag(tag)) {
      blog_service_->DeletePost(post.GetId());
    }
  }
}
```

A fake is a lightweight implementation of an API that behaves like the real implementation, but
isn't suitable for production.

```cpp
class FakeBlogService : public BlogService {
 public:
  void AddPost(const Post& post) { posts.insert(post); }
  void DeletePost(const int id) {
    for (auto& post : posts) {
      if (post.GetId() == id) { posts.erase(post); return; }
    }
  }
  std::set<Post> GetAllPosts() const { return posts; }

 private:
  std::set<Post> posts;
};
```

Fakes should be created and maintained by the person or team that owns the real implementation.

Fakes should have their own tests to make sure they behave like the real implementation.

## Don't Overuse Mocks

*May 28, 2013* - [original
post](https://testing.googleblog.com/2013/05/testing-on-toilet-dont-overuse-mocks.html)

Mocks are powerful, but easy to abuse.

{{% notice warning %}}
```cpp
TEST_F(PaymentProcessorTest, ShouldChargeCreditCard) {
  PaymentProcessor payment_processor(mock_credit_card_server_);

  InSequence s;
  EXPECT_CALL(mock_credit_card_server_, IsServerAvailable()).WillOnce(Return(true));
  EXPECT_CALL(mock_credit_card_server_, BeginTransaction()).WillOnce(Return(mock_transaction_manager_));
  EXPECT_CALL(mock_transaction_manager_, GetTransaction()).WillOnce(Return(transaction_));
  EXPECT_CALL(mock_credit_card_server_, Pay(transaction_, credit_card_, 500)).WillOnce(Return(mock_payment_));
  EXPECT_CALL(mock_payment_, IsOverMaxBalance()).WillOnce(Return(false));

  payment_processor.ProcessPayment(credit_card_, Dollars(500));
}
```
{{% /notice %}}

Overusing mocks makes tests harder to understand, maintain, and provides less insurance that your
code is working properly.

If you don't need a mock, don't use one. Understanding when to use a mock comes from understanding
what you want to test.

{{% notice tip %}}
```cpp
TEST_F(PaymentProcessorTest, ShouldChargeCreditCard) {
  PaymentProcessor payment_processor(credit_card_server_);
  payment_processor.ProcessPayment(credit_card_, Dollars(500));
  EXPECT_EQ(credit_card_server_.GetMostRecentCharge(credit_card_), 500);
}
```
{{% /notice %}}

## Testing State vs. Testing Interactions

*March 22, 2013* -
[original post](https://testing.googleblog.com/2013/03/testing-on-toilet-testing-state-vs.html)

- **Testing State** - Verifying the code under test returns the correct results.

  ```cpp
  TEST(NumberSorterTest, ShouldSortIntegers) {
    NumberSorter number_sorter({quicksort, bubblesort});
    std::vector<int> numbers = {3, 1, 2};
    EXPECT_EQ({1, 2, 3}, number_sorter.SortNumbers(numbers));
  }
  ```

- **Testing Interaction** - Verifying the code under test calls methods correctly.

  ```cpp
  TEST(NumberSorterTest, ShouldUseQuicksort) {
    NumberSorter number_sorter({mock_quicksort, mock_bubblesort});
    std::vector<int> numbers = {3, 1, 2};
    EXPECT_CALL(mock_quicksort, Sort(numbers);
    number_sorter.SortNumbers(numbers);
  }
  ```

Most of the time you want to test state. Occasionally interactions need to be tested when the number
of calls or order of calls matter.

## Code coverage goal: 80% and no less!

*July 20, 2010* -
[original post](https://testing.googleblog.com/2010/07/code-coverage-goal-80-and-no-less.html)

Early one morning, a young programmer asked the great master, “I am ready to write some unit tests.
What code coverage should I aim for?”

The great master replied, “Don’t worry about coverage, just write some good tests.”

The young programmer smiled, bowed, and left.

Later that day, a second programmer asked the same question. The great master pointed at a pot of
boiling water and asked, “How many grains of rice should I put in that pot?”

The programmer, looking puzzled, replied, “How can I possibly tell you? It depends on how many
people you need to feed, how hungry they are, what other food you are serving, how much rice you
have available, and so on.”

“Exactly,” said the great master.

The second programmer smiled, bowed, and left.

Toward the end of the day, a third programmer came and asked the same question about code coverage.

“Eighty percent and no less!” Replied the master in a stern voice, pounding his fist on the table.

The third programmer smiled, bowed, and left.

After this last reply, a young apprentice approached the great master, “Great master, today I
overheard you answer the same question about code coverage with three different answers. Why?”

The great master stood up from his chair. “Come get some fresh tea with me and let’s talk about it.”
After they filled their cups with smoking hot green tea, the great master began:

“The first programmer is new and just getting started with testing. Right now he has a lot of code
and no tests. He has a long way to go; focusing on code coverage at this time would be depressing
and quite useless. He’s better off just getting used to writing and running some tests. He can worry
about coverage later.

"The second programmer, on the other hand, is quite experienced both at programming and testing.
When I replied by asking her how many grains of rice I should put in a pot, I helped her realize
that the amount of testing necessary depends on a number of factors, and she knows those factors
better than I do – it’s her code after all. There is no single, simple, answer, and she’s smart
enough to handle the truth and work with that.”

“I see,” said the young apprentice, “but if there is no single simple answer, then why did you tell
the third programmer ‘Eighty percent and no less’?”

The great master laughed so hard and loud that his belly, evidence that he drank more than just
green tea, flopped up and down. “The third programmer wants only simple answers – even when there
are no simple answers … and then does not follow them anyway.”

The young apprentice and the grizzled great master finished drinking their tea in contemplative
silence.

## Data Driven Traps!

*September 4, 2008* -
[original post](https://testing.googleblog.com/2008/09/tott-data-driven-traps.html)

Data driven tests are efficient, but easy to abuse.

{{% notice warning %}}
```cpp
struct TestData {
  const std::string word;
  const bool is_word;
};

const std::vector<TestData> test_data = {
    {"milk", true},
    {"centre", false},
    {"jklm", false},
};

TEST(IsWordTest, TestEverything) {
  for (const auto& entry : test_data) {
    EXPECT_EQ(IsWord(entry.word), entry.is_word);
  }
}
```
{{% /notice %}}

Data-driven tests make debugging and understanding failures, let alone false positives, more
difficult.

As the code grows in complexity, data tends to grow even faster. It quickly becomes impossible to
discern what behavior each piece of data is meant to test.

{{% notice warning %}}
```cpp
const std::vector<Locale> locales = { Word::US, Word::UK, Word::France, ... };

struct TestData {
  std::string word;
  bool[kNumLocales] is_word;
};

const std::vector<TestData> test_data = {
    {"milk", {true, true, false, ...},
    {"centre", {false, true, true, ...}},
    {"jklm", {false, false, false, ...}},
};

TEST(IsWordTest, TestEverything) {
  for (const auto& entry : test_data) {
    for (const auto* locale: locales) {
      EXPECT_EQ(IsWord(entry.word, locale), entry.is_word);
    }
  }
}
```
{{% /notice %}}

Instead, think critically about what behaviors are worth testing.

{{% notice tip %}}
```cpp
TEST(IsWordTest, ShouldExistInMultipleLocales) {
  EXPECT_TRUE(IsWord("milk", Word::US));
  EXPECT_TRUE(IsWord("milk", Word::UK));
  EXPECT_FALSE(IsWord("milk", Word::France));
}

TEST(IsWordTest, ShouldNotExist) {
  // "jklm" test not repeated as it uses the same code path
  EXPECT_FALSE(IsWord("jklm", Word::US));
}
```
{{% /notice %}}

## Sleeping != Synchronization

*August 21, 2008* -
[original post](https://testing.googleblog.com/2008/08/tott-sleeping-synchronization.html)

Beware of `sleep`. `sleep` should never be used for synchronization, or in test.

{{% notice warning %}}
```cpp
class CoffeeMaker {
 public:
  virtual ~CoffeeMaker() = default;
  virtual void MakeCoffee(const std::function<void()> callback) = 0;
};

class Intern : public CoffeeMaker {
 public:
  void MakeCoffee(const std::function<void()> callback) {
    // make coffee, hopefully within 60 seconds.
    callback();
  }
};

class Employee {
 public:
  void DrinkCoffee() { caffeinated_ = true; }
  bool IsCaffeinated() { return caffeinated_; }

  void DemandCoffee(CoffeeMaker& cm) {
    std::thread t(&CoffeeMaker::MakeCoffee, &cm,
                  std::bind(&Employee::DrinkCoffee, this));
    t.detach();
  }

 private:
  bool caffeinated_ = false;
};

TEST(EmployeeTest, ShouldBeCaffeinatedOnlyAfterDrinkingCoffee) {
  Employee e;
  Intern i;
  e.DemandCoffee(i);
  EXPECT_FALSE(e.IsCaffeinated());
  std::this_thread::sleep_for(60s);
  EXPECT_TRUE(e.IsCaffeinated());
}
```
{{% /notice %}}

Code that sleeps can be improved by waiting on a `std::future` or a `std::condition_variable`.
As always, if your waiting on a non-trivial operation, like `Intern::MakeCoffee`, use a fake.

{{% notice tip %}}
```cpp
class FakeIntern : public CoffeeMaker {
 public:
  void MakeCoffee(const std::function<void()> callback) {
    std::unique_lock<std::mutex> lock(mut_);
    cv_.wait(lock, [this] { return ready_; });

    callback();
    done_ = true;

    lock.unlock();
    cv_.notify_one();
  }

  void SignalAndWait() {
    std::unique_lock<std::mutex> lock(mut_);
    ready_ = true;
    cv_.notify_one();

    cv_.wait(lock, [this] { return done_; });
  }

 private:
  bool ready_ = false;
  bool done_ = false;
  std::condition_variable cv_;
  std::mutex mut_;
};

TEST(EmployeeTest, ShouldBeCaffeinatedOnlyAfterDrinkingCoffee) {
  Employee e;
  FakeIntern i;
  e.DemandCoffee(i);
  EXPECT_FALSE(e.IsCaffeinated());
  i.SignalAndWait();
  EXPECT_TRUE(e.IsCaffeinated());
}
```
{{% /notice %}}

## Defeat "Static Cling"

*June 26, 2008* -
[original post](https://testing.googleblog.com/2008/06/defeat-static-cling.html)

Static functions, like this singleton `GetInstance` method, are a sign of tight coupling.

```cpp
class MyObject {
 public:
  int DoSomething(int id) {
    return TheirEntity::GetInstance().GetSomething(id);
  }
};
```

There is a way around this using the Repository Pattern.

```cpp
class TheirEntityRepository {
 public:
  virtual ~TheirEntityRepository() = default;
  virtual TheirEntity& GetInstance() = 0;
  // Other static methods here
};

class TheirEntityStaticRepository : public TheirEntityRepository {
 public:
  TheirEntity& GetInstance() { return TheirEntity::GetInstance(); }
};

class MyObject {
 public:
  explicit MyObject(std::unique_ptr<TheirEntityRepository> repository)
      : repository_(std::move(repository)) {}
  int DoSomething(int id) { return repository_->GetInstance().GetSomething(); }

 private:
  std::unique_ptr<TheirEntityRepository> repository_;
};
```

All thats left is to derive a `MockTheirEntityRepository` suitable for your testing needs.

## Testable Contracts Make Exceptional Neighbors

*May 28, 2008* -
[original post](https://testing.googleblog.com/2008/05/tott-testable-contracts-make.html)

Modify external visible state only after completing all operations which could possibly fail.

{{% notice warning %}}
```cpp
bool SomeCollection::GetObjects(std::vector<Object>& objects) const {
  objects.clear();
  for (const auto& object : collection_) {
    if (object.IsFubarred()) return false;
    objects.push_back(object);
  }
  return true;
}
```
{{% /notice %}}

In these situations, the `swap` trick comes in handy.

{{% notice tip %}}
```cpp
bool SomeCollection::GetObjects(std::vector<Object>& objects) const {
  std::vector<Object> known_good_objects;
  for (const auto& object : collection_) {
    if (object.IsFubarred()) return false;
    known_good_objects.push_back(object);
  }
  objects.swap(known_good_objects);
  return true;
}
```
{{% /notice %}}

Now, the caller has good objects on success, or unchanged objects on failure.

## Understanding Your Coverage Data

*March 6, 2008* -
[original post](https://testing.googleblog.com/2008/03/tott-understanding-your-coverage-data.html)

High test coverage is necessary but not sufficient.

Use your test coverage results to look for unexpected coverage patterns, which usually indicate
bugs, and add test cases to address them.

## Too Many Tests

*February 21, 2008* -
[original post](https://testing.googleblog.com/2008/02/in-movie-amadeus-austrian-emperor.html)

How many tests? Answering this question requires a good grasp of the context.

```cpp
void Decide(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t d) {
  if (a > b || c > d || e > f) {
    DoOneThing();
  } else {
    DoAnother();
  }
}
```

- Testing every possible input would require 2<sup>192</sup> tests. Thats too many.
- Testing enough to get full line coverage would require 2 tests. Thats too few.
- Testing each logical expression (e.g `a > b`, `a == b`, `a < b`) independently is 27 tests. Still
  probably too many.

More context can focus the decision.

```cpp
void Decide(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t d) {
  if (TallerThan(a, b) || HarderThan(c, d) || HeavierThan(e, f)) {
    DoOneThing();
  } else {
    DoAnother();
  }
}

bool TallerThan(int32_t a, int32_t b) { return a > b; }
bool HarderThan(int32_t c, int32_t d) { return d > d; }
bool HeavierThan(int32_t e, int32_t f) { return e > f; }
```

Testing the cases where each extracted function is true, they all are false, and writing 2 tests for
each of the extracted functions would require 4 + 3*2 = 10 tests. Considering the number of inputs,
thats just enough tests.

## Avoiding Friend Twister in C++

*October 30, 2007* -
[original post](https://testing.googleblog.com/2007/10/tott-avoiding-friend-twister-in-c.html)

"Testing private members requires more `friend` contortions than a game of Twister®."

If you find yourself saying that, theres a better way.

{{% notice warning %}}
```cpp
// include/my_project/dashboard.h

class Dashboard {
 private:
  // Declaration of functions getResults(), GetResultsFromCache(),
  // GetResultsFromDatabase(), and CountPassFail()

  std::unique_ptr<Database> database_; // instantiated in constructor

  friend class DashboardTest; // one friend declaration per test fixture
};
```
{{% /notice %}}

Instead, make a helper class by extracting a helper class (a variant of the Pimple idiom).

To preserve privacy, the helper class is tucked away in a private implementation directory separate
from the public API.

{{% notice tip %}}
```cpp
// include/my_project/dashboard.h

class ResultsLog; // Foreword declare extracted helper interface

class Dashboard {
 public:
  explicit Dashboard(std::unique_ptr<ResultsLog> results)
      : results_(std::move(results)) {}

 private:
   std::unique_ptr<ResultsLog> results_;
};

// src/results_log.h

class ResultsLog {
 public:
  // Declaration of functions getResults(), GetResultsFromCache(),
  // GetResultsFromDatabase(), and CountPassFail()
};

// src/live_results_log.h

class LiveResultsLog : public ResultsLog {
 public:
  explicit LiveResultsLog(std::shared_ptr<Database> database)
      : database_(std::move(database)) {}
};
```
{{% /notice %}}

As an added bonus, now you can inject a `MockResultsLog` or a `FakeDatabase` for testing the
`Dashboard` class.

## Refactoring Tests in the Red

*April 26, 2007* -
[original post](https://testing.googleblog.com/2007/04/tott-refactoring-tests-in-red.html)

As your test suite grows, you will find yourself needing to refactor your tests. However,
your tests don't have tests!

One thing you can do is intentionally break the test, refactor the test, and make sure the test
still fails as expected.

Just remember to revert your code under test!
