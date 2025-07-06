### How to use

Type ur expression into the string `expr` in main.cpp and run:

`g++ -Wall -D_GLIBCXX_DEBUG main.cpp && ./a.out` in terminal

Valid operations: `+`, `-`, `*`, `/`, `^`

Valid functions: `sin`, `cos`, `log`

Valid variables: `x`

Note:
- Functions raised to other functions is fine so like `sin(x)^cos(x)`
- Integers less than INT_MAX is fine
- Constants like "e" and "pi" **aren't** supported
- Nested parenthesis allowed so like `(x*(1+x))^2`

Make sure the math expression is:
- 1 variable which is `x` (no uppercase)
- no spaces
- all multiplication is specified with `*` (so `6*x^2` instead of `6x^2`)
- no negative numbers but stuff like `(0-5)` is accepted