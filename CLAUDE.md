# Coding Standards for Claude Code

This document outlines the coding standards for this project when working with Claude Code.

## C++ Style Guidelines

### Header Files
- Use `.hpp` extension for C++ header files
- Use `#pragma once` instead of traditional include guards
- Place headers in the same directory as their corresponding `.cpp` files

### Naming Conventions
**Use camelCase for all variables and functions to match Qt conventions.**

- Member variables: `camelCase` (e.g., `documentTitle`, `photoView`)
- Functions: `camelCase` (e.g., `showImage()`, `openFile()`)
- Local variables: `camelCase` (e.g., `fileName`, `deltaValue`)
- Constants: `camelCase` with descriptive names (e.g., `handleSize`)

```cpp
// ✅ CORRECT - camelCase
QString documentTitle;
void showImage(const QString& filePath);

// ❌ INCORRECT - snake_case
QString document_title;
void show_image(const QString& file_path);
```

### Member Access
**All member variable and member function access MUST use explicit `this->` prefix.**

This rule improves code clarity and makes it immediately obvious when accessing class members versus local variables or parameters.

```cpp
// ✅ CORRECT
void MyClass::setName(const QString& name) {
    this->name = name;
    this->updateDisplay();
}

// ❌ INCORRECT
void MyClass::setName(const QString& name) {
    name = name;  // Ambiguous! Shadows parameter
    updateDisplay();  // Not clear this is a member function
}
```

### Benefits
- **Clarity**: Immediately distinguishes members from locals/parameters
- **No shadowing**: Prevents accidental variable shadowing bugs
- **Consistency**: Uniform style across the codebase
- **Maintainability**: Easier to understand code at a glance

### Automated Enforcement
The explicit `this->` convention can be checked using the provided `check-this-usage.sh` script:

```bash
# Requires clang-query and compile_commands.json
cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
./check-this-usage.sh
```

This check is also integrated into CI/CD workflows to ensure compliance.

## Code Quality Tools

### clang-format
- All code must be formatted with `clang-format`
- Configuration is in `.clang-format` at the project root
- Run: `find src tests -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i`

### clang-tidy
- Code style and naming conventions are enforced via `.clang-tidy`
- Checks camelCase naming for all identifiers
- Run: `clang-tidy src/*.cpp -- -I/path/to/qt/include`

## Testing
- All GUI components must have comprehensive unit tests
- Use Qt Test framework for all tests
- Tests should achieve high coverage of functionality
- Run tests with `xvfb` for CI/CD compatibility

## Git Workflow
- Write clear, descriptive commit messages
- Include "what" and "why" in commit descriptions
- Run tests before committing
