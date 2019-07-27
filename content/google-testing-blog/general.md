---
title: "General Notes"
---

## Test Reliability

- Smaller tests run faster, are less flakey, and isolate failures. As a general rule of thumb, favor
  a composition of 70% small, 20% medium, and 10% large tests.[^11]
- Test binary size and memory usage, including third-party testing tools, have a strong correlation
  on whether a test is flaky.[^18]
- Beware of using UI testing to verify underlying functionality. In these cases, it is cheaper and
  more reliable to have smaller tests that break closer to the source of the problem.[^3] [^4]
- Hermetic servers add speed and reliability into end-to-end tests. A environment is considered
  Hermetic if it can run an entire system under test on a single machine with fake network
  connections and database implementations.[^9]
- Filter out flakey tests by rerunning failing tests. If a test fails three times in a row, consider it
  a real failure.[^13]
- Build testability into the product. For example, a real-time system can rely on a fake clock
  instead of a hardware clock. Processes can spawn other processes attached to a debugger with
  debugging flags.[^15]

## Code Quality

- Releasing often gives teams an incentive to automate testing and reduce coupling with the rest of
  the system.[^17]
- When a team provides [fakes](../tott/#know-your-test-doubles) and writes tests for them, they
  become clients of their own software. Experiencing the perspective of the client gives the team an
  incentive to make their API easier to use.[^17]
- While writing the header first encourages consideration for the interface, writing tests first
  encourages consideration for how the interface will be *used*.[^5]
- Good code quality is taught, not enforced. Create a culture that teaches code quality through code
  review, pair programming, and mentoring.[^15]

## Productivity

- Automation is costly. Automate only the tests that you find yourself running often to reliably
  catch regressions on features with business value.[^3]
- Speed up the feedback loop between test engineers and development engineers. Share the same space,
  tools, daily stand-ups, and design discussions.[^3]
- Effective automation depends on test design. Good test design is built from a solid foundation of
  manual tests.[^6]
- If a test plan isn't worth bothering to update, it isn't worth creating in the first place. A
  quick brainstorming session will suffice.[^8]
- Use formatting tools, like clang-format, to improve readability.[^14]

## Infrastructure
- Remove the detective work of tracking down bad changes by investing in a pre-submit system that
  runs automated tests against the commit before it reaches the depot.[^7] [^13]
- Don't fall behind on updating third party dependencies. Update them quickly by setting up CI
  system with dependencies pinned at head.[^12]
- Avoid making more than one branch by putting risky new changes behind feature flags.[^14]
- Constantly look for opportunities to make the build system faster. Reduce the amount of code being
  compiled, replace tools with faster counterparts, and use distributed build systems.[^16]
- Release early and release often. Services and websites can deploy rapidly. A good target for
  client projects is Chromes six week cycle.[^16]


## Metrics & Logging

- *Pre- vs post-production defect ratio* and a breakdown of *defects by component or functional
  area* help identify holes in test.[^1]
- Premature performance optimization makes bad code. Develop in a clean, maintainable and
  extensible manner *first*, and then let benchmarks drive performance optimizations.[^2]
- Remove unwanted noise by logging with conditional verbosity. Log all levels to a logging queue.
  If a transaction completes successfully, discard the unimportant levels.[^10]
- Use two sets of logging levels, one for production builds and one for development builds.[^10]
- Trace the time spent on every significant processing step. Measuring is the only way to detect
  performance issues or make claims about performance.[^10]
- Write automated performance tests for performance sensitive parts of your product.[^16]

[^1]: https://testing.googleblog.com/2007/10/post-release-closing-loop_02.html
[^2]: https://testing.googleblog.com/2007/10/performance-testing.html
[^3]: https://testing.googleblog.com/2007/10/automating-tests-vs-test-automation.html
[^4]: https://testing.googleblog.com/2008/06/taming-beast-aka-how-to-test-ajax.html
[^5]: https://testing.googleblog.com/2008/09/test-first-is-fun_08.html
[^6]: https://testing.googleblog.com/2008/09/presubmit-and-performance.html
[^7]: https://testing.googleblog.com/2009/06/burning-test-questions-at-google.html
[^8]: https://testing.googleblog.com/2011/09/10-minute-test-plan.html
[^9]: https://testing.googleblog.com/2012/10/hermetic-servers.html
[^10]: https://testing.googleblog.com/2013/06/optimal-logging.html
[^11]: https://testing.googleblog.com/2015/04/just-say-no-to-more-end-to-end-tests.html
[^12]: https://testing.googleblog.com/2015/05/multi-repository-development.html
[^13]: https://testing.googleblog.com/2016/05/flaky-tests-at-google-and-how-we.html
[^14]: https://testing.googleblog.com/2016/08/hackable-projects.html
[^15]: https://testing.googleblog.com/2016/10/hackable-projects-pillar-2-debuggability.html
[^16]: https://testing.googleblog.com/2016/11/hackable-projects-pillar-3.html
[^17]: https://testing.googleblog.com/2017/02/discomfort-as-tool-for-change.html
[^18]: https://testing.googleblog.com/2017/04/where-do-our-flaky-tests-come-from.html
