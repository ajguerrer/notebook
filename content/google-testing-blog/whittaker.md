---
title: "James Whittaker"
---

## The Seven Plagues of Software Testing

- *Aimlessness* - Do not test for the sake of testing. Every test should have a goal. Document
  what works and analyze what doesn't. Then, share with your colleagues.[^1]
- *Repetitiveness* - Running the same test suite over again without finding new bugs does not
  mean that there are no bugs. Variation is healthy.[^2]
- *Amnesia* - Chances are the problem your are trying to solve has been solved before. If the same
  issue keeps stinging you, or you had to answer a question the hard way, document it and put it in
  a place others will find it.[^3]
- *Boredom* - A bored tester rushes through the tactical aspects of testing without considering
  the interesting strategic aspects. The day testing gets "figured out" is the day it gets
  completely automated away.[^4]
- *Homelessness* - Testers are homeless. They don't actually live in the software like users do.
  Some bugs are only found with the hands of users doing their work in their environment.[^5]
- *Blindness* - Testers require tools to provide helpful feedback from software. It's tempting to
  settle down with a trusty set of tools, but doing so causes self-inflicted blindness to a growing
  ecosystem of useful feedback.[^6]
- *Entropy* - Testers increase entropy by giving developers things to do. This is unavoidable, but
  preventative. As developers do more *during* development, testers add less work, and entropy tends
  towards a minimum.[^7]

## An Ingredients List for Testing

- *Product expertise* - A good developer knows how the product works; a good tester knows how to use
  it.[^8]
- *Bill of materials* - Testers should bea able to reference a complete list of features that can be
  tested.[^9]
- *Risk analysis* - Features are not equally important, or equally time consuming to test. Have a
  model to quantitatively analyze the risk of each feature.[^17]
- *Domain expertise* - It is not enough to be good at testing. Testers also need expertise with the
  technologies of the domain the product operates in.[^10]
- *Test guidance* - Whether it be technique, nomenclature, or history, testers need a way to
  identify and store tribal knowledge of the team.[^11]
- *Variation* - Tests often get stale. Wasting time running stale tests is also a form of risk.
  Adding variation can breathe new life into stale tests.[^12]
- *Completeness analysis* - Teams need a model to measure how well their testing efforts have
  covered the risk landscape of their product.[^13]

## How Google Tests Software

Engineers are loaned out to product teams on an as-needed basis. Engineers are free to change
product teams at their own cadence.[^14]

Developers own quality while testers support developers with tools and feedback. As developers get
better at testing, less testers are needed. Successful teams have higher developer-to-tester
ratios. [^14]

Development and test are not treated as separate disciplines. Developers test and testers code.[^16]
Instead, each of the three roles look at the product from different angles:

- *SWE (Software Engineer)* - Feature creators responsible for their work. SWEs design and write
  features, and prove they work by writing and running tests.
- *SET (Software Engineer in Test)* - Codebase caretakers who enable SWEs to enforce quality. SETs
  refactor code for testability, and write test doubles and framework.
- *TE (Test Engineer)* - Product experts who analyze quality and risk from the perspective of the
  user. TEs write automation scripts, drive test execution and interpret test results.[^15]


Quality is a work in progress that relies on getting product out to users and receiving feedback as
quickly as possible. As its being developed, a release is pushed through several channels in order
of increasing confidence in product quality:

- *Canary* - Only fit for ultra tolerant users running experiments.
- *Dev* - What developers use on their day-to-day work.
- *Test* - Used internally for day-to-day work.
- *Beta/Release* - Fit for external exposure.[^17]

[^1]: https://testing.googleblog.com/2009/06/7-plagues-of-software-testing.html
[^2]: https://testing.googleblog.com/2009/06/by-james.html
[^3]: https://testing.googleblog.com/2009/07/plague-of-amnesia.html
[^4]: https://testing.googleblog.com/2009/07/plague-of-boredom.html
[^5]: https://testing.googleblog.com/2009/07/plague-of-homelessness.html
[^6]: https://testing.googleblog.com/2009/07/plague-of-blindness.html
[^7]: https://testing.googleblog.com/2009/09/plague-of-entropy.html
[^8]: https://testing.googleblog.com/2010/08/ingredients-list-for-testing-part-one.html
[^9]: https://testing.googleblog.com/2010/08/ingredients-list-for-testing-part-two.html
[^9]: https://testing.googleblog.com/2010/09/ingredients-list-for-testing-part-three.html
[^10]: https://testing.googleblog.com/2010/09/ingredients-list-for-testing-part-four.html
[^11]: https://testing.googleblog.com/2010/10/ingredients-list-for-testing-part-five.html
[^12]: https://testing.googleblog.com/2010/11/ingredients-list-for-testing-part-six.html
[^13]: https://testing.googleblog.com/2010/11/ingredients-list-for-testing-part-seven.html
[^14]: https://testing.googleblog.com/2011/01/how-google-tests-software.html
[^15]: https://testing.googleblog.com/2011/02/how-google-tests-software-part-two.html
[^16]: https://testing.googleblog.com/2011/02/how-google-tests-software-part-three.html
[^17]: https://testing.googleblog.com/2011/03/how-google-tests-software-part-four.html