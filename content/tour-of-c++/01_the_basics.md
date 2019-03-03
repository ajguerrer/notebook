---
title: "The Basics"
pre: "<b>1. </b>"
weight: 1
---

1. Don't panic! All will become clear in time.
1. Don't use the built-in features exclusively or on their own. On the contrary, built-in features are usually best used indirectly through libraries, such as the ISO C++ standard library.
    ```cpp
    class Date {
      // ...
    public:
      Month month() const; // good
      int month() cont;    // bad
      // ...
    };

    void change_speed(double s); // bad
    void change_speed(Speed s);  // good
    // ...

    change_speed(2.3);       // bad
    change_speed(32m / 10s); // good
    ```
1. You don't have to know every detail of C++ to write good programs.
1. Focus on programming techniques, not on language features.
1. For the final word on language definition issues, see the ISO C++ standard.
1. "Package" meaningful operations as carefully named functions.
    ```cpp
    // bad
    void read_and_print(istream& is)
    {
      int x;
      if (is >> x)
        cout << "the int is " << x << '\n';
      else
        cerr << "no int on input\n";
    }
    // good
    auto lessT = [](T x, T y) { return x.rank() < y.rank() && x.value() < y.value(); };

    sort(a, b, lessT);
    find_if(a, b, lessT);
    ```
1. A function should perform as single logical operation.
    ```cpp
    // bad
    void read_and_print()
    {
        int x;
        cin >> x;
        // check for errors
        cout << x << "\n";
    }

    // better
    auto read = [](auto& input, auto& value)
    {
      input >> value;
      // check for errors
    };

    auto print(auto& output, const auto& value)
    {
      output << value << "\n";
    }
    ```
1. Keep functions short.
    ```cpp
    // bad: complex control structure
    double simple_func(double val, int flag1, int flag2)
    // simple_func: takes a value and calculates the expected ASIC output,
    // given the two mode flags.
    {
      double intermediate;
      if (flag1 > 0) {
        intermediate = func1(val);
        if (flag2 % 2)
          intermediate = sqrt(intermediate);
      }
      else if (flag1 == -1) {
        intermediate = func1(-val);
        if (flag2 % 2)
          intermediate = sqrt(-intermediate);
        flag1 = -flag1;
      }
      if (abs(flag2) > 10) {
        intermediate = func2(intermediate);
      }
      switch (flag2 / 10) {
        case 1: if (flag1 == -1) return finalize(intermediate, 1.171);
          break;
        case 2: return finalize(intermediate, 13.1);
        default: break;
      }
      return finalize(intermediate, 0.);
    }

    // better: refactor
    double func1_muon(double val, int flag)
    {
      // ???
    }

    double func1_tau(double val, int flag1, int flag2)
    {
      // ???
    }

    double simple_func(double val, int flag1, int flag2)
    // simple_func: takes a value and calculates the expected ASIC output,
    // given the two mode flags.
    {
      if (flag1 > 0)
        return func1_muon(val, flag2);
      if (flag1 == -1)
        // handled by func1_tau: flag1 = -flag1;
        return func1_tau(-val, flag1, flag2);
      return 0.;
    }
    ```
1. Use overloading when functions perform conceptually the same task on different types.
1. If a function may have to be evaluated at compile time, declare it `constexpr`.
    ```cpp
    constexpr int fac(int n)
    {
      constexpr int max_exp = 17;      // constexpr enables max_exp to be used in Expects
      Expects(0 <= n && n < max_exp);  // prevent silliness and overflow
      int x = 1;
      for (int i = 2; i <= n; ++i) x *= i;
      return x;
    }
    ```
1. Understand how language primitives map to hardware.
1. Use digit separators to make large literals readable.
    ```cpp
    auto c = 299'792'458;
    auto hello = "Hello!"s;
    auto interval = 100ms; // <chrono>
    ```
1. Avoid complicated expressions.
    ```cpp
    // bad: multiple assignments "hidden" in subexpressions
    x = a + (b = f()) + (c = g()) * 7;

    // bad: relies on commonly misunderstood precedence rules
    x = a & b + c * d && e ^ f == 7;

    // bad: undefined behavior
    x = x++ + x++ + ++x;

    x = k * y + z;             // OK

    auto t1 = k * y;           // bad: unnecessarily verbose
    x = t1 + z;
    
    if (0 <= x && x < max)   // OK
    
    auto t1 = 0 <= x;        // bad: unnecessarily verbose
    auto t2 = x < max;
    if (t1 && t2)            // ...
    ```
1. Avoid narrowing conversions.
    ```cpp
    double d = 7.9;
    int i = d;    // bad: narrowing: i becomes 7
    i = (int) d;  // bad: we're going to claim this is still not explicit enough
    // ...

    // using guideline support library
    i = narrow_cast<int>(d);   // ok (you asked for it): narrowing: i becomes 7
    i = narrow<int>(d);        // ok: throws narrowing_error
    ```
