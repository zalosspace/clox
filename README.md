# Clox: Lox Bytecode Compiler and Virtual Machine
## Get Started (Linux)
- Setup
```
# Setup Cmake
cmake -S . -B build
# Resolve Include Path
ln -s build/compile_commands.json .
```
- Compile code

```
cd build
# Build Source And Run
cmake --build . && ./clox
```
