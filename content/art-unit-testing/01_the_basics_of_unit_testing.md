---
title: "The Basics of Unit Testing"
pre: "<b>1. </b>"
weight: 1
---

## Properties of a good unit test

* Automated
* Repeatable
* Aasy to read
* Aasy to run
* Aasy to fix
* Relevant
* Quick
* Consistent
* Isolated
* Focused

Unit tests are meant to be small and test one scenario at a time. However, the scope of the system 
under test (SUT) may be small and concrete or big and abstract.

## Integration tests

System hardware has non-deterministic tendencies. Integration tests are not isolated from their 
hardware dependencies, making them neither fast nor consistent. Unit tests do not depend on system 
hardware.