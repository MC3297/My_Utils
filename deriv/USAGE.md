### How to use

Run `g++ -Wall -D_GLIBCXX_DEBUG main.cpp` initially to create executable file.

Run `./a.out "<expr>"` where `<expr>` is your math expression.

For example `./a.out "x^2"`

Valid operations: `+`, `-`, `*`, `/`, `^`

Valid functions: `sin`, `cos`, `log`

Valid variables: `x`

Note:
- Functions raised to other functions is fine so like `sin(x)^cos(x)`
- Integers less than INT_MAX is fine
- Constants like "e" and "pi" **aren't** supported
- Nested parenthesis allowed so like `(x*(1+x))^2`
- "log" refers to the natural logarithm

Make sure the math expression is:
- 1 variable which is `x` (no uppercase)
- no spaces
- all multiplication is specified with `*` (so `6*x^2` instead of `6x^2`)
- no negative numbers but stuff like `(0-5)` is accepted
- functions are followed by parenthesis so no `sinx`