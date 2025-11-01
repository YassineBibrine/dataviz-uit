# Contributing Guidelines

## Code of Conduct

We are committed to providing a welcoming and inclusive environment for all contributors.

- Be respectful and constructive in all interactions
- Acknowledge and credit contributors' work
- Report issues privately to maintainers if needed

## Getting Started

1. Read `README.md` for project overview
2. Follow `SETUP.md` for environment setup
3. Read your team's implementation guide in `docs/`
4. Create a feature branch for development

## Development Workflow

### 1. Create Feature Branch

```bash
git checkout -b feature/your-feature-name
```

Use descriptive branch names:
- `feature/ui-syntax-highlighting` - Feature work
- `fix/clang-rewriter-segfault` - Bug fixes
- `docs/api-reference` - Documentation
- `refactor/logger-optimization` - Refactoring

### 2. Make Changes

Follow the guidelines in your team's implementation guide (UI, C2V, or V2C).

### 3. Test Locally

```bash
# Build and test
cd build
cmake --build . --config Release
ctest --verbose
```

### 4. Commit with Meaningful Messages

```bash
git add .
git commit -m "Brief description (50 chars max)

Longer explanation if needed. Explain *why*, not what.
- Separate concerns with bullet points
- Reference related issues with #123
"
```

**Commit Message Format**:
- First line: 50 characters, imperative mood
- Blank line
- Detailed explanation (wrapped at 72 characters)
- References to issues/PRs

**Good Example**:
```
Implement C++ syntax highlighting in code editor

- Add QSyntaxHighlighter subclass for C++
- Support common C++ keywords and operators
- Apply keyword, string, and comment coloring
- Handles preprocessor directives

Fixes #42
```

**Bad Example**:
```
fixed stuff
```

### 5. Push and Create Pull Request

```bash
git push origin feature/your-feature-name
```

Then create PR on GitHub with:
- Clear title describing the change
- Description of what was changed and why
- Reference to related issues
- Checklist of testing performed

## Code Standards

### C++ Style Guide

**File Organization**:
```cpp
// Header guard
#ifndef COMPONENT_NAME_H
#define COMPONENT_NAME_H

// System includes
#include <vector>
#include <string>

// Qt includes (if applicable)
#include <QWidget>

// Project includes
#include "core/logger.h"

// Declarations and implementations
class MyClass { ... };

#endif // COMPONENT_NAME_H
```

**Naming Conventions**:
- Classes: `PascalCase` (`MyClass`, `CodeEditor`)
- Functions: `camelCase` (`parseFile()`, `extractStructures()`)
- Variables: `camelCase` (`nodeCount`, `graphData`)
- Constants: `UPPER_SNAKE_CASE` (`MAX_NODES`, `DEFAULT_TIMEOUT`)
- Member variables: `camelCase` with optional `m_` prefix (`m_buffer`, `nodeIndex`)
- Private methods: Start with `do` or just use naming convention (`doParseFile()`)

**Comments**:
```cpp
// Use // for single-line comments
// Explain *why*, not *what* (code should be self-documenting)

/*
 * Use /* */ for multi-line comments
 * Particularly for file headers and class documentation
 */

/// Use /// for Doxygen documentation
/// @param name Parameter description
/// @return Return value description
void myFunction(const std::string &name);
```

**Formatting**:
- Indentation: 4 spaces (not tabs)
- Line length: 100 characters (soft limit, try to stay under)
- Braces: Opening brace on same line (Stroustrup style)
- Pointers/References: `Type *ptr` and `Type &ref` (star/ampersand with type)

```cpp
// Good
void processData(const std::string &data)
{
    if (data.empty()) {
        return;
    }
    // Process...
}

// Bad
void processData(const string& data){
if (data.empty())
    return;
    // ...
}
```

**Qt Conventions**:
- Use Qt containers (`QString`, `QVector`, etc.)
- Use Qt memory management (parenting, smart pointers)
- Connect signals/slots using new syntax: `connect(sender, QOverload<Args>::of(&Sender::signal), ...)`
- Declare `Q_OBJECT` in classes using signals/slots

### CMake Standards

```cmake
# Clear project name
project(ComponentName)

# Use explicit source lists
set(SOURCES
    file1.cpp
  file2.cpp
)

set(HEADERS
    file1.h
 file2.h
)

# Create library with both sources and headers
add_library(component_name ${SOURCES} ${HEADERS})

# Public vs private include directories
target_include_directories(component_name PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(component_name PRIVATE
    ${PRIVATE_INCLUDE_DIR}
)

# Link libraries explicitly
target_link_libraries(component_name
    PUBLIC some_library
    PRIVATE another_library
)
```

## Documentation Standards

### Header File Documentation

```cpp
/**
 * @class MyClass
 * @brief Brief one-line description
 * 
 * Longer description explaining the purpose,
 * responsibilities, and usage patterns of the class.
 * 
 * @note Any important notes
 * @warning Any warnings
 * 
 * Example usage:
 * @code
 *     MyClass obj;
 *     obj.doSomething();
 * @endcode
 */
class MyClass {
public:
    /**
     * @brief Brief description
     * 
     * @param param1 Description of param1
 * @param param2 Description of param2
     * @return Description of return value
     * @throws std::invalid_argument If validation fails
     */
    int myMethod(const std::string &param1, int param2);
};
```

### README Sections

READMEs should include:
- Brief description
- Key features
- Installation/setup
- Usage examples
- Team responsibilities (for major components)
- Troubleshooting (if applicable)

