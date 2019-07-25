---
title: "General Notes"
---

## Test Reliability

- Test binary size and memory usage, including third-party testing tools, have a strong correlation
  on whether a test is flaky. [^98]
- Beware of using UI testing to verify underlying functionality. In these cases, it is cheaper and
  more reliable to have smaller tests that break closer to the source of the problem.[^3] [^4]

## Productivity

- Automation is costly. Automate only the tests that you find yourself running often to reliably
  catch regressions on features with business value.[^3]
- Tighten the feedback loop between test engineers and development engineers. Share the same space,
  tools, daily stand-ups, and design discussions. [^3]
- Effective automation depends on test design. Good test design is built from a solid foundation of
  manual tests.[^6]
- If a test plan isn't worth bothering to update, it isn't worth creating in the first place. A
  quick brainstorming session will suffice.[^8]

## Code Health

- Releasing often gives teams an incentive to automate testing and reduce coupling with the rest of
  the system.[^99]
- When a team provides [fakes](../tott/#know-your-test-doubles) and writes tests for them, they
  become clients of their own software. Experiencing the perspective of the client gives the team an
  incentive to make their API easier to use.[^99]
- While writing the header first encourages consideration for the interface, writing tests first
  encourages consideration for how the interface will be *used*.[^5]
- Remove the detective work of tracking down bad changes by investing in a pre-submit system that
  runs automated tests against the commit before it reaches the depot. [^7]

## Metrics

- *Pre- vs post-production defect ratio* and a breakdown of *defects by component or functional
  area* help identify holes in test.[^1]
- Premature performance optimization makes bad code. Develop in a clean, maintainable and
  extensible manner *first*, and then let benchmarks drive performance optimizations.[^2]



[^1]: https://testing.googleblog.com/2007/10/post-release-closing-loop_02.html
[^2]: https://testing.googleblog.com/2007/10/performance-testing.html
[^3]: https://testing.googleblog.com/2007/10/automating-tests-vs-test-automation.html
[^4]: https://testing.googleblog.com/2008/06/taming-beast-aka-how-to-test-ajax.html
[^5]: https://testing.googleblog.com/2008/09/test-first-is-fun_08.html
[^6]: https://testing.googleblog.com/2008/09/presubmit-and-performance.html
[^7]: https://testing.googleblog.com/2009/06/burning-test-questions-at-google.html
[^8]: https://testing.googleblog.com/2011/09/10-minute-test-plan.html

[^98]: https://testing.googleblog.com/2017/04/where-do-our-flaky-tests-come-from.html
[^99]: https://testing.googleblog.com/2017/02/discomfort-as-tool-for-change.html
