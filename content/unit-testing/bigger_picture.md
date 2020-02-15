---
title: "The Bigger Picture"
weight: 1
---

## What Makes a Good or Bad Test?

- Unit testing enables *sustainable* growth in software projects by acting as a form of insurance 
  against regressions. Good tests outweigh their maintenance cost with the cost of the bugs they 
  prevent.
- Code is a liability, not an asset, and tests are code too. Tests that don't verify business value, 
  raise false alarms, run slow, and are difficult to maintain, do more harm than good.

## Code Coverage Metrics

Coverage metrics work well as a negative indicator for test quality, but a bad positive one.

#### Curly Braces

Curly braces count. 75% of 4 lines covered.

```cpp
bool IsStringLong(const std::string& input) {
  if (input.length() > 5) {
    return true;
  }

  return false;
}

TEST(IsStringLong, ShortString) {
  bool result = IsStringLong("abc");
  EXPECT_EQ(result, false);
}
```

#### Refactoring

Refactoring can be used to "game" coverage by condensing the code. 100% of 1 line is covered, with
no change to behavior.

```cpp
bool IsStringLong(const std::string& input) { return input.length() > 5; }
```

#### Test Quality

100% of 4 lines are covered, for what?

```cpp
bool IsStringLong(const std::string& input) {
  if (input.length() > 5) {
    return true;
  }

  return false;
}

TEST(IsStringLong, ShortString) {
  bool result1 = IsStringLong("abc");
  bool result2 = IsStringLong("abcdef");
}
```

#### External Dependencies

Coverage metrics do not include external libraries. Though nowhere close to being exhaustive, 100% 
of 1 line is covered.

```cpp
int Parse(const std::string& input) { return std::atoi(input.c_str()); }

TEST(IsStringLong, ShortString) {
  int result = Parse("5");
  EXPECT_EQ(result, 5);
}
```



{{% notice tip %}}
**Hospital Analogy** - Temperature is another good negative indicator, but a bad positive indicator. 
Nurses take temperature measurements to see if a patient's body is unwell. But, regulating the 
patient's temperature is never *the goal*. Otherwise, hospitals would tend towards more "effective" 
strategies, like dumping patients with fevers into ice baths. Instead, nurses continuously monitor 
a patients temperature while focusing on the root cause of the illness.  
{{% /notice %}}

## What Makes a Successful Test Suite

A successful test suite has the following properties:

1. Integrated into the development cycle.
1. Targets only the most important parts of the code base: the domain logic.
1. Provides maximum value with minimum maintenance costs.

Enforcing test quality is a tough problem; one that requires personal judgement.

## Test Dependencies

Dependencies may be shared or private, and mutable or immutable.

- **Private** - Lifetime is managed by the test; includes local variables.
- **Shared** -  Lifetime is not managed by the test; includes global variables and out-of-process 
  state.
- **Immutable** - State never changes over the course of the test.
- **Mutable** - State may change over the course of the test.

## Test Isolation

Varying degrees of isolation can be used when writing tests:

- **Heavy Isolation** - Replace all mutable dependencies with test doubles. Development tends to
  start at the top of the object graph and work it's way down.
- **Light Isolation** - Replace all mutable, shared dependencies with test doubles. Development 
  tends to start at the bottom of the object graph and work it's way up.

#### Granularity

Heavy isolation helps increase granularity. However, it is easy to fall into the trap of testing
*code* instead of *behavior*. 

{{% notice warning %}}
```
When I call my dog, he moves his front left leg, followed by his front right leg, then the back 
right leg, his head turns, and the tail starts wagging.
```
{{% /notice %}}

What is the purpose of those movements?

{{% notice tip %}}
```
When I call my dog, he comes right to me.
```
{{% /notice %}}

A test should tell a story about the problem your code helps to solve, and this story should be 
cohesive and meaningful to a business expert.

#### Coupling

Heavy isolation can make it easier to test coupled classes. Substituting a classes dependencies with
test doubles can help break up coupling in a large graph of dependencies. However, dependance on 
test doubles is a good negative indicator of quality code design.

#### Feedback

Heavy isolation makes it easier to narrow down bugs, while light isolation tends to produce 
cascades of failing tests. However, this issue can be mitigated by running tests regularly.

## Test Structure

- *Arrange*, *Act*, then *Assert* (AAA).
- Watch out for *Act* and *Assert* sections larger than one line, which may indicate: 
  - Test verifies more than one behavior and needs to be split.
  - Multiple outcomes need checking, a.k.a *invariant violation*.
- *Arrange* sections should not be placed inside test fixtures. Doing so:
  - Introduces coupling between tests.
  - Decreases readability.
- Simplify tedious *Arrange* sections with the factory pattern.
- Avoid using conditional logic, including loop conditions.
- Name tests using plain english, describing behavior from the perspective of a business expert.
- Tests that verify the same behavior with different values may be parameterized, but never at the
  expense of readability.
- Consider an assertion library that reads fluently.
