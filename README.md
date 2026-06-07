# C to RISC-V Compiler

A two-pass compiler that translates a C subset into RV32IM + F (single-precision float) assembly. Built as part of the *Program Optimisations and Runtime Systems* (PORS) course at the University of Vienna.

## What it does

Takes a `.c` input file containing a single function body and emits `program.s` — valid RISC-V assembly that can be assembled and run on any RV32 simulator or hardware target.

```
source.c  →  [Flex scanner]  →  [Bison parser]  →  [AST]  →  [codegen]  →  program.s
```

## Supported features

| Feature | Details |
|---|---|
| Types | `int`, `float` (f32) |
| Arithmetic | `+ - * / %` for int; `+ - * /` for float |
| Relational | `< > <= >= == !=` |
| Logical / bitwise | `&& \|\| & \| ^ << >>` |
| Control flow | `if`, `if/else`, `while`, `do-while`, `for` |
| Mixed expressions | Implicit `int → float` promotion via `fcvt.s.w` |

## Project structure

```
.
├── scanner.l        # Flex tokeniser
├── parser.y         # Bison grammar + AST construction
├── intermediate.h   # Node types, symbol table, float const table
├── intermediate.c   # AST node constructors + RISC-V code generator
├── Makefile
└── test_function.c  # Example input
```

## Build & run

**Requirements:** `gcc`, `flex`, `bison`, `graphviz`

```bash
make                        # builds ./cparser and runs it on test_function.c
```

To assemble and simulate the output (requires [RARS](https://github.com/TheThirdOne/rars) or [spike](https://github.com/riscv-software-src/riscv-isa-sim)):

```bash
rars program.s
```

## Key implementation details

### fp-relative expression stack
Nested arithmetic expressions need to preserve intermediate values across recursive code generation. A naive hardware-stack approach (`sw t0, 0(sp)`) breaks because nested sub-expressions move `sp`, making the reload address wrong. Instead, a global `expr_depth` counter indexes fixed slots above the frame pointer (`fp+4`, `fp+8`, …), so each nesting level always finds its value at the same address.

### Float constant interning
Float literals (e.g. `3.14`) are collected into a `.data` table at parse time via `intern_float_const`. Each unique value gets a label (`.FC0`, `.FC1`, …). At codegen time they are loaded with `lui t0, %hi(.FC0)` + `flw ft0, %lo(.FC0)(t0)` — the only way to get a 32-bit float value into a float register in a two-instruction sequence.

### Type propagation
Every AST node carries a `type` field. `make_binop` tags the result `FLOAT` if either child is `FLOAT`, so the code generator knows which instruction family to use without a separate analysis pass.

## Known limitations

- Variable declarations must appear before any statements (C89 style)
- No `char` or `short` — all variables use 4-byte slots with `lw`/`sw`
- No `double` — falls silently to the integer code path
- Single function scope only (`main`) — no function calls or return values
- No pointers, arrays, or structs
- No I/O — results are observable via memory inspection only
- `&&` and `||` are not short-circuit
- `%` is undefined for float operands

## Assignment context

| | Assignment 1 | Assignment 2 |
|---|---|---|
| Output | AST → Graphviz DOT | RISC-V assembly |
| Focus | Scanning, parsing, symbol table | Code generation, type system |
| New in `.h/.c` | Basic node types | `op` field, `type` propagation, float support, expr stack |