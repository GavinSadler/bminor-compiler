# bminor-compiler

Gavin Sadler, Spring 2024 \
COMP.4060 - Compiler Construction

### How to make the compiler
```bash
$ make
```

### Running the compiler

So far, the only functioning parts of the compiler are the scanner, parse validator, AST generator, and a pretty printer, the following functionality is
supported:

```bash
$ ./bminor -scan        <file>
$ ./bminor -parse       <file>
$ ./bminor -prettyprint <file>
```

### How to run tests

This will run all of the tests created for the compiler. This includes lexing, parsing, and ensuring that the AST is valid via the pretty printer.

```bash
$ make test
```