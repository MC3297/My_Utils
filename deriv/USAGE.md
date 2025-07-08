# How to use

Run `make compile` to create executable file.

Run `./derivate "<expr>"` where `<expr>` is your math expression.

For example `./derivate "x^2"`

Valid operations`+`, `-`, `*`, `/`, `^`

Valid functions: `sin`, `cos`, `log`

Valid variables: `x`

### Note:
- Functions raised to other functions is fine so like `sin(x)^cos(x)`
- Integers less than INT_MAX is fine
- Constants like "e" and "pi" **aren't** supported
- Nested parenthesis allowed so like `(x*(1+x))^2`
- "log" refers to the natural logarithm

### Make sure the math expression is:
- 1 variable which is `x` (no uppercase)
- no spaces
- all multiplication is specified with `*` (so `6*x^2` instead of `6x^2`)
- no negative numbers but stuff like `(0-5)` is accepted
- functions are followed by parenthesis so no `sinx`

### Some examples
`./derivate "log(x^2)"`  
`./derivate "2^x"`  
`./derivate "x^x"`  
`./derivate "5*x^7"`