---
title: "Making your Tests Work for You"
weight: 2
---

## Four Pillars of a Good Test

1. **Protection against regressions** - Ability to indicate the presence of regressions.
1. **Resistance to refactoring** - Degree to which a test can sustain refactoring without producing
  a false positive.
1. **Fast feedback** - Measure of how quickly the test executes.
1. **Maintainability** - Ability to read and run the test.

<table>
  <tr>
    <th>Error Types</th>
    <th>Functionality is Correct</th>
    <th>Functionality is Broken</th>
  </tr>
  <tr>
    <th>Test Passes</th>
    <td>True negative</td>
    <td>False negative (indicates poor protection against regressions)</td>
  </tr>
  <tr>
    <th>Test Fails</th>
    <td>False positive (indicates poor resistance to refactoring)</td>
    <td>True positive</td>
  </tr>
</table>

$$Test\ accuracy = {Signal\ (True\ positives) \over Noise\ (False\ positives)}$$

## The Ideal Test

{*Protection against regressions*, *Resistance to refactoring*, *Fast feedback*} - Choose two. 

![Ideal test](/images/ideal_test.png)

Always chose to maximize *resistance to refactoring*. Then, test size becomes a slider between 
*protection against regressions* and *fast feedback*.

![Test triangle](/images/test_triangle.png)

A diverse ratio of test sizes provides an ideal amount of both *protection against regressions* and 
*fast feedback*. For any non-trivial production system, test counts should form a pyramid where the
test count shrinks as test size grows. 

![Test pyramid](/images/test_pyramid.png)

## Preventing False Positives

When tests fail for the wrong reasons, developers quickly loose trust in their tests. However, 
without trustworthy tests, refactoring is risky. This trap leads to real bugs slipping through.

Fast positives are formed from coupling between the test and implementation details:

{{% notice warning %}}
```cpp
TEST(Render, IsCorrect) {
  std::ifstream in("message_renderer.cc");
  std::stringstream sstr;
  sstr << in.rdbuf();

  EXPECT_EQ(sstr.str(), R"(#include "message_renderer.h"
#include "body_renderer.h"
#include "title_renderer.h"
#include "footer_renderer.h"

#include <memory>
#include <numeric>
#include <string>

MessageRenderer::MessageRenderer()
    : sub_renderers_({std::make_unique<TitleRenderer>(),
                      std::make_unique<BodyRenderer>(),
                      std::make_unique<FooterRenderer>()}) {}

std::string MessageRenderer::Render(const Message& message) {
  return std::accumulate(sub_renderers_.begin(), sub_renderers_.end(),
                         std::string{},
                         [&](std::string m, std::shared_ptr<Renderer> r) {
                           return std::move(m) + r->Render(message);
                         });
})");
}
```
{{% /notice %}}

Instead of testing the code, test the behavior.

{{% notice tip %}}
```cpp
TEST(Render, BasicMessage) {
  MessageRenderer renderer;
  const Message message{title : "a", body : "b", footer : "c"};

  std::string html = renderer.Render(message);

  EXPECT_EQ(html,
            "<head><title>a</title></head><body>b</body><footer>c</footer>");
}
```
{{% /notice %}}

## Types of Test Doubles

- **Mock** *(mock, spy)* - Emulate and verify *outgoing* interactions from the SUT.
- **Stub** *(stub, dummy, fake)* - Emulate *incoming* interactions to the SUT.

{{% notice note %}}
The term *mock* can also refer to the mocking framework itself. Mocking frameworks are also 
generally responsible for creating stubs which can be confusing.
{{% /notice %}}

Mocks can emulate interactions like stubs, but stubs should never assert interactions like mocks. 
Asserting interactions with stubs is *over-specification*, a common anti-pattern.

## Encapsulation

A well-designed API hides all implementation details behind a private API, leaving only observable 
behavior in the public API. Implementation details should never *leak* into the public API.

#### Invariant violation

API's that require several steps to achieve an individual goal are prone to *invariant violation*.

{{% notice warning %}}
```cpp
class User {
 public:
  void SetName(const std::string& name) { name_ = name; }

  std::string NormalizeName(const std::string& name) {
    return std::string(absl::StripAsciiWhitespace(name)).substr(50);
  }

 private:
  std::string name_;
};
{{% /notice %}}

Client must remember to `NormalizeName` before `SetName`.

{{% notice warning %}}
```cpp
std::string normalized_name = user.NormalizeName(new_name);
user.SetName(normalized_name);
```
{{% /notice %}}

A client should be able to achieve any individual goal atomically.

{{% notice tip %}}
```cpp
class User {
 public:
  void SetName(const std::string& name) { name_ = NormalizeName(name); }

