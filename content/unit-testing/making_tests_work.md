---
title: "Making your Tests Work for You"
weight: 2
---

## Four Pillars of a Good Unit Test

- **Protection against regressions** - Ability to indicate the presence of regressions.
- **Resistance to refactoring** - Degree to which a test can sustain refactoring without producing
  a false positive.
- **Fast feedback** - Measure of how quickly the test executes.
- **Maintainability** - Ability to read and run the test.

<table>
  <tr>
    <th>Error Types</th>
    <th>Functionality is Correct</th>
    <th>Functionality is Broken</th>
  </tr>
  <tr>
    <th>Test Passes</th>
    <td>True negative</td>
    <td>False negative (protection against regressions)</td>
  </tr>
  <tr>
    <th>Test Fails</th>
    <td>False positive (resistance to refactoring)</td>
    <td>True positive</td>
  </tr>
</table>

$$Test\ accuracy = {Signal\ (True\ positives) \over Noise\ (False\ positives)}$$

## The Ideal Test

![Ideal test](/images/ideal_test.png)

{Protection against regressions, Resistance to refactoring, Fast feedback} - Choose two. Always 
maximize resistance to refactoring. Once resistance against refactoring is maximized, test size 
becomes a slider between protection against regressions and fast feedback.

![Test triangle](/images/test_triangle.png)

## Preventing False Positives

When tests fail for the wrong reasons, developers quickly loose trust in their tests. But without 
trustworthy tests, refactoring is risky. Caught in a catch-22, eventually real bugs will start to 
slip through.

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
