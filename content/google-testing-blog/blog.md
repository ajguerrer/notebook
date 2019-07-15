---
title: "Blog Notes"
---

## Test Reliability

- Test binary size and memory usage, including third-party testing tools, have a strong correlation
  on whether a test is flaky. [^1]
- Beware of using UI testing to verify underlying functionality. In these cases, it is cheaper and
  more reliable to have smaller tests that break closer to the source of the problem.[^5] [^6]

## Productivity

- Releasing often gives teams an incentive to automate testing and reduce coupling with the rest of
  the system.[^2]
- Automation is costly. Automate only the tests that you find yourself running often to reliably
  catch regressions on features with business value.[^5]
- Tighten the feedback loop between test engineers and development engineers. Share the same space,
  tools, daily stand-ups, and design discussions. [^5]
- Effective automation depends on test design. Good test design is built from a solid foundation of
  manual tests.[^8]


## Code Health

- When a team provides [fakes](../tott/#know-your-test-doubles) and writes tests for them, they
  become clients of their own software. Experiencing the perspective of the client gives the team an
  incentive to make their API easier to use.[^2]
- Remove the detective work of tracking down bad changes by investing in a pre-submit system that
  runs automated tests against the commit before it reaches the depot. [^7]

## Metrics

- *Pre- vs post-production defect ratio* and a breakdown of *defects by component or functional
  area* help identify holes in test.[^3]
- Premature performance optimization makes bad code. Develop in a clean, maintainable and
  extensible manner *first*, and then let benchmarks drive performance optimizations.[^4]

## The Seven Plagues of Software Testing

- **Aimlessness** - Do not test for the sake of testing. Every test should have a goal. Document
  what works and analyze what doesn't. Then, share with your colleagues.[^9]
- **Repetitiveness** - Running the same test suite over again without finding new bugs does not
  mean that there are no bugs. Variation is healthy.[^10]
- **Amnesia** - Chances are the problem your are trying to solve has been solved before. If the same
  issue keeps stinging you, or you had to answer a question the hard way, document it and put it in
  a place others will find it.[^11]
- **Boredom** - A bored tester rushes through the tactical aspects of testing without considering
  the interesting strategic aspects. The day testing gets "figured out" is the day it gets
  completely automated away.[^12]
- **Homelessness** - Testers are homeless. They don't actually live in the software like users do.
  Some bugs are only found with the hands of users doing their work in their environment.[^13]
- **Blindness** - Software is a series of electromagnetic fluctuations on a storage media. Thats
  hardly a helpful visual for a software tester. So, we rely on auxiliary tools to provide useful
  feedback, but what other useful information are we blind to?[^14]
- **Entropy** - Testers introduce entropy by increasing the amount of things developers need to do.
  This is unavoidable, but preventative. As developers do more *during* development, entropy tends
  towards its minimum.[^15]

[^1]: https://testing.googleblog.com/2017/04/where-do-our-flaky-tests-come-from.html
[^2]: https://testing.googleblog.com/2017/02/discomfort-as-tool-for-change.html
[^3]: https://testing.googleblog.com/2007/10/post-release-closing-loop_02.html
[^4]: https://testing.googleblog.com/2007/10/performance-testing.html
[^5]: https://testing.googleblog.com/2007/10/automating-tests-vs-test-automation.html
[^6]: https://testing.googleblog.com/2008/06/taming-beast-aka-how-to-test-ajax.html
[^7]: https://testing.googleblog.com/2008/09/presubmit-and-performance.html
[^8]: https://testing.googleblog.com/2009/06/burning-test-questions-at-google.html
[^9]: https://testing.googleblog.com/2009/06/7-plagues-of-software-testing.html
[^10]: https://testing.googleblog.com/2009/06/by-james.html
[^11]: https://testing.googleblog.com/2009/07/plague-of-amnesia.html
[^12]: https://testing.googleblog.com/2009/07/plague-of-boredom.html
[^13]: https://testing.googleblog.com/2009/07/plague-of-homelessness.html
[^14]: https://testing.googleblog.com/2009/07/plague-of-blindness.html
[^15]: https://testing.googleblog.com/2009/09/plague-of-entropy.html