1. Minimize the scope of a variable.
1. Avoid "magic constants"; use symbolic constants.
    ```cpp
    // bad: magic constant 12
    for (int m = 1; m <= 12; ++m)
    cout << month[m] << '\n';

    // better
    constexpr int first_month = 1;
    constexpr int last_month = 12;

    for (int m = first_month; m <= last_month; ++m)
    cout << month[m] << '\n';

    // better still
    for (auto m : month)
    cout << m << '\n';
    ```
1. Prefer immutable data.
1. Declare one name (only) per declaration.
    ```cpp
    char *p, c, a[7], *pp[7], **aa[10];   // yuck!
    ```

    Structured bindings are specifically designed to introduce several variables.

    ```cpp
    auto [iter, inserted] = m.insert_or_assign(k, val);
    if (inserted) { /* new entry was inserted */ }
    ```
1. Keep common and local names short, and keep uncommon an non-local names longer.
    ```cpp
    template<typename T>    // good
    void print(ostream& os, const vector<T>& v)
    { /* ... */ }

    template<typename Element_type>   // bad: verbose, hard to read
    void print(ostream& target_stream, const vector<Element_type>& current_vector)
    { /* ... */ }

    void use1(const string& s)
    {
      // ...
      tt(s);   // bad: what is tt()?
      // ...
    }

    void use1(const string& s)
    {
      // ...
      trim_tail(s);   // better
      // ...
    }

    // bad: large functions are de facto non-local; vr, vi and out need to be more meaningful
    void complicated_algorithm(vector<Record>& vr, const vector<int>& vi, map<string, int>& out)
    {
      // ... 500 lines of code using vr, vi, and out ...
    }
    ```
1. Avoid similar-looking names.
    ```cpp
    if (readable(i1 + l1 + ol + o1 + o0 + ol + o1 + I0 + l0)) surprise(); // bad

    struct foo { int n; };
    struct foo foo();       // bad: foo is a type already in scope
    struct foo x = foo();   // bad: requires disambiguation
    ```
1. Avoid `ALL_CAPS` names. `ALL_CAPS` should used for macros only.
1. Prefer the `{}`-initializer syntax for declarations with a named type.
    ```cpp
    int x {7.9};   // error: narrowing
    int y = 7.9;   // y becomes 7. Hope for a compiler warning
    ```
1. Use `auto` to avoid repeating type names.
    ```cpp
    auto p = v.begin();   // vector<int>::iterator
    auto h = t.future();
    auto q = make_unique<int[]>(s);
    auto f = [](int x){ return x + 10; };
    ```

    Avoid `auto` for initializer lists.

    ```cpp
    auto lst = { 1, 2, 3 };   // lst is an initializer list
    auto x{1};   // x is an int (in C++17; initializer_list in C++11)
    ```
1. Avoid uninitialized variables.
    ```cpp
    void use(int arg)
    {
      int i;   // bad: uninitialized variable
      // ...
      i = 7;   // initialize i
    }

    void use(int arg)   // ok
    {
      int i = 7;   // ok: initialized
      string s;    // ok: default initialized
      // ...
    }
    ```
1. Keep scopes small.
    ```cpp
    int i;    // bad: i is needlessly accessible after loop
    for (i = 0; i < 20; ++i) { /* ... */ }

    for (int i = 0; i < 20; ++i) { /* ... */ }  // good: i is local to for-loop
    
    // bad
    string fn = name + ".txt";
    ifstream is {fn};
    // ... 200 lines of code without intended use of fn or is ...
    ```
1. When declaring a variable in the condition of an `if`-statement, prefer the version with the implicit test against `0`.
1. Use `unsigned` for bit manipulation only.
    ```cpp
    unsigned area(unsigned height, unsigned width) { return height*width; }
    // ...
    int height;
    cin >> height;
    auto a = area(height, 2);   // if the input is -2 a becomes 4294967292
    ```
1. Keep use of pointers simple and straightforward.
    ```cpp
    void f(int* p, int count)
    {
      if (count < 2) return;
  
      int* q = p + 1;   // bad: pointer arithmetic
  
      ptrdiff_t d;
      int n;
      d = (p - &n);     // ok
      d = (q - p);      // ok
  
      int n = *p++;     // bad: pointer arithmetic
  
      if (count < 6) return;
  
      p[4] = 1;         // bad: pointer indexing
  
      p[count - 1] = 2; // bad: pointer indexing
  
      use(&p[0], 3);    // bad: implicit conversion of array to pointer
    }
    ```
1. Use `nullptr` rather than `0` or `NULL`.
1. Don't declare a variable until you have a value to initialize with.
    ```cpp
    string s;
    // ... no use of s here ...
    s = "what a waste";
    ```
1. Dont' say in comments what can be clearly stated in code.
   ```cpp
   // bad
   auto x = m * v1 + vv;   // multiply m with v1 and add the result to v
   ```
1. State intent in comments.
    ```cpp
    // sort c in the order determined by <, keep equal elements (as defined by ==) in
    // their original relative order
    void stable_sort(Sortable& c)
    {
      // ... quite a few lines of non-trivial code ...
    }
    ```
1. Maintain a consistent indentation style.