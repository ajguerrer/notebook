---
title: "Integration Testing"
weight: 3
---

## Role of Integration Tests 

Integration tests verify:

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
*Fail Fast principle* and don't require an integration test. 
{{% /notice %}}

While unit tests are great for testing domain logic, integration tests are great for testing 
controllers.

![Test matrix](/images/test_matrix.png)

#### Interfaces

Genuine abstractions are *discovered*, not *invented*. For an interface to be genuine, it must have 
at least two implementations. Otherwise, the cost to the reader and the writer isn't worth it.

Interfaces enable mocking. Write an interface to mock shared out-of-process dependencies. 
However, not all out-of-process dependencies need an interface. Private out-of-process dependencies 
don't need mocks and therefore don't benefit from an interface, unless they can be swapped for 
another implementation.

#### Layering

Adding layers of indirection where they are not appropriate makes code hard to reason with. Aim
to have as few layers as possible. Most applications only need three layers: Domain, Controller, and 
Infrastructure. The Domain and Controller Layers are already familiar. The Infrastructure Layer
provides utility libraries such as logging and networking.

![Layering](/images/layering.png)

#### Logging

Logging falls under two categories:

- **Support logging** - messages are intended to be consumed by support staff and system admins; 
  observable behavior.
- **Diagnostic logging** - messages are intended to be consumed by developers; implementation 
  detail.

Since *Support* logging is observable behavior, it is worth the testing effort. *Diagnostic* 
logging, however, is an implementation detail and isn't worth testing.

{{% notice note %}}
The Domain layer should not have dependencies, including loggers. Have the Controller layer inject a 
logger, or have the Domain layer emit a log event.
{{% /notice %}}

Effective logging maximizes the signal to noise ratio. *Support* logging cannot be controlled 
because its a business requirement, but *Diagnostic* logging can. Minimize diagnostic logging
in the Domain layer, only adding it for debugging and the subsequently removing it once finished.

## Mocking Best Practices

- **Mocks are for integration tests only** - Mentioned in 
  [Hexagonal Architecture](/unit-testing/making_tests_work/#hexagonal-architecture), only shared 
  out-of-process dependencies should be mocked. Since unit tests don't involve out-of-process 
  dependencies, only integration tests should make use of mocks.
- **Never mock private out-of-process dependencies** - When a private out-of-process dependency is too 
  difficult or prohibitive to setup, don't try to mock it out. If that dependency cannot be tested 
  as-is, it defeats the point of integration testing and should not be tested at all.
- **Mock the furthest edges of the system** - Mock the type that directly communicates with the 
  shared dependency, not the wrapper. Verify the message sent to the shared dependency, not the call
  to a class you wrote. Doing so maximizes resistance against regressions.
- **Use as many mocks as necessary per test** -  One mock per test is a common misconception. It's 
  irrelevant how many mocks it takes to verify one unit of behavior. The number of mocks depends
  solely on the number of shared out-of-process dependencies participating in the unit of behavior.
- **Verify the absence of unexpected messages too** - It is not enough to verify your system is 
  sending the correct messages to shared out-of-process dependencies, unexpected messages are a
  source of bugs too.
- **Mock only the types you own** - When using a third party library that communicates with a shared
  out-of-process dependency, write a adapter for it and mock the adapter instead. Third-party 
  libraries have arcane inner workings; it's futile to try to emulate their behavior.

## Testing the Database

#### Setup

Be able to build the database from a series of migration scripts checked into source control. This 
includes tables, views, indexes, stored procedures, reference data and anything else critical to 
run the database in production.

{{% notice note %}}
*Reference data* is data that is necessary for the application to run, but isn't actively modified 
by the application.
{{% /notice %}}

migration scripts are used in testing and production. Once committed to source control,
don't modify them. Make a new migration script instead of fixing the old one, unless fixing the old
one prevents data loss.

#### Transactions

Transactions are capable of updating sets of data within the same business operation atomically. 
When transactions are involved, applications make two separate types of decisions:

1. What data gets updated
2. Should a set of updates be kept or rolled back

These decisions should be separated into two different responsibilities:

- **Repositories** - Can access and modify database data; lifetime lasts the duration of the query.
- **Transactions** - Commits or rolls back a set of updates in full; lifetime lasts the duration of
  the business operation.

{{% notice note %}}
Non-relational databases lack transactions. Instead, updates to the document are atomic. Related 
data should be placed in the same document so that updates don't span more than one document.
{{% /notice %}}


To replicate the production environment as closely as possible, integration tests should wrap the 
*act* section in a separate transaction from the *arrange* and *assert* sections.

#### Cleanup

There are many ways to cleanup a database between tests:

- **Restore a backup database before each test** - Works but is slow.
- **Cleanup data at the end of the test** - Might be skipped when test fails.
- **Wrap each test in a transaction and then don't commit it** - Makes behavior between 
  production and testing inconsistent.
- **Cleanup data at the beginning of the test** - Is fast, consistent, and won't get skipped.
 
#### Best Practices

- **Avoid in-memory databases** - Changing the database introduces behavioral inconsistencies 
  between production and testing.
- **Reuse code in test sections** - *Arrange* with factory or builder pattern, *act* with decorator
  pattern, and *assert* with handmade mocks (a.k.a. Spys).
- **Skip trivial database read tests** - Write operations are always important because they can lead
  to data corruption, but reads won't corrupt the database. Only test reads that are important or 
  complex; disregard the rest.
- **Don't test repositories directly** - Repositories belong in the Controller layer, and if 
  properly designed, shouldn't be complex. Like any controller, they should be included in the 
  overarching integration test. 
