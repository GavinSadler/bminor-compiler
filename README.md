# bminor-compiler

Gavin Sadler, Spring 2024 \
COMP.4060 - Compiler Construction

### How to make the compiler
```bash
$ make
```

### Running the compiler

So far, the only funcitoning portion of the compiler is the scanner, so it can only be used to scan tokens from a file.

```bash
$ ./bminor -scan <file>
```

### How to run tests

So far, the only funcitoning portion of the compiler is the scanner, so it will only run the scanner tests

```bash
$ make test
```