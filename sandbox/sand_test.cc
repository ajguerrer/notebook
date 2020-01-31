
#include "sand.h"

#include <string>

#include "gtest/gtest.h"

TEST(Render, BasicMessage) {
  MessageRenderer renderer;
  const Message message{header : "a", body : "b", footer : "c"};

  std::string html = renderer.Render(message);

  EXPECT_EQ(html,
            "<head><title>a</title></head><body>b</body><footer>c</footer>");
}
