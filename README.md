# C++23 template repository

## Dependencies

We require a compiler that supports C++23. The default configuration uses LLVM
17 and libc++. On Ubuntu/Debian: install `clang-17 libc++-17-dev
libc++abi-17-dev`

## LSP

Run `bazel run :refresh_compile_commands` to generate `compile_commands.json`
for your LSP.
