# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

```bash
cmake -B build && cmake --build build        # configure + build
./build/src/myproject                         # run the executable
```

## Test

```bash
cmake --build build --target test                         # run all tests
./build/tests/mylib_test --gtest_filter='MylibTest.Greet' # single test
```

Tests use GoogleTest, discovered via `find_package(GTest REQUIRED)` — it must be installed on the system.

## Lint & Format

```bash
bash scripts/format.sh    # clang-format (in-place) on src/, include/, tests/
bash scripts/tidy.sh      # clang-tidy (requires build/ to exist for compile_commands.json)
```

## Naming Conventions (enforced by clang-tidy)

| Element                        | Style        | Example           |
|-------------------------------|-------------|-------------------|
| Functions / methods           | `lower_case` | `do_thing()`      |
| Local variables / parameters  | `camelBack`  | `myVar`           |
| Classes / structs / enums     | `CamelCase`  | `MyClass`         |
| Member variables              | `camelBack_` | `memberVar_`      |
| Global variables              | `gCamelCase` | `gMaxSize`        |
| Global constants              | `gCamelCase` | `gMaxSize`        |
| Static vars & consts          | `sCamelCase` | `sMaxSize`        |
| Constexpr variables           | `kCamelCase` | `kMaxSize`        |
| Enum values                   | `UPPER_CASE` | `COLOR_RED`       |
| Namespaces                    | `lower_case` | `mylib`           |
| Macros                        | `UPPER_CASE` | `MY_MACRO`        |

## Code Style

### Formatting (clang-format, LLVM-based)

- C++17, 2-space indent, 100-col limit, no tabs
- Braces: Attach style (K&R), no brace wrapping for functions/classes/control statements
- Pointers and references right-aligned: `int *ptr`, `const std::string &ref`
- Constructor initializer lists: `BeforeComma` style, zero extra indent, one per line (`PackConstructorInitializers: NextLine`)
- Function arguments and parameters: one per line when broken (`BinPackArguments: false`, `BinPackParameters: false`), break after open bracket (`AlignAfterOpenBracket: AlwaysBreak`)
- Short functions allowed inline only; short if/loops never on a single line
- Includes: regrouped and sorted case-insensitively — local headers first (`"foo.hpp"`), then system headers (`<...>`), then other quoted headers
- `Cpp11BracedListStyle: true`, no space before braced lists
- Align trailing comments (even across empty lines), align operands

### Static Analysis (clang-tidy)

Enabled check groups: `bugprone-*`, `cppcoreguidelines-*`, `misc-*`, `modernize-*`, `performance-*`, `readability-*`, `llvm-header-guard`.

Notable disabled checks:
- `bugprone-easily-swappable-parameters`, `bugprone-exception-escape`
- `cppcoreguidelines-avoid-magic-numbers`, `cppcoreguidelines-owning-memory`, `cppcoreguidelines-pro-bounds-array-to-pointer-decay`
- `misc-non-private-member-variables-in-classes`, `misc-include-cleaner`, `misc-use-internal-linkage`
- `modernize-use-trailing-return-type`
- `readability-magic-numbers`, `readability-identifier-length`

Thresholds:
- Function cognitive complexity: 25
- Function line count: 100

Header guards: use `#ifndef`/`#define` (not `#pragma once`), extensions: `.hpp`, `.h`, `.hh`, `.hxx`.

### Namespace Usage

- Prefer `using` declarations for individual names over namespace-qualified access: `using std::string;` rather than writing `std::string` everywhere.
- Never use `using namespace std;` or other blanket `using namespace` directives.
- One `using` per name, placed at the top of the function or `.cpp` file scope as appropriate.

### Project Layout

- Headers in `include/`, sources in `src/`, tests in `tests/`

## Architecture

CMake project with a single library target (`mylib`) built from `src/` and a main executable (`myproject`) that links to it. Public headers live in `include/`. Tests link against `mylib` and `GTest::gtest_main`.
