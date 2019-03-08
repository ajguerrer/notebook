---
title: "User-Defined Types"
pre: "<b>2. </b>"
weight: 2
---

1. Prefer well-defined user-defined types over built-in types when the built-in types are too low level.
1. Organize related data into structures.
    
    ```cpp
    void draw(int x, int y, int x2, int y2);  // bad: unnecessary implicit relationships
    void draw(Point from, Point to);          // better
    ```
1. Represent the distinction between an interface and an implementation using a `class`.
1. A `struct` is simply a `class` with its members `public by default.
1. Define constructors to guarantee and simplify initialization of `class`es.
1. Avoid "naked" `union`s; wrap them in a class together with a type field.
    ```cpp
    union Value {
      int x;
      double d;
    };

    Value v;
    v.d = 987.654;  // v holds a double
    cout << v.x << '\n';    // bad, undefined behavior: v holds a double, but we read it as an int
    ```
1. Use enumerations to represent sets of named constants.
1. Prefer `class` `enum`s over "plain" `enum`s to minimize surprises.
    ```cpp
    void Print_color(int color);

    enum Web_color { red = 0xFF0000, green = 0x00FF00, blue = 0x0000FF };
    enum Product_info { Red = 0, Purple = 1, Blue = 2 };

    Web_color webby = Web_color::blue;
    
    // Clearly at least one of these calls is buggy.
    Print_color(webby);
    Print_color(Product_info::Blue);
    ```

    `enum`s readily convert to `int`. Use `enum class` to prevent the conversion.

    ```cpp
    void Print_color(int color);

    enum class Web_color { red = 0xFF0000, green = 0x00FF00, blue = 0x0000FF };
    enum class Product_info { red = 0, purple = 1, blue = 2 };

    Web_color webby = Web_color::blue;

    Print_color(webby);              // Error: cannot convert Web_color to int.
    Print_color(Product_info::Red);  // Error: cannot convert Product_info to int.
    ```
1. Define operations on enumerations for safe and simple use.
    ```cpp
    enum Day { mon, tue, wed, thu, fri, sat, sun };

    Day& operator++(Day& d)
    {
        return d = (d == Day::sun) ? Day::mon : static_cast<Day>(static_cast<int>(d)+1);
    }
    ```