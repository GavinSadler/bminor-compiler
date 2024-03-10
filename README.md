# bminor-compiler

Gavin Sadler, Spring 2024 \
COMP.4060 - Compiler Construction

### How to make the compiler
```bash
$ make
```

### Running the compiler

So far, the only funcitoning parts of the compiler are the scanner and a parse validator, the following functionality is supported:

```bash
$ ./bminor -scan <file>
$ ./bminor -parse <file>
```

### How to run tests

So far, the only funcitoning parts of the compiler are the scanner and a parse validator, so it will run tests to validate both the parser and the scanner.

```bash
$ make test
```