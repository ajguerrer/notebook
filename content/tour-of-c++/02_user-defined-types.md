---
title: "User-Defined Types"
pre: "<b>2. </b>"
weight: 2
---

1. Prefer well-defined user-defined types over built-in types when the built-in types are too low level.
1. Organize related data into structures
1. Represent the distinction between an interface and an implementation using a `class`
1. A `struct` is simply a `class` with its members `public by default
1. Define constructors to guarantee and simplify initialization of `class`es
1. Avoid "naked" `union`s; wrap them in a class together with a type field
1. Use enumerations to represent sets of named constants
1. Prefer `class` `enum`s over "plain" `enum`s to minimize surprises
1. Define operations on enumerations for safe and simple use