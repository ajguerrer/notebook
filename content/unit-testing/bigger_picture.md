---
title: "The Bigger Picture"
weight: 1
---

## What Makes a Good or Bad Test?

- Unit testing enables *sustainable* growth in software projects by acting as a form of insurance 
  against regressions. Good tests outweigh the cost of their upkeep with the cost of the bugs they 
  prevent.
- Code is a liability, not an asset, and tests are code too. A test that in addition to raising 
  false alarms, is slow and difficult to maintain, can do more harm than good.
- Testability works well as a negative indicator for good code design by pointing out 
  *tight coupling*.  However, the reverse is not true. Too much decoupling can lead to disaster.

## Code Coverage Metrics

Coverage metrics work well as a negative indicator for test quality, but a bad positive one.

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

3/4 = 75% of the lines are covered. Only three lines are important, but curly braces count.

```cpp
bool IsStringLong(const std::string& input) { return input.length() > 5; }
```

Refactoring is a handy tool to "game" coverage. Now, 1/1 = 100% of the lines are covered with no 
change to the test.

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

Even with 100% coverage, that says nothing about the quality of the test.

```cpp
int Parse(const std::string& input) { return std::atoi(input.c_str()); }

TEST(IsStringLong, ShortString) {
  int result = Parse("5");
  EXPECT_EQ(result, 5);
}
```

Coverage does not factor in external libraries. Though nowhere close to being exhaustive, this 
test has 100% coverage.

{{% notice tip %}}
**Hospital Analogy** - Temperature is another good negative indicator, but a bad positive indicator. 
Nurses may take temperature measurements to see if a patient's body is unwell. But, regulating the 
patient's temperature is never *the goal*. Otherwise, hospitals would tend towards more "effective" 
strategies, like dumping patients with fevers into ice baths. Instead, knowing that temperature 
will normalize as a consequence of the patient feeling better, nurses continuously monitor while 
focusing on the root cause of the illness.  
{{% /notice %}}

## What Makes a Successful Test Suite

A successful test suite has the following properties:

1. Integrated into the development cycle.
1. Targets only the most important parts of the code base: the business logic.
1. Provides maximum value with minimum maintenance costs.

Enforcing test quality is a tough problem; one that requires personal judgement.
