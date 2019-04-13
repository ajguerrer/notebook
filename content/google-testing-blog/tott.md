---
title: "Testing on the Toilet"
---

## Exercise Service Call Contracts in Tests

*November 27, 2018* -
[ref](https://testing.googleblog.com/2018/11/testing-on-toilet-exercise-service-call.html)

If the code under test relies on the contract of a service, prefer exercising the service call
instead of mocking it out. Some service owners provide a fake. Otherwise, use a hermetic server.

## Make Interfaces Hard to Misuse

*July 25, 2018* -
[ref](https://testing.googleblog.com/2018/07/code-health-make-interfaces-hard-to.html)

Don't push the responsibility of maintaining invariants on the caller.

```cpp
class Vector {
  explicit Vector(int num_slots);
  int RemainingSlots() const;
  void AddSlots(int num_slots);
  void Insert(int value);
};
```

Caller needs to check `RemainingSlots` and if 0, `AddSlots` in order for `Insert` to work properly.

`Insert` could automatically manage slots.

```cpp
class Vector {
  explicit Vector(int num_slots);
  void Insert(int value);
};
```

Other examples:

- `Initialize` / `Deitialize` functions.
- Allowing partially created objects.
- Parameters that can have invalid values.

Sometimes it's not practical to have a foolproof interface. In those cases rely on static analysis
or documentation.

## Only Verify Relevant Method Arguments

*June 26, 2018* -
[ref](https://testing.googleblog.com/2018/06/testing-on-toilet-only-verify-relevant.html)

Tests become fragile when they expect exact values on all the arguments.

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

Only verify one behavior per test. Only verify arguments that affect the correctness of the specific
behavior being tested.

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

## Keep Tests Focused

*June 11, 2018* -
[ref](https://testing.googleblog.com/2018/06/testing-on-toilet-keep-tests-focused.html)

Testing too many scenarios at once can make it difficult to understand test and reason about the
failure.

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

Break up each scenario into its own test.

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

Notice that each test only asserts on the output of one call each test.


