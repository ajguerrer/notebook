---
title: "A First Unit Test"
pre: "<b>2. </b>"
weight: 2
---

## The LogAn project

*LogAn* is the scenario project used throughout the book. LogAn is a log analyzer for logs written 
in the company proprietary format. If LogAn finds a special, an alert is sent to the appropriate
parties.

## First test

Lets start by testing `IsValidFileName`.

```cpp
bool LogAnalyzer::IsValidLogFileName(const std::string& fileName) {
  if (!EndsWith(fileName, ".SLF")) {
    return false;
  }

  return true;
}

bool LogAnalyzer::EndsWith(const std::string& fileName,
                           const std::string& suffix) {
  return std::equal(suffix.rbegin(), suffix.rend(), fileName.rbegin(),
                    [](const char a, const char b) {
                      return std::tolower(a) == std::tolower(b);
                    });
}
```

### Recipe for a test

Unit test follow a general recipe:

1. Setup
2. Exercise
3. Expect
4. Teardown

Unit tests should not be random; each test captures a meaningful scenario. That scenario and it's 
expected outcome is expressed explicitly in the name of the test.

```cpp
TEST(LogAnTest, WrongExtensionIsInvalid) {
  LogAnalyzer analyzer;
  bool result = analyzer.IsValidLogFileName("filewithbadextension.foo");
  EXPECT_FALSE(result);
}

TEST(LogAnTest, LowercaseExtensionIsValid) {
  LogAnalyzer analyzer;
  bool result = analyzer.IsValidLogFileName("filewithgoodextension.slf");
  EXPECT_TRUE(result);
}

TEST(LogAnTest, UppercaseExtensionIsValid) {
  LogAnalyzer analyzer;
  bool result = analyzer.IsValidLogFileName("filewithgoodextension.SLF");
  EXPECT_TRUE(result);
}
```

### Parameterized tests

Tests can get repetitive. When only the parameters change, consider parameterized tests. Be careful,
 doing so can reduce readability.

```cpp
using ::testing::TestWithParam;
using ::testing::ValuesIn;

class LogAnTest : public TestWithParam<std::tuple<std::string, bool>> {
 protected:
  void SetUp() override { std::tie(fileName_, valid_) = GetParam(); }
  
  LogAnalyzer analyzer_;
  std::string fileName_;
  bool valid_;
};

TEST_P(LogAnTest, VariousExtensions) {
  bool result = analyzer_.IsValidLogFileName(fileName_);
  EXPECT_EQ(valid_, result);
}

const std::tuple<std::string, bool> params[] = {
    {"filewithbadextension.foo", false},
    {"filewithgoodextension.slf", true},
    {"filewithgoodextension.SLF", true}};

INSTANTIATE_TEST_SUITE_P(ExtensionParams, LogAnTest, ValuesIn(params));
```

### Exceptions

Suppose calling `IsValidLogFileName` with a empty filename throws an `exception`.

``` cpp
bool LogAnalyzer::IsValidLogFileName(const std::string& fileName) {
  if (fileName.empty()) {
    throw std::invalid_argument("Filename cannot be empty");
  }
  // ...
}
```

Unit tests can also verify that a statement `throw`s (or doesn't `throw`) an `exception`.

```cpp
TEST(LogAnTest, EmptyFilenameThrowsInvalidArgument) {
  LogAnalyzer analyzer;
  EXPECT_THROW(analyzer.IsValidLogFileName(""), std::invalid_argument);
}
```