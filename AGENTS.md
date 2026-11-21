# AGENTS.md

Guidance for coding agents working in this repository. This file is tool-agnostic: any harness that reads
`AGENTS.md` (Codex, Cursor, Gemini CLI, DSH, Claude Code, ...) gets the same instructions. Keep it that way — do not
add tool-specific instruction files.

## Project layout

CMake project for a C++17 static library (`mylib`) plus a demo executable (`myproject`) and GoogleTest unit tests.

| Path | Contents |
|------|----------|
| `include/` | Public headers (`mylib.hpp`); public API lives here |
| `src/` | Library implementation and `main.cpp` |
| `tests/` | GoogleTest suites (`mylib_test.cpp`), linked against `mylib` and `GTest::gtest_main` |
| `scripts/` | `format.sh` (clang-format) and `tidy.sh` (clang-tidy), both bash |
| `.clang-format`, `.clang-tidy` | Formatting and naming/lint rules |
| `build/`, `.cache/` | Generated, git-ignored |

The top-level `CMakeLists.txt` defaults `CMAKE_BUILD_TYPE` to `Release` on single-config generators and turns on
`CMAKE_EXPORT_COMPILE_COMMANDS` unless it is explicitly set. Tests are built when `BUILD_TESTS` is `ON` (the default).

## Build and run

```sh
cmake -B build && cmake --build build
./build/src/myproject
```

With a multi-config generator (Visual Studio, Xcode) pass `--config Release` and run
`build/src/<Config>/myproject(.exe)`.

## Tests

Tests need GoogleTest: `find_package(GTest REQUIRED)` must succeed, so install it with your package manager
(vcpkg, Homebrew, distro packages) before enabling tests.

```sh
cmake -B build -DBUILD_TESTS=ON && cmake --build build
ctest --test-dir build -C Release --output-on-failure            # all tests
./build/tests/mylib_test --gtest_filter='MylibTest.Greet'         # one test
```

On multi-config generators the test binary is under `build/tests/<Config>/`. Add a `TEST(Suite, Case)` for every
behavior change or bug fix. Test sources follow the same convention as the library: `using` declarations at the top
of the file (`using mylib::greet;`), never a `using namespace` directive.

## Format and lint

```sh
bash scripts/format.sh   # clang-format -i over src/, include/, tests/
bash scripts/tidy.sh     # clang-tidy over the same tree; needs build/compile_commands.json
```

Run clang-format before calling a change done, and fix new clang-tidy warnings rather than silencing them (the
config has `WarningsAsErrors: ''`, so they are reported but do not fail the run). On Windows run the scripts from
Git Bash or WSL; `tidy.sh` regenerates `compile_commands.json` with a plain `cmake -B build` if it is missing.

`CMAKE_EXPORT_COMPILE_COMMANDS` only works with single-config generators (Ninja, Unix Makefiles); Visual Studio and
Xcode ignore it and never write `compile_commands.json`. On Windows, configure `build/` with `-G Ninja` when you need
`scripts/tidy.sh` or clangd to see the compilation database.

## Naming conventions (enforced by clang-tidy)

| Element | Style | Example |
|---------|-------|---------|
| Functions / methods | `lower_case` | `do_thing()` |
| Local variables, parameters, local constants | `camelBack` | `myVar` |
| Classes / structs / enums / typedefs / type aliases / template parameters | `CamelCase` | `MyClass` |
| Member variables | `camelBack_` | `memberVar_` (public members: no suffix) |
| Global variables and global constants | `gCamelCase` | `gMaxSize` |
| Static variables and static constants | `sCamelCase` | `sMaxSize` |
| Constexpr variables | `kCamelCase` | `kMaxSize` |
| Enum values | `CamelCase` | `ColorRed` |
| Namespaces | `lower_case` | `mylib` |
| Macros | `UPPER_CASE` | `MY_MACRO` |

## File naming

- Keep file and directory names lowercase: `mylib.hpp`, `my_lib.cpp`, `scripts/run_bench.sh`, never `MyLib.hpp`
  or `MyLib/`.
- Join words with `_`: `my_lib.cpp`, `my_lib.hpp`, `mesh_loader_test.cpp`, `scripts/run_bench.sh`.
- Do not use `-`, dots, camelCase, or PascalCase as word separators in names we choose (`my-lib.cpp`,
  `my.lib.cpp`, `MyLib.cpp` are all wrong).