## Testing

### Unit Tests

Create tests in `tests/` directory with same structure as source:

```cpp
// tests/test_ast_parser.cpp
#include <gtest/gtest.h>
#include "analysis/clang_integration/ast_parser.h"

TEST(ASTParserTest, ParseValidCppFile) {
    ASTParser parser;
    bool result = parser.parseFile("test_files/simple.cpp");
    EXPECT_TRUE(result);
}

TEST(ASTParserTest, HandleInvalidFile) {
    ASTParser parser;
    bool result = parser.parseFile("nonexistent.cpp");
    EXPECT_FALSE(result);
}
```

### Integration Tests

Test interaction between components:
```cpp
// tests/integration/test_c2v_pipeline.cpp
TEST(C2VPipelineTest, FullAnalysisFlow) {
    // Load code
    ASTParser parser;
    parser.parseFile("test.cpp");
    
    // Extract structures
    StructureExtractor extractor(parser.getASTContext());
    auto structures = extractor.extractStructures();
    
    // Verify
    ASSERT_FALSE(structures.empty());
    EXPECT_EQ(structures[0].className, "LinkedList");
}
```

### Running Tests

```bash
cd build
ctest --verbose     # Run all tests
ctest -R "test_name"    # Run specific test
ctest -E "integration"    # Exclude integration tests
```

## Code Review Checklist

Before submitting a PR, ensure:

- [ ] Code follows style guide
- [ ] No compiler warnings
- [ ] Unit tests added/updated
- [ ] Tests pass locally
- [ ] Documentation updated
- [ ] No large unrelated changes
- [ ] Commit messages are clear
- [ ] PR description explains *why*

## Performance Considerations

### Profiling

```bash
# Use appropriate tools for your platform
# Windows: Visual Studio Profiler
# macOS: Xcode Instruments
# Linux: perf, valgrind, or gprof
```

### Optimization Guidelines

1. **Measure before optimizing**: Use profiler to identify bottlenecks
2. **Algorithm first**: Better algorithm beats micro-optimization
3. **Cache efficiency**: Consider memory locality
4. **Avoid premature optimization**: Write clear code first
5. **Document optimization**: Explain why optimization was necessary

### Memory Management

- Use RAII (Resource Acquisition Is Initialization)
- Prefer `std::unique_ptr` over raw `new`/`delete`
- Use `std::shared_ptr` only when multiple owners needed
- Avoid circular references
- Check for memory leaks with Valgrind (Linux) or Dr. Memory (Windows)

## Security Guidelines

### Code Security

- Validate all user input
- Use safe string functions (not `strcpy`, `strcat`)
- Avoid buffer overflows
- Check return values for errors
- Use const where appropriate
- Limit scope of variables

### Dependency Security

- Keep dependencies updated
- Review security advisories
- Use stable versions in production
- Pin dependency versions in CMakeLists.txt

## Team Coordination

### Before Starting Major Work

1. Check if anyone else is working on it (check issues/PRs)
2. Comment on related issues to claim work
3. Discuss approach with team lead
4. Create issue for tracking

### Code Review Process

1. **Author**: Create detailed PR
2. **Reviewer**: Review code thoroughly
3. **Discussion**: Address comments constructively
4. **Approval**: PR can be merged after approval
5. **Merge**: Use "Squash and merge" for feature branches

### Communication

- Use GitHub issues for discussion
- Use PR comments for code-specific feedback
- Use team chat for urgent items
- Document decisions in issue comments

## Debugging Tips

### Useful Tools

- **gdb** (Linux/macOS): `gdb ./build/dataviz-app`
- **lldb** (macOS): `lldb ./build/dataviz-app`
- **Visual Studio Debugger**: Built-in (F5 to debug)
- **Qt Creator**: Debug with breakpoints and watches
- **Valgrind**: Memory leak detection

### Common Debugging Patterns

```cpp
// Add debug output
LOG_DEBUG("Variable value: " << value);

// Use assertions
ASSERT(pointer != nullptr, "Pointer should not be null");

// Add breakpoints in IDE
// - Click in gutter to add breakpoint
// - Conditional breakpoints: right-click ? Filter
```

## Updating Documentation

When making changes:

1. **Update code comments**: Explain non-obvious logic
2. **Update README**: If user-facing changes
3. **Update API docs**: If public interface changes
4. **Update team guide**: If process/structure changes
5. **Add examples**: If new feature added

## Release Process

### Version Numbering

Use Semantic Versioning: `MAJOR.MINOR.PATCH`
- MAJOR: Incompatible API changes
- MINOR: Backward-compatible new features
- PATCH: Bug fixes

### Release Checklist

- [ ] Update version in CMakeLists.txt
- [ ] Update CHANGELOG.md
- [ ] Update version in docs
- [ ] Create git tag: `git tag v1.0.0`
- [ ] Push tag: `git push origin v1.0.0`
- [ ] Create release on GitHub

## Getting Help

- **Questions**: Check documentation first, then ask team
- **Issues**: Create GitHub issue with detailed description
- **Code Review**: Request review from team lead or codebase expert
- **Performance**: Profile first, then discuss optimization approach

## Thank You

Thank you for contributing to the C++ Data Structure Visualization Application! Your work helps create a powerful tool for DSA education and understanding.

---

**Remember**: Good code is clear, tested, documented, and kind to future maintainers (including yourself!).
