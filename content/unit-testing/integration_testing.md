---
title: "Integration Testing"
weight: 3
---

## Role of Integration Tests 

While unit tests are great for testing domain logic, integration tests are great for testing 
controllers.

Integration tests are used to verify:

- Interactions with out-of-process dependencies.
- Edge cases unsuitable for a unit tests.

Each integration test should choose a happy path route that tests the most out-of-process 
dependencies while still testing a single use case. If a single integration test won't do, write
more until all out-of-process dependencies are covered.

{{% notice tip %}}
To keep maintenance costs low, check as many edge cases as possible with unit tests before resorting 
to an integration test.
{{% /notice %}}

{{% notice tip %}}
Edge cases that immediately stop the current operation without changing state adhere to the 
*Fail Fast principle* and can save you an integration test. 
{{% /notice %}}

### Mocking

Mocking works the same in integration tests as the do in unit tests:
- Private out-of-process dependencies are implementation details and should not be mocked. 
- Shared out-of-process dependencies should be mocked.
- Relational databases are more granular: private tables don't need to be mocked; shared tables do.

{{% notice note %}}
When the real version of a managed dependency is too difficult to setup or prohibitive, don't mock 
it. It the dependency cannot be tested as-is, don't write an integration test at all.
{{% /notice %}}

### Interfaces

Genuine abstractions are *discovered*, not *invented*. For an interface to be genuine, it must have 
at least two abstractions. Otherwise, the cost to the reader and the writer isn't worth it.

Interfaces enable mocking. Write an interface to mock shared out-of-process dependencies. 
However, private out-of-process dependencies aren't mocked, and by extension, don't benefit from an 
interface, unless they can be swapped for another implementation.

### Layering

Adding layers of indirection where they are not appropriate makes code hard to reason with. Aim
to have as few layers as possible. Most applications only need three layers: Domain, Controller, and 
Infrastructure. The Domain and Controller Laters are already familiar. The Infrastructure Layer
provides utility and protocol libraries.

![Layering](/images/layering.png)

### Logging

Logging falls under two categories:

- **Support logging** - messages are intended to be consumed by support staff and system admins; 
  observable behavior.
- **Diagnostic logging** - messages are intended to be consumed by developers; implementation 
  detail.

Since *Support* logging is observable behavior, it is worth the testing effort. *Diagnostic* 
logging, however, is an implementation detail and isn't worth testing.

{{% notice note %}}
The Domain layer should not have dependencies and the same is true for loggers. To log in the 
Domain layer, either the Controller layer injects a logger, or the Domain layer emits a log event.
{{% /notice %}}

Effective logging maximizes the signal to noise ratio. *Support* logging cannot be controlled 
because its a business requirement, but *Diagnostic* logging can. Minimize diagnostic logging
in the Domain layer, only adding it for debugging and the subsequently removing it once finished.