- Single-word names need no separator: `main.cpp`, `mylib.hpp`.
- Some names are fixed by tooling or convention and keep their exact spelling: `CMakeLists.txt`, `AGENTS.md`,
  `README.md`, `LICENSE`, `compile_commands.json`, and dotfiles such as `.clang-format`, `.clang-tidy`,
  `.gitignore`; build outputs under `build/` follow the generator's own naming (e.g. `Release/`, `Debug/`).
- Rename existing files only when it is part of a deliberate cleanup — do not churn names for the sake of this rule
  in unrelated changes.

## Formatting (clang-format, LLVM-based)

- 2-space indent, 100-column limit, no tabs, `namespace` bodies not indented, case labels not indented.
- Attach braces (K&R); braces are not wrapped for functions, classes, or control statements.
- Pointers and references right-aligned: `int *ptr`, `const std::string &ref`.
- Constructor initializer lists: `BeforeComma`, one per line, no extra indent (`PackConstructorInitializers:
  NextLine`).
- Arguments and parameters: one per line when broken (`BinPackArguments: false`, `BinPackParameters: false`), break
  after the open bracket (`AlignAfterOpenBracket: AlwaysBreak`).
- Short functions may be single-line (`AllowShortFunctionsOnASingleLine: All`); short `if`s and loops never.
- Include order (regrouped, case-insensitive): the file's own header first (`"mylib.hpp"` in `mylib.cpp`), then
  system headers (`<...>`), then other project headers.
- Braced lists: C++11 style, no space before the braces; align operands and trailing comments.

## Static analysis (clang-tidy)

Enabled groups: `bugprone-*`, `cppcoreguidelines-*`, `misc-*`, `modernize-*`, `performance-*`, `readability-*`.

Notable disabled checks: `bugprone-easily-swappable-parameters`, `bugprone-exception-escape`,
`cppcoreguidelines-avoid-magic-numbers`, `cppcoreguidelines-owning-memory`,
`cppcoreguidelines-pro-bounds-array-to-pointer-decay`, `cppcoreguidelines-pro-bounds-pointer-arithmetic`,
`misc-include-cleaner`, `misc-use-internal-linkage`, `misc-no-recursion`, `modernize-use-trailing-return-type`,
`modernize-use-nodiscard`, `performance-enum-size`, `readability-magic-numbers`,
`readability-identifier-length`, `readability-implicit-bool-conversion`, `llvm-header-guard`.

Thresholds: function cognitive complexity 50, function length 100 lines.

Header guards: use `#ifndef`/`#define` (not `#pragma once`) with extensions `.hpp`, `.h`, `.hh`, `.hxx`. The
`llvm-header-guard` check is currently disabled, so follow this as a convention.

## Namespace usage

- Every C++ source file — `src/*.cpp` and `tests/*.cpp` alike — starts with `using` declarations for the names it
  uses, placed at the top of the file directly after the includes.
- Any entity may be imported this way: functions, classes/structs/enums, variables and constants, type aliases, and
  other names from `std`, `mylib`, or third-party namespaces (`using std::string;`, `using std::cout;`,
  `using mylib::greet;`, `using std::vector;`).
- Never introduce a whole namespace: no `using namespace std;`, `using namespace mylib;`, or any other blanket
  `using namespace` directive.
- Prefer the declaration over namespace-qualified access in function bodies; add another `using` instead of writing
  `std::`/`mylib::` inline. Full qualification is only a fallback where a declaration cannot work (a header, a
  conflicting name, a macro).
- One declaration per name. A function-scoped `using` is acceptable when only that function needs the name.
- Headers keep names qualified (`std::string`) or scope a `using` inside the project namespace; do not put `using`
  declarations at global scope in a header.

## Changing the project

- New public API: declare in `include/mylib.hpp`, implement in `src/mylib.cpp`.
- New source file: name it with lowercase `_`-joined words (see File naming) and add it to the matching
  `CMakeLists.txt` (`src/` or `tests/`) in the same change.
- New dependency: prefer `find_package`, document the package and version requirement here, avoid vendoring
  binaries.
- Keep `build/` and `.cache/` out of version control; `compile_commands.json` is generated by CMake.
- Update this file whenever build steps, tooling, or layout change.

## Ground rules

- `AGENTS.md` is the single source of agent guidance; do not add tool-specific instruction files (e.g.
  `CLAUDE.md`). If a tool only understands its own file name, point it at this file instead of copying content.
- Prefer the smallest change that keeps the build and tests green, and run the commands above before reporting
  done.
