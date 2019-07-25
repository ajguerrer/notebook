---
title: "General Notes"
---

## Test Reliability

- Test binary size and memory usage, including third-party testing tools, have a strong correlation
  on whether a test is flaky. [^98]
- Beware of using UI testing to verify underlying functionality. In these cases, it is cheaper and
  more reliable to have smaller tests that break closer to the source of the problem.[^3] [^4]

## Productivity

- Releasing often gives teams an incentive to automate testing and reduce coupling with the rest of
  the system.[^99]
- Automation is costly. Automate only the tests that you find yourself running often to reliably
  catch regressions on features with business value.[^3]
- Tighten the feedback loop between test engineers and development engineers. Share the same space,
  tools, daily stand-ups, and design discussions. [^3]
- Effective automation depends on test design. Good test design is built from a solid foundation of
  manual tests.[^6]


## Code Health

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

## Code coverage goal: 80% and no less!

*July 20, 2010* -
[original post](https://testing.googleblog.com/2010/07/code-coverage-goal-80-and-no-less.html)

Early one morning, a young programmer asked the great master, “I am ready to write some unit tests.
What code coverage should I aim for?”

The great master replied, “Don’t worry about coverage, just write some good tests.”

The young programmer smiled, bowed, and left.

Later that day, a second programmer asked the same question. The great master pointed at a pot of
boiling water and asked, “How many grains of rice should I put in that pot?”

The programmer, looking puzzled, replied, “How can I possibly tell you? It depends on how many
people you need to feed, how hungry they are, what other food you are serving, how much rice you
have available, and so on.”

“Exactly,” said the great master.

The second programmer smiled, bowed, and left.

Toward the end of the day, a third programmer came and asked the same question about code coverage.

“Eighty percent and no less!” Replied the master in a stern voice, pounding his fist on the table.

The third programmer smiled, bowed, and left.

After this last reply, a young apprentice approached the great master, “Great master, today I
overheard you answer the same question about code coverage with three different answers. Why?”

The great master stood up from his chair. “Come get some fresh tea with me and let’s talk about it.”
After they filled their cups with smoking hot green tea, the great master began:

“The first programmer is new and just getting started with testing. Right now he has a lot of code
and no tests. He has a long way to go; focusing on code coverage at this time would be depressing
and quite useless. He’s better off just getting used to writing and running some tests. He can worry
about coverage later.

"The second programmer, on the other hand, is quite experienced both at programming and testing.
When I replied by asking her how many grains of rice I should put in a pot, I helped her realize
that the amount of testing necessary depends on a number of factors, and she knows those factors
better than I do – it’s her code after all. There is no single, simple, answer, and she’s smart
enough to handle the truth and work with that.”

“I see,” said the young apprentice, “but if there is no single simple answer, then why did you tell
the third programmer ‘Eighty percent and no less’?”

The great master laughed so hard and loud that his belly, evidence that he drank more than just
green tea, flopped up and down. “The third programmer wants only simple answers – even when there
are no simple answers … and then does not follow them anyway.”

The young apprentice and the grizzled great master finished drinking their tea in contemplative
silence.

[^1]: https://testing.googleblog.com/2007/10/post-release-closing-loop_02.html
[^2]: https://testing.googleblog.com/2007/10/performance-testing.html
[^3]: https://testing.googleblog.com/2007/10/automating-tests-vs-test-automation.html
[^4]: https://testing.googleblog.com/2008/06/taming-beast-aka-how-to-test-ajax.html
[^5]: https://testing.googleblog.com/2008/09/test-first-is-fun_08.html
[^6]: https://testing.googleblog.com/2008/09/presubmit-and-performance.html
[^7]: https://testing.googleblog.com/2009/06/burning-test-questions-at-google.html

[^98]: https://testing.googleblog.com/2017/04/where-do-our-flaky-tests-come-from.html
[^99]: https://testing.googleblog.com/2017/02/discomfort-as-tool-for-change.html