 private:
  std::string NormalizeName(const std::string& name) {
    return std::string(absl::StripAsciiWhitespace(name)).substr(50);
  }

  std::string name_;
};
```
{{% /notice %}}

Client no longer needs to be concerned about `NormalizeName`.

{{% notice tip %}}
```cpp
user.SetName(new_name);
```
{{% /notice %}}

#### Leaking state

Public access to state that isn't directly related to a client's goal is considered *leaking state*. 

{{% notice warning %}}
```cpp
class MessageRenderer : public Renderer {
 public:
  std::vector<std::shared_ptr<Renderer>> GetSubRenderers() {
    return sub_renderers_;
  }
  virtual std::string Render(const Message& message);

 private:
  std::vector<std::shared_ptr<Renderer>> sub_renderers_;
};
```
{{% /notice %}}

Keep the public API surface as small as possible while still meeting the clients needs.

{{% notice tip %}}
```cpp
class MessageRenderer : public Renderer {
 public:
  virtual std::string Render(const Message& message);

 private:
  std::vector<std::unique_ptr<Renderer>> sub_renderers_;
};
```
{{% /notice %}}

## Styles of Unit Testing

- **Output-based** - Verify output with a given input. Requires 
  [pure functions](#functional-architecture).
- **State-based** - Perform operation, then verify state of SUT and collaborators.
- **Communication-based** - Perform operation, then verify communication with collaborators.

| Style                     | Output-based | State-based | Communication-based |
| ------------------------- | ------------ | ----------- | ------------------- |
| Resistance to refactoring | High         | Medium      | Medium              |
| Maintainability           | High         | Medium      | Low                 |

## Hexagonal Architecture

*Hexagonal architecture*, proposed by Alistair Cockburn, breaks applications into two layers:

- **Domain layer** - Logic and models essential to the business domain.
- **Controller layer** - All other responsibilities of the application.

Interaction between the Domain layer and the Controller layer follows three guidelines: 

1. The Domain layer is isolated from the Controller layer.
1. The Domain layer may not depend on the Controller layer, but the Controller layer may 
   depend on the Domain layer.
1. Communication with external applications is handled by the Controller layer, not the 
   Domain layer.

![Hexagonal Architecture](/images/hexagonal_architecture.png)

The Domain layer has high cyclomatic complexity and domain significance, while the Controller layer 
has a large number of collaborators. Code that is both complex and involves many collaborators is 
overcomplicated and what Hexagonal architecture aims to dissolve. 

## Communication

Communication can be classified into two different types:

- **Intra-process** - Communication within the application; implementation details.
- **Inter-process** - Communication with other applications; observable behavior.

Mocks are necessary for emulating external applications and verifying inter-system communication 
patterns. However, mocks couple tests to implementation details, reducing their resistance to 
refactoring. For this reason, the use of mocks should be avoided when dealing with intra-system 
communication. External dependencies only accessible by the application, are implementation 
details too, and should not be mocked either.

## Functional Architecture

*Functional architecture* builds off of Hexagonal architecture with an added guideline that business 
logic is written in a functional paradigm. The architecture generally flows in a three step process:

1. The Controller layer gathers and prepare input.
1. The Domain layer makes decisions based on prepared input.
1. The Controller layer converts decisions into side effects.

![Functional Architecture](/images/functional_architecture.png)

> Object-oriented programming makes code understandable by encapsulating moving parts. Functional
> programming makes code understandable by minimizing moving parts. *--Michael Feathers*

Functional architecture trades maintainability for performance; pure functions tend to eagerly load 
data when they could have been lazy.

## Controller Orchestration

Functional architecture assumes a clearcut pipeline of inputs, decisions, and side effects. However,
production applications are rarely that simple. Decisions may involve gathering more input followed
by making additional decisions. There are three strategies to consider:

1. **Eagerly gather all the input** - Preserve controller simplicity and isolated domain logic, 
   but concede performance.
1. **Inject dependencies into the Domain layer** - Preserve controller simplicity and performance, but
   concede isolated domain logic.
1. **Allow controller orchestration** - Preserve isolated domain logic and performance, but concede 
   controller simplicity.

Isolated domain logic is an attribute that should always be maximized because it has a huge impact 
on *maintainability* and *resistance to refactoring*. Injecting dependencies into the Domain layer
is rule out. 

In cases where performance is not critical, feel free to stick to a Functional architecture and 
eagerly gather input. Otherwise, allow controllers to orchestrate gathering input to meet the 
needs of the Domain layer.

{{% notice tip %}}
Controller orchestration will make controllers more complex, but complexity can be mitigated with 
familiar patterns like switching on a result or listening for Domain layer events.
{{% /notice %}}
